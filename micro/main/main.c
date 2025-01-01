/* Simple HTTP + SSL Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "freertos/FreeRTOS.h"

#include "esp_netif.h"
#include "esp_tls.h"
#include "freertos/task.h"
#include "protocol_examples_common.h"
#include "sdkconfig.h"

#include <esp_event.h>
#include <esp_https_server.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <inttypes.h> // For PRIu32 macro
#include <nvs_flash.h>
#include <stdio.h>
#include <sys/param.h>

#include "esp_littlefs.h"

/* A simple example that demonstrates how to create GET and POST
 * handlers and start an HTTPS server.
 */

static const char* TAG = "microsite";

void init_littlefs()
{
    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/littlefs",
        .partition_label = "littlefs",
        .format_if_mount_failed = true,
        .dont_mount = false,
    };

    esp_err_t ret = esp_vfs_littlefs_register(&conf);
    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE(TAG, "Failed to find LittleFS partition");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize LittleFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_littlefs_info(conf.partition_label, &total, &used);
    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "LittleFS Partition: Total = %zu bytes, Used = %zu bytes", total, used);
    }
    else
    {
        ESP_LOGE(TAG, "Failed to get LittleFS partition info (%s)", esp_err_to_name(ret));
    }
}

void test_littlefs()
{
    // Write a test file
    FILE* f = fopen("/littlefs/hello.txt", "w");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    fprintf(f, "Hello LittleFS!\n");
    fclose(f);
    ESP_LOGI(TAG, "File written");

    // Read the test file
    f = fopen("/littlefs/hello.txt", "r");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }
    char buffer[64];
    fgets(buffer, sizeof(buffer), f);
    fclose(f);
    ESP_LOGI(TAG, "Read from file: '%s'", buffer);
}

/* Event handler for catching system events */
static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id,
                          void* event_data)
{
    if (event_base == ESP_HTTPS_SERVER_EVENT)
    {
        if (event_id == HTTPS_SERVER_EVENT_ERROR)
        {
            esp_https_server_last_error_t* last_error = (esp_tls_last_error_t*)event_data;
            ESP_LOGE(TAG,
                     "Error event triggered: last_error = %s, last_tls_err = %d, "
                     "tls_flag = %d",
                     esp_err_to_name(last_error->last_error), last_error->esp_tls_error_code,
                     last_error->esp_tls_flags);
        }
    }
}

const char* htmlContent =
    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "<head>\n"
    "    <meta charset=\"UTF-8\">\n"
    "    <meta name=\"viewport\" content=\"width=device-width, "
    "initial-scale=1.0\">\n"
    "    <title>Example Website</title>\n"
    "</head>\n"
    "<body style=\"font-family: Arial, sans-serif; line-height: 1.6; margin: "
    "0; padding: 0;\">\n"
    "    <header style=\"background-color: #4CAF50; color: white; padding: "
    "20px; text-align: center;\">\n"
    "        <h1>This Website is Hosted on an ESP32</h1>\n"
    "    </header>\n"
    "    <main style=\"padding: 20px;\">\n"
    "        <p style=\"font-size: 18px; color: #333;\">This is an example "
    "webpage with basic content. It includes a header with a welcoming message "
    "and this paragraph with some additional information. Inline CSS is used "
    "to style the elements and ensure a visually appealing presentation.</p>\n"
    "    </main>\n"
    "    <footer style=\"background-color: #f1f1f1; text-align: center; "
    "padding: 10px;\">\n"
    "        <p style=\"font-size: 14px; color: #777;\">&copy; 2024 Example "
    "Website. All rights reserved.</p>\n"
    "    </footer>\n"
    "</body>\n"
    "</html>";

/* An HTTP GET handler */
static esp_err_t root_get_handler(httpd_req_t* req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, htmlContent, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static void print_peer_cert_info(const mbedtls_ssl_context* ssl)
{
    const mbedtls_x509_crt* cert;
    const size_t buf_size = 1024;
    char* buf = calloc(buf_size, sizeof(char));
    if (buf == NULL)
    {
        ESP_LOGE(TAG, "Out of memory - Callback execution failed!");
        return;
    }

    // Logging the peer certificate info
    cert = mbedtls_ssl_get_peer_cert(ssl);
    if (cert != NULL)
    {
        mbedtls_x509_crt_info((char*)buf, buf_size - 1, "    ", cert);
        ESP_LOGI(TAG, "Peer certificate info:\n%s", buf);
    }
    else
    {
        ESP_LOGW(TAG, "Could not obtain the peer certificate!");
    }

    free(buf);
}

/**
 * Example callback function to get the certificate of connected clients,
 * whenever a new SSL connection is created and closed
 *
 * Can also be used to other information like Socket FD, Connection state, etc.
 *
 * NOTE: This callback will not be able to obtain the client certificate if the
 * following config `Set minimum Certificate Verification mode to Optional` is
 * not enabled (enabled by default in this example).
 *
 * The config option is found here - Component config → ESP-TLS
 *
 */
static void https_server_user_callback(esp_https_server_user_cb_arg_t* user_cb)
{
    ESP_LOGI(TAG, "User callback invoked!");
    mbedtls_ssl_context* ssl_ctx = NULL;
    switch (user_cb->user_cb_state)
    {
    case HTTPD_SSL_USER_CB_SESS_CREATE:
        ESP_LOGD(TAG, "At session creation");

        // Logging the socket FD
        int sockfd = -1;
        esp_err_t esp_ret;
        esp_ret = esp_tls_get_conn_sockfd(user_cb->tls, &sockfd);
        if (esp_ret != ESP_OK)
        {
            ESP_LOGE(TAG, "Error in obtaining the sockfd from tls context");
            break;
        }
        ESP_LOGI(TAG, "Socket FD: %d", sockfd);
        ssl_ctx = (mbedtls_ssl_context*)esp_tls_get_ssl_context(user_cb->tls);
        if (ssl_ctx == NULL)
        {
            ESP_LOGE(TAG, "Error in obtaining ssl context");
            break;
        }
        // Logging the current ciphersuite
        ESP_LOGI(TAG, "Current Ciphersuite: %s", mbedtls_ssl_get_ciphersuite(ssl_ctx));
        break;

    case HTTPD_SSL_USER_CB_SESS_CLOSE:
        ESP_LOGD(TAG, "At session close");
        // Logging the peer certificate
        ssl_ctx = (mbedtls_ssl_context*)esp_tls_get_ssl_context(user_cb->tls);
        if (ssl_ctx == NULL)
        {
            ESP_LOGE(TAG, "Error in obtaining ssl context");
            break;
        }
        print_peer_cert_info(ssl_ctx);
        break;
    default:
        ESP_LOGE(TAG, "Illegal state!");
        return;
    }
}

static const httpd_uri_t root = {.uri = "/", .method = HTTP_GET, .handler = root_get_handler};

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server");

    httpd_ssl_config_t conf = HTTPD_SSL_CONFIG_DEFAULT();

    extern const unsigned char servercert_start[] asm("_binary_fullchain_pem_start");
    extern const unsigned char servercert_end[] asm("_binary_fullchain_pem_end");
    conf.servercert = servercert_start;
    conf.servercert_len = servercert_end - servercert_start;

    extern const unsigned char prvtkey_pem_start[] asm("_binary_privkey_pem_start");
    extern const unsigned char prvtkey_pem_end[] asm("_binary_privkey_pem_end");
    conf.prvtkey_pem = prvtkey_pem_start;
    conf.prvtkey_len = prvtkey_pem_end - prvtkey_pem_start;

    conf.user_cb = https_server_user_callback;
    esp_err_t ret = httpd_ssl_start(&server, &conf);
    if (ESP_OK != ret)
    {
        ESP_LOGI(TAG, "Error starting server!");
        return NULL;
    }

    // Set URI handlers
    ESP_LOGI(TAG, "Registering URI handlers");
    httpd_register_uri_handler(server, &root);
    return server;
}

static esp_err_t stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    return httpd_ssl_stop(server);
}

static void disconnect_handler(void* arg, esp_event_base_t event_base, int32_t event_id,
                               void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*)arg;
    if (*server)
    {
        if (stop_webserver(*server) == ESP_OK)
        {
            *server = NULL;
        }
        else
        {
            ESP_LOGE(TAG, "Failed to stop https server");
        }
    }
}

static void connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id,
                            void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*)arg;
    if (*server == NULL)
    {
        *server = start_webserver();
    }
}

void app_main(void)
{
    static httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* Initialize LittleFS */
    init_littlefs();
    test_littlefs();

    /* Register event handlers to start server when Wi-Fi or Ethernet is
     * connected, and stop server when disconnection happens.
     */

    ESP_ERROR_CHECK(
        esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED,
                                               &disconnect_handler, &server));
    ESP_ERROR_CHECK(
        esp_event_handler_register(ESP_HTTPS_SERVER_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));

    /* This helper function configures Wi-Fi or Ethernet, as selected in
     * menuconfig. Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());
}
