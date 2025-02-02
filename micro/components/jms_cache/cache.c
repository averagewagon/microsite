#include "jms_cache.h"
#include "jms_error.h"
#include "jms_filesystem.h"
#include <esp_heap_caps.h>
#include <esp_log.h>
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

        // Open file in LittleFS
        jms_fs_handle_t file_handle;
        if (jms_fs_open(files[i], &file_handle) != JMS_OK)
        {
            ESP_LOGW(TAG, "File not found: %s", files[i]);
            continue;
        }

        size_t file_size = file_handle.file_size;
        size_t path_length = strlen(files[i]) + 1;
        size_t metadata_size = sizeof(jms_cache_entry_t) + path_length;

        if (cache_region_front + metadata_size >= cache_region_back - file_size)
        {
            ESP_LOGW(TAG, "Skipping %s (not enough space), continuing to next file", files[i]);
            jms_fs_close(&file_handle);
            continue;
        }

        // Allocate metadata at front
        jms_cache_entry_t* new_entry = (jms_cache_entry_t*)cache_region_front;
        cache_region_front += sizeof(jms_cache_entry_t);

        new_entry->filepath = (char*)cache_region_front;
        memcpy(new_entry->filepath, files[i], path_length);
        cache_region_front += path_length;

        // Allocate file data at back
        cache_region_back -= file_size;
        new_entry->data = cache_region_back;
        new_entry->size = file_size;

        // Read file into allocated memory
        size_t bytes_read;
        if (jms_fs_read_chunk(&file_handle, new_entry->data, file_size, &bytes_read) != JMS_OK ||
            bytes_read != file_size)
        {
            ESP_LOGE(TAG, "Failed to read file: %s", files[i]);
            cache_region_back += file_size; // Roll back allocation
            cache_region_front -= metadata_size;
            jms_fs_close(&file_handle);
            continue;
        }

        jms_fs_close(&file_handle);

        cache_used += metadata_size + file_size;
        cache_count++;

        ESP_LOGI(TAG, "Cached file: %s (%zu bytes) - %zu/%zu bytes used", files[i], file_size,
                 cache_used, cache_region_size);
    }

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
