#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <esp_event.h>
#include <esp_log.h>
#include <esp_netif.h>
#include <esp_system.h>
#include <esp_tls.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>

#include "protocol_examples_common.h"
#include "sdkconfig.h"

#include "jms_cache.h"
#include "jms_error.h"
#include "jms_filesystem.h"
#include "jms_mime.h"
#include "jms_webserver.h"

static const char* TAG = "microsite";

#define CACHE_MAX_SIZE (7 * 1024 * 1024) // 7MB cache

static jms_err_t serve_file(const jms_ws_request_t* request, char* filepath)
{
    jms_fs_handle_t file_handle;
    char buffer[4096];
    char brotli_filepath[256];
    char base_filepath[256];
    size_t bytes_read = 0;
    const char* mime_type;
    const char* selected_filepath = filepath;
    const char* content_encoding = NULL;
    const uint8_t* cached_data;
    size_t cached_size;
    bool is_html = strstr(filepath, ".html") != NULL;

    ESP_LOGI(TAG, "Serving file: %s", filepath);

    // Step 1: If this is an HTML file, check for Brotli-encoded version in cache
    if (is_html && strstr(request->accept_encoding, "br") != NULL)
    {
        snprintf(brotli_filepath, sizeof(brotli_filepath), "%s.br", filepath);
        if (jms_cache_get(brotli_filepath, &cached_data, &cached_size) == JMS_OK)
        {
            ESP_LOGI(TAG, "Serving Brotli version from cache: %s", brotli_filepath);

            // Determine MIME type from the original file (without .br extension)
            if (jms_mime_get_type(filepath, &mime_type) != JMS_OK)
            {
                mime_type = "application/octet-stream";
            }

            jms_ws_set_response_header(request, "Status", "200 OK");
            jms_ws_set_response_header(request, "Content-Type", mime_type);
            jms_ws_set_response_header(request, "Content-Encoding", "br");
            jms_ws_set_response_header(request, "Cache-Control", "max-age=86400");

            return jms_ws_response_send(request, (const char*)cached_data, cached_size);
        }
    }

    // Step 2: Check if the original file is in cache
    if (jms_cache_get(filepath, &cached_data, &cached_size) == JMS_OK)
    {
        ESP_LOGI(TAG, "Serving from cache: %s", filepath);

        if (jms_mime_get_type(filepath, &mime_type) != JMS_OK)
        {
            mime_type = "application/octet-stream";
        }

        jms_ws_set_response_header(request, "Status", "200 OK");
        jms_ws_set_response_header(request, "Content-Type", mime_type);
        jms_ws_set_response_header(request, "Cache-Control", "max-age=86400");

        return jms_ws_response_send(request, (const char*)cached_data, cached_size);
    }

    // Step 3: If this is an HTML file, check for Brotli version in the filesystem
    if (is_html && strstr(request->accept_encoding, "br") != NULL)
    {
        snprintf(brotli_filepath, sizeof(brotli_filepath), "%s.br", filepath);
        if (jms_fs_exists(brotli_filepath) == JMS_OK)
        {
            ESP_LOGI(TAG, "Using Brotli version from filesystem: %s", brotli_filepath);
            selected_filepath = brotli_filepath;
            content_encoding = "br";

            // Extract base file path without .br extension
            strncpy(base_filepath, filepath, sizeof(base_filepath));
            base_filepath[sizeof(base_filepath) - 1] = '\0';

            // Determine MIME type from the original file
            if (jms_mime_get_type(base_filepath, &mime_type) != JMS_OK)
            {
                mime_type = "application/octet-stream";
            }
        }
    }
    else
    {
        // Determine MIME type from the original file
        if (jms_mime_get_type(filepath, &mime_type) != JMS_OK)
        {
            mime_type = "application/octet-stream";
        }
    }

    // Step 4: Verify file exists and open it
    if (jms_fs_exists(selected_filepath) != JMS_OK ||
        jms_fs_open(selected_filepath, &file_handle) != JMS_OK)
    {
        ESP_LOGW(TAG, "File not found: %s", selected_filepath);
        return JMS_ERR_FS_FILE_NOT_FOUND;
    }

    // Step 5: Set response headers
    jms_ws_set_response_header(request, "Status", "200 OK");
    jms_ws_set_response_header(request, "Content-Type", mime_type);
    if (content_encoding)
    {
        jms_ws_set_response_header(request, "Content-Encoding", content_encoding);
    }
    jms_ws_set_response_header(request, "Cache-Control", "max-age=86400");

    // Step 6: Stream file content in chunks
    while (jms_fs_read_chunk(&file_handle, buffer, sizeof(buffer), &bytes_read) ==
               JMS_OK &&
           bytes_read > 0)
    {
        jms_ws_response_send_chunk(request, buffer, bytes_read);
    }

    // End chunked response
    jms_ws_response_send_chunk(request, NULL, 0);
    jms_fs_close(&file_handle);
    return JMS_OK;
}

static jms_err_t microsite_request_handler(const jms_ws_request_t* request)
{
    char filepath[256];

    ESP_LOGI(TAG, "Incoming request: %s", request->path);

    // Step 1: Determine base filepath
    if (strcmp(request->path, "/") == 0)
    {
        strcpy(filepath, "/littlefs/index.html");
    }
    else if (request->path[strlen(request->path) - 1] == '/')
    {
        snprintf(filepath, sizeof(filepath), "/littlefs%sindex.html", request->path);
    }
    else
    {
        snprintf(filepath, sizeof(filepath), "/littlefs%s", request->path);
    }

    ESP_LOGI(TAG, "Resolved file path: %s", filepath);

    // Set common response headers
    jms_ws_set_response_header(
        request, "Content-Security-Policy",
        "default-src 'self'; script-src 'none'; style-src 'self'; img-src 'self';");
    jms_ws_set_response_header(request, "Referrer-Policy",
                               "strict-origin-when-cross-origin");
    jms_ws_set_response_header(request, "Strict-Transport-Security",
                               "max-age=31536000; includeSubDomains; preload");
    jms_ws_set_response_header(request, "X-Content-Type-Options", "nosniff");
    jms_ws_set_response_header(request, "X-Frame-Options", "SAMEORIGIN");
    jms_ws_set_response_header(request, "Cross-Origin-Resource-Policy", "same-origin");

    // Step 2: Serve the file (handles Brotli fallback logic)
    if (serve_file(request, filepath) == JMS_OK)
    {
        return JMS_OK;
    }

    // Step 3: Serve 404.html if file is missing
    ESP_LOGW(TAG, "File not found: %s, trying 404 fallback.", filepath);
    strcpy(filepath, "/littlefs/404.html");

    if (serve_file(request, filepath) == JMS_OK)
    {
        return JMS_OK;
    }

    // Step 4: If 404.html is missing, serve built-in response
    ESP_LOGW(TAG, "404.html not found, serving built-in response.");
    jms_ws_set_response_header(request, "Status", "404 Not Found");
    jms_ws_set_response_header(request, "Content-Type", "text/html");
    jms_ws_set_response_header(request, "Cache-Control", "no-cache");

    return jms_ws_response_send(
        request, "<h1>404 Not Found (and my 404 page was broken, too?!?)</h1>", 22);
}

/**
 * @brief Starts the webserver with the compiled-in SSL certificate and private key.
 */
static void connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id,
                            void* event_data)
{
    extern const unsigned char servercert_start[] asm("_binary_fullchain_pem_start");
    extern const unsigned char servercert_end[] asm("_binary_fullchain_pem_end");
    extern const unsigned char prvtkey_pem_start[] asm("_binary_privkey_pem_start");
    extern const unsigned char prvtkey_pem_end[] asm("_binary_privkey_pem_end");

    const unsigned char* cert = servercert_start;
    size_t cert_len = servercert_end - servercert_start;
    const unsigned char* pkey = prvtkey_pem_start;
    size_t pkey_len = prvtkey_pem_end - prvtkey_pem_start;

    JMS_HANDLE_ERR(
        TAG, jms_ws_start(microsite_request_handler, cert, cert_len, pkey, pkey_len));
}

/**
 * @brief Stops the HTTPS server when the device disconnects from the network.
 */
static void disconnect_handler(void* arg, esp_event_base_t event_base, int32_t event_id,
                               void* event_data)
{
    jms_err_t err = jms_ws_stop();
    if (err > 0)
    {
        ESP_LOGE(TAG, "Failed to stop HTTPS server, %u", (uint16_t)err);
    }

    ESP_ERROR_CHECK(example_connect());
}

/**
 * @brief Main entry point for the application.
 */
void app_main(void)
{
    ESP_LOGI(TAG, "Initializing system...");

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize the filesystem
    JMS_HANDLE_ERR(TAG, jms_fs_init());

    // Log available memory before caching
    ESP_LOGI(TAG, "Total free heap: %zu bytes", heap_caps_get_free_size(MALLOC_CAP_8BIT));
    ESP_LOGI(TAG, "Largest free block: %zu bytes",
             heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
    ESP_LOGI(TAG, "Total SPIRAM free heap: %zu bytes",
             heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    ESP_LOGI(TAG, "Largest SPIRAM free block: %zu bytes",
             heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM));

    // Initialize the cache
    jms_err_t cache_status = jms_cache_init(CACHE_MAX_SIZE);
    JMS_HANDLE_ERR(TAG, cache_status);

    // Register network event handlers
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                               &connect_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED,
                                               &disconnect_handler, NULL));

    // Connect to network
    ESP_ERROR_CHECK(example_connect());
}
