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

#include "jms_error.h"
#include "jms_filesystem.h"
#include "jms_mime.h"
#include "jms_webserver.h"

static const char* TAG = "microsite";

static jms_err_t microsite_request_handler(const jms_ws_request_t* request)
{
    char filepath[256];
    jms_fs_handle_t file_handle;
    char buffer[512];
    size_t bytes_read = 0;

    ESP_LOGI(TAG, "Incoming request: %s", request->path);

    // Resolve file path
    if (strcmp(request->path, "/") == 0)
    {
        snprintf(filepath, sizeof(filepath), "/littlefs/index.html");
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

    // Open file
    jms_err_t fs_result = jms_fs_open(filepath, &file_handle);
    if (fs_result != JMS_OK)
    {
        ESP_LOGW(TAG, "File not found: %s", filepath);
        jms_ws_set_response_headers(request, "404 Not Found", "text/html", NULL, "max-age=60");
        return jms_ws_response_send(request, "<h1>404 Not Found</h1>", 22);
    }

    // Determine MIME type
    char mime_type[64];
    if (jms_mime_get_type(filepath, mime_type, sizeof(mime_type)) != JMS_OK)
    {
        strncpy(mime_type, "application/octet-stream", sizeof(mime_type));
    }

    // Set headers
    jms_ws_set_response_headers(request, "200 OK", mime_type, NULL, "max-age=86400");

    // Send file in chunks
    while (jms_fs_read_chunk(&file_handle, buffer, sizeof(buffer), &bytes_read) == JMS_OK &&
           bytes_read > 0)
    {
        jms_ws_response_send_chunk(request, buffer, bytes_read);
    }

    // End chunked response
    jms_ws_response_send_chunk(request, NULL, 0);
    jms_fs_close(&file_handle);
    return JMS_OK;
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

    JMS_HANDLE_ERR(TAG, jms_ws_start(microsite_request_handler, cert, cert_len, pkey, pkey_len));
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

    JMS_HANDLE_ERR(TAG, jms_fs_init());

    ESP_ERROR_CHECK(
        esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED,
                                               &disconnect_handler, NULL));

    ESP_ERROR_CHECK(example_connect());
}
