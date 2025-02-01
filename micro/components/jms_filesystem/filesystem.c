#include "jms_filesystem.h"
#include <esp_littlefs.h>
#include <esp_log.h>
#include <stdio.h>
#include <string.h>

#define TAG "JMS_FS"

static bool fs_initialized = false;

/**
 * @brief Initializes the filesystem.
 */
jms_err_t jms_fs_init(void)
{
    if (fs_initialized)
    {
        return JMS_OK;
    }

    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/littlefs",
        .partition_label = "littlefs",
        .format_if_mount_failed = true,
        .dont_mount = false,
    };

    esp_err_t ret = esp_vfs_littlefs_register(&conf);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize LittleFS: %s", esp_err_to_name(ret));
        return JMS_ERR_FS_MOUNT_FAILED;
    }

    fs_initialized = true;
    ESP_LOGI(TAG, "LittleFS initialized.");
    return JMS_OK;
}

/**
 * @brief Opens a file for reading.
 */
jms_err_t jms_fs_open(const char* path, jms_fs_handle_t* handle)
{
    if (!fs_initialized)
    {
        ESP_LOGE(TAG, "Filesystem not initialized");
        return JMS_ERR_FS_NOT_INITIALIZED;
    }
    if (!path || !handle)
    {
        ESP_LOGE(TAG, "Invalid arguments to jms_fs_open");
        return JMS_ERR_INVALID_ARG;
    }

    handle->file = fopen(path, "r");
    if (!handle->file)
    {
        ESP_LOGW(TAG, "File not found: %s", path);
        return JMS_ERR_FS_FILE_NOT_FOUND;
    }

    // Get file size
    fseek(handle->file, 0, SEEK_END);
    handle->file_size = ftell(handle->file);
    fseek(handle->file, 0, SEEK_SET);

    return JMS_OK;
}

/**
 * @brief Reads a chunk from an open file.
 */
jms_err_t jms_fs_read_chunk(jms_fs_handle_t* handle, void* buffer, size_t buffer_size,
                            size_t* out_bytes_read)
{
    if (!fs_initialized || !handle || !handle->file || !buffer || !out_bytes_read)
    {
        ESP_LOGE(TAG, "Invalid arguments to jms_fs_read_chunk");
        return JMS_ERR_INVALID_ARG;
    }

    *out_bytes_read = fread(buffer, 1, buffer_size, handle->file);
    if (*out_bytes_read == 0 && ferror(handle->file))
    {
        ESP_LOGE(TAG, "Error reading file");
        return JMS_ERR_FS_READ_FAILED;
    }

    return JMS_OK;
}

/**
 * @brief Closes an open file.
 */
void jms_fs_close(jms_fs_handle_t* handle)
{
    if (handle && handle->file)
    {
        fclose(handle->file);
        handle->file = NULL;
    }
}
