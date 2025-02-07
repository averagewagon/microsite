#include "jms_cache.h"
#include "jms_error.h"
#include "jms_filesystem.h"
#include <dirent.h>
#include <esp_heap_caps.h>
#include <esp_log.h>
#include <stdio.h>
#include <string.h>

#define TAG "JMS_CACHE"

#define CACHE_FILE_PATH "/littlefs/cache.txt"
#define MAX_LINE_LENGTH 128

typedef struct
{
    char* filepath; // Pointer to file path (stored at cache_region_front)
    uint8_t* data;  // Pointer to file contents (stored at cache_region_back)
    size_t size;    // File size
} jms_cache_entry_t;

// Fixed-size memory region for caching
static uint8_t* cache_region = NULL;
static size_t cache_region_size = 0;
static uint8_t* cache_region_front = NULL;
static uint8_t* cache_region_back = NULL;
static size_t cache_used = 0;
static size_t cache_count = 0;

/**
 * @brief Attempts to cache a given file.
 * @param path The file path to cache.
 * @return JMS_OK if cached successfully, an error code otherwise.
 */
static jms_err_t cache_file(const char* path)
{
    // Validate file existence
    if (jms_fs_exists(path) != JMS_OK)
    {
        return JMS_ERR_FS_FILE_NOT_FOUND;
    }

    // Open file in LittleFS
    jms_fs_handle_t file_handle;
    if (jms_fs_open(path, &file_handle) != JMS_OK)
    {
        ESP_LOGW(TAG, "File not found: %s", path);
        return JMS_ERR_FS_FILE_NOT_FOUND;
    }

    size_t file_size = file_handle.file_size;
    size_t path_length = strlen(path) + 1;
    size_t metadata_size = sizeof(jms_cache_entry_t) + path_length;

    // Check if space is available in cache
    if (cache_region_front + metadata_size >= cache_region_back - file_size)
    {
        ESP_LOGW(TAG, "Skipping %s (not enough space)", path);
        jms_fs_close(&file_handle);
        return JMS_ERR_CACHE_FULL;
    }

    // Allocate metadata at the front
    jms_cache_entry_t* new_entry = (jms_cache_entry_t*)cache_region_front;
    cache_region_front += sizeof(jms_cache_entry_t);

    new_entry->filepath = (char*)cache_region_front;
    memcpy(new_entry->filepath, path, path_length);
    cache_region_front += path_length;

    // Allocate file data at the back
    cache_region_back -= file_size;
    new_entry->data = cache_region_back;
    new_entry->size = file_size;

    // Read file into allocated memory
    size_t bytes_read;
    if (jms_fs_read_chunk(&file_handle, new_entry->data, file_size, &bytes_read) != JMS_OK ||
        bytes_read != file_size)
    {
        ESP_LOGE(TAG, "Failed to read file: %s", path);
        cache_region_back += file_size; // Roll back allocation
        cache_region_front -= metadata_size;
        jms_fs_close(&file_handle);
        return JMS_ERR_FS_READ_FAILED;
    }

    jms_fs_close(&file_handle);

    cache_used += metadata_size + file_size;
    cache_count++;

    ESP_LOGI(TAG, "Cached file: %s (%zu bytes) - %zu/%zu bytes used", path, file_size, cache_used,
             cache_region_size);
    return JMS_OK;
}

/**
 * @brief Initializes the cache by preloading files listed in cache.txt into SPIRAM.
 *
 * @param max_cache_size The maximum memory (bytes) to use for caching.
 * @return JMS_OK if cache initialized successfully, or an error code.
 */
jms_err_t jms_cache_init(size_t max_cache_size)
{
    ESP_LOGI(TAG, "Initializing cache with max size: %zu bytes", max_cache_size);

    // Allocate the fixed cache region in SPIRAM
    cache_region = (uint8_t*)heap_caps_malloc(max_cache_size, MALLOC_CAP_SPIRAM);
    if (!cache_region)
    {
        ESP_LOGE(TAG, "Failed to allocate cache region");
        return JMS_ERR_CACHE_ALLOC;
    }

    cache_region_size = max_cache_size;
    cache_region_front = cache_region;
    cache_region_back = cache_region + max_cache_size;

    ESP_LOGI(TAG, "Cache memory allocated successfully.");

    // Open cache file
    jms_fs_handle_t file_handle;
    if (jms_fs_open(CACHE_FILE_PATH, &file_handle) != JMS_OK)
    {
        ESP_LOGE(TAG, "Failed to open cache list file: %s", CACHE_FILE_PATH);
        return JMS_ERR_CACHE_FILE_NOT_FOUND;
    }

    ESP_LOGI(TAG, "Opened cache file: %s", CACHE_FILE_PATH);

    char line[MAX_LINE_LENGTH];
    char full_path[MAX_LINE_LENGTH + 11]; // Extra space for "littlefs/"
    size_t file_index = 0;

    // Use fgets() to read one line at a time
    while (fgets(line, sizeof(line), file_handle.file))
    {
        // Trim newline and carriage return characters
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
        {
            line[--len] = '\0';
        }

        if (len == 0)
        {
            continue; // Skip empty lines
        }

        // Prepend "littlefs/" to the path
        snprintf(full_path, sizeof(full_path), "/littlefs/%s", line);

        ESP_LOGI(TAG, "Processing file [%zu]: %s", file_index, full_path);

        // Check if file exists before caching
        if (jms_fs_exists(full_path) != JMS_OK)
        {
            ESP_LOGW(TAG, "File not found: %s", full_path);
            file_index++;
            continue;
        }

        ESP_LOGI(TAG, "Caching file [%zu]: %s", file_index, full_path);
        jms_err_t cache_status = cache_file(full_path);

        if (cache_status != JMS_OK)
        {
            ESP_LOGW(TAG, "Skipping %s (cache_file failed with error code: %d)", full_path,
                     cache_status);
        }
        else
        {
            ESP_LOGI(TAG, "Cached file [%zu]: %s", file_index, full_path);
        }

        file_index++;
    }

    jms_fs_close(&file_handle);

    ESP_LOGI(TAG, "Cache initialization complete. %zu files attempted, %zu bytes used", file_index,
             cache_used);
    return JMS_OK;
}

/**
 * @brief Retrieves a cached file.
 */
jms_err_t jms_cache_get(const char* path, const uint8_t** out_data, size_t* out_size)
{
    if (!cache_region)
    {
        ESP_LOGE(TAG, "Cache not initialized");
        return JMS_ERR_CACHE_NOT_INITIALIZED;
    }

    // Iterate over cache entries stored at cache_region_front
    uint8_t* current = cache_region;
    while (current < cache_region_front)
    {
        jms_cache_entry_t* entry = (jms_cache_entry_t*)current;
        if (strcmp(entry->filepath, path) == 0)
        {
            *out_data = entry->data;
            *out_size = entry->size;
            return JMS_OK;
        }
        current += sizeof(jms_cache_entry_t) + strlen(entry->filepath) + 1;
    }

    ESP_LOGW(TAG, "Cache miss: %s", path);
    return JMS_ERR_CACHE_MISS;
}
