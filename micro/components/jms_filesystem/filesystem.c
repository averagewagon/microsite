#include "jms_filesystem.h"

#include <esp_littlefs.h>
#include <esp_log.h>
#include <stdio.h>

#include "jms_error.h"

#define TAG "JMS_FS"

static bool fs_initialized = false; // Track filesystem state

jms_err_t jms_fs_init(void)
{
    if (fs_initialized)
    {
        return JMS_OK; // Already initialized
    }

    // Configure LittleFS
    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/littlefs",
        .partition_label = "littlefs",
        .format_if_mount_failed = true,
        .dont_mount = false,
    };

    // Initialize LittleFS
    esp_err_t ret = esp_vfs_littlefs_register(&conf);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize LittleFS: %s", esp_err_to_name(ret));
        return JMS_ERR_FS_MOUNT_FAILED;
    }

    // Mark as initialized
    fs_initialized = true;

    // Log filesystem usage
    size_t total = 0, used = 0;
    ret = esp_littlefs_info(conf.partition_label, &total, &used);
    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "LittleFS Initialized - Total: %zu bytes, Used: %zu bytes", total, used);
    }
    else
    {
        ESP_LOGE(TAG, "Failed to get LittleFS info: %s", esp_err_to_name(ret));
        return JMS_ERR_FS_READ_FAILED;
    }

    return JMS_OK;
}

jms_err_t jms_fs_read(const char* path, void* buffer, size_t buffer_size, size_t* out_bytes_read)
{
    if (!fs_initialized)
    {
        ESP_LOGE(TAG, "Filesystem not initialized");
        return JMS_ERR_FS_NOT_INITIALIZED;
    }
    if (!path || !buffer || !out_bytes_read)
    {
        ESP_LOGE(TAG, "Invalid arguments to jms_fs_read");
        return JMS_ERR_INVALID_ARG;
    }

    FILE* f = fopen(path, "r");
    if (!f)
    {
        ESP_LOGW(TAG, "File not found: %s", path);
        return JMS_ERR_FS_FILE_NOT_FOUND;
    }

    size_t bytes_read = fread(buffer, 1, buffer_size, f);
    if (bytes_read == 0 && ferror(f))
    {
        ESP_LOGE(TAG, "Error reading file: %s", path);
        fclose(f);
        return JMS_ERR_FS_READ_FAILED;
    }

    fclose(f);
    *out_bytes_read = bytes_read;
    return JMS_OK;
}
