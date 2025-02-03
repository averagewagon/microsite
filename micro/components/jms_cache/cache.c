#include "jms_cache.h"
#include "jms_error.h"
#include "jms_filesystem.h"
#include <dirent.h>
#include <esp_heap_caps.h>
#include <esp_log.h>
#include <stdio.h>
#include <string.h>

#define TAG "JMS_CACHE"

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
 * @brief Iterates through LittleFS and caches additional files.
 */
static void cache_remaining_files()
{
    ESP_LOGI(TAG, "Expanding cache with additional files...");

    DIR* dir = opendir("/littlefs/");
    if (!dir)
    {
        ESP_LOGW(TAG, "Failed to open LittleFS root for scanning.");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        char full_path[256];
        snprintf(full_path, sizeof(full_path), "/littlefs/%.245s", entry->d_name);

        // Check if file is already cached
        const uint8_t* cached_data;
        size_t cached_size;
        if (jms_cache_get(full_path, &cached_data, &cached_size) == JMS_OK)
        {
            ESP_LOGI(TAG, "Skipping %s (already cached)", full_path);
            continue;
        }

        // If it's a file, attempt to cache it
        if (entry->d_type == DT_REG)
        {
            if (cache_file(full_path) != JMS_OK)
            {
                ESP_LOGW(TAG, "Unable to cache %s (not enough space or error)", full_path);
            }
        }
    }
    closedir(dir);

    ESP_LOGI(TAG, "Automatic caching complete.");
}

/**
 * @brief Initializes the cache with a fixed memory region and preloads files.
 */
jms_err_t jms_cache_init(const char** files, size_t file_count, size_t max_cache_size)
{
    ESP_LOGI(TAG, "Initializing cache with max size: %zu bytes", max_cache_size);

    // Allocate the fixed cache region in SPIRAM
    cache_region = (uint8_t*)heap_caps_malloc(max_cache_size, MALLOC_CAP_SPIRAM);
    if (!cache_region)
    {
        ESP_LOGE(TAG, "Failed to allocate cache region");
        return JMS_ERR_CACHE_ALLOC;
    }

    // Initialize pointers for split bump allocation
    cache_region_size = max_cache_size;
    cache_region_front = cache_region;
    cache_region_back = cache_region + max_cache_size;

    for (size_t i = 0; i < file_count; i++)
    {
        if (cache_region_front >= cache_region_back)
        {
            ESP_LOGW(TAG, "Cache is full, stopping at %zu bytes used", cache_used);
            return JMS_OK;
        }

        // Validate file path length
        if (strlen(files[i]) >= JMS_CACHE_MAX_PATH)
        {
            ESP_LOGW(TAG, "Skipping %s (file path exceeds %d bytes)", files[i], JMS_CACHE_MAX_PATH);
            continue;
        }

        // Cache the provided file
        cache_file(files[i]);
    }

    // Expand caching with additional files
    cache_remaining_files();

    ESP_LOGI(TAG, "Cache initialization complete: %zu files, %zu bytes used", cache_count,
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
