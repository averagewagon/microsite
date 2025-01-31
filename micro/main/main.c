/* Simple HTTP + SSL Server */

// FreeRTOS.h includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// C includes
#include <dirent.h>
#include <inttypes.h> // For PRIu32 macro
#include <stdio.h>

// ESP32 includes
#include <esp_event.h>
#include <esp_https_server.h>
#include <esp_littlefs.h>
#include <esp_log.h>
#include <esp_netif.h>
#include <esp_system.h>
#include <esp_tls.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <sys/param.h>

// Project includes
#include "protocol_examples_common.h"
#include "sdkconfig.h"

// hehe
#include "jms_error.h"
#include "jms_filesystem.h"
#include "jms_mime.h"
#include "jms_webserver.h"

// === Global Variables ===
static const char* TAG = "microsite";

#define MAX_URI_LENGTH 512

// === Filesystem Initialization ===
/**
 * Initializes the LittleFS filesystem for persistent storage and prepares it for use.
 */
void init_littlefs()
{
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

    // Check LittleFS partition info
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

    // Check if index.html exists
    const char* test_file = "/littlefs/index.html";
    FILE* f = fopen(test_file, "r");
    if (f)
    {
        ESP_LOGI(TAG, "Found test file: %s", test_file);
        fclose(f);
    }
    else
    {
        ESP_LOGW(TAG, "Test file not found: %s", test_file);
        ESP_LOGW(TAG, "Ensure the LittleFS image is flashed with the expected files.");
    }
}

void log_littlefs_contents()
{
    const char* dir_path = "/littlefs";
    DIR* dir = opendir(dir_path);
    if (dir == NULL)
    {
        ESP_LOGE(TAG, "Failed to open directory: %s", dir_path);
        return;
    }

    ESP_LOGI(TAG, "Contents of %s:", dir_path);
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        ESP_LOGI(TAG, "  %s", entry->d_name);
    }
    closedir(dir);
}

// === HTTP Handlers ===

/**
 * Helper function to determine MIME type based on file extension.
 * @param filename The name of the file.
 * @return A string representing the MIME type.
 */
static const char* get_mime_type(const char* filename)
{
    const char* ext = strrchr(filename, '.'); // Find the last dot
    if (!ext || ext == filename)
    {
        return "application/octet-stream"; // Default MIME type
    }

    // Match common file extensions
    if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0)
    {
        return "text/html";
    }
    else if (strcmp(ext, ".css") == 0)
    {
        return "text/css";
    }
    else if (strcmp(ext, ".js") == 0)
    {
        return "application/javascript";
    }
    else if (strcmp(ext, ".png") == 0)
    {
        return "image/png";
    }
    else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0)
    {
        return "image/jpeg";
    }
    else if (strcmp(ext, ".gif") == 0)
    {
        return "image/gif";
    }
    else if (strcmp(ext, ".svg") == 0)
    {
        return "image/svg+xml";
    }
    else if (strcmp(ext, ".json") == 0)
    {
        return "application/json";
    }
    else if (strcmp(ext, ".txt") == 0)
    {
        return "text/plain";
    }
    else if (strcmp(ext, ".ico") == 0)
    {
        return "image/x-icon";
    }
    else if (strcmp(ext, ".xml") == 0)
    {
        return "application/xml";
    }
    else if (strcmp(ext, ".woff") == 0 || strcmp(ext, ".woff2") == 0)
    {
        return "font/woff";
    }
    else if (strcmp(ext, ".ttf") == 0)
    {
        return "font/ttf";
    }
    else if (strcmp(ext, ".eot") == 0)
    {
        return "application/vnd.ms-fontobject";
    }
    else if (strcmp(ext, ".mp4") == 0)
    {
        return "video/mp4";
    }
    else if (strcmp(ext, ".webm") == 0)
    {
        return "video/webm";
    }
    else if (strcmp(ext, ".ogg") == 0)
    {
        return "audio/ogg";
    }
    else if (strcmp(ext, ".wav") == 0)
    {
        return "audio/wav";
    }
    else if (strcmp(ext, ".mp3") == 0)
    {
        return "audio/mpeg";
    }
    else if (strcmp(ext, ".pdf") == 0)
    {
        return "application/pdf";
    }
    else if (strcmp(ext, ".zip") == 0)
    {
        return "application/zip";
    }
    else if (strcmp(ext, ".tar") == 0 || strcmp(ext, ".gz") == 0 || strcmp(ext, ".tgz") == 0)
    {
        return "application/gzip";
    }

    return "application/octet-stream"; // Default for unknown types
}

/**
 * Handles incoming HTTP requests by serving files from LittleFS.
 * If no matching file is found, redirects to 404.html.
 * Automatically redirects subfolder root paths to index.html.
 * Strips query and fragment parts from the URI.
 * @param req The HTTP request object.
 * @return ESP_OK on success, ESP_FAIL on failure.
 */
static esp_err_t file_server_handler(httpd_req_t* req)
{
    char stripped_uri[MAX_URI_LENGTH];
    char filepath[MAX_URI_LENGTH + 20U];
    ESP_LOGI(TAG, "Incoming request for URI: %s", req->uri);

    // Strip query and fragment from the URI
    size_t len = strcspn(req->uri, "?#");
    snprintf(stripped_uri, sizeof(stripped_uri), "%.*s", (int)len, req->uri);
    ESP_LOGI(TAG, "Stripped URI: %s", stripped_uri);

    // Check for URI length
    if (strlen(stripped_uri) >= sizeof(filepath) - strlen("/littlefs"))
    {
        ESP_LOGE(TAG, "URI too long: %s", stripped_uri);
        httpd_resp_send_err(req, HTTPD_414_URI_TOO_LONG, "URI too long");
        return ESP_FAIL;
    }

    // Redirect root path ("/") and subfolder root paths ("/subfolder/") to index.html
    if (stripped_uri[strlen(stripped_uri) - 1] == '/')
    {
        snprintf(filepath, sizeof(filepath), "/littlefs%sindex.html", stripped_uri);
    }
    else
    {
        snprintf(filepath, sizeof(filepath), "/littlefs%s", stripped_uri);
    }

    ESP_LOGI(TAG, "Mapped URI to file path: %s", filepath);

    // Open the requested file
    FILE* f = fopen(filepath, "r");
    if (!f)
    {
        ESP_LOGW(TAG, "File not found: %s", filepath);

        // Redirect to 404.html
        snprintf(filepath, sizeof(filepath), "/littlefs/404.html");
        f = fopen(filepath, "r");
        if (!f)
        {
            ESP_LOGE(TAG, "404.html not found in LittleFS");
            httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "404 Not Found");
            return ESP_FAIL;
        }
    }

    // Determine MIME type using helper function
    const char* mime_type = get_mime_type(filepath);
    httpd_resp_set_type(req, mime_type);

    // Serve the file in chunks
    char buffer[256];
    size_t read_bytes;
    while ((read_bytes = fread(buffer, 1, sizeof(buffer), f)) > 0)
    {
        if (httpd_resp_send_chunk(req, buffer, read_bytes) != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to send chunk for file: %s", filepath);
            fclose(f);
            return ESP_FAIL;
        }
    }
    fclose(f);

    ESP_LOGI(TAG, "Served file: %s", filepath);

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

// === HTTPS Callback ===
/**
 * Callback function for SSL/TLS events.
 * Logs client certificate and connection details during SSL handshake.
 */
static void https_server_user_callback(esp_https_server_user_cb_arg_t* user_cb)
{
    ESP_LOGI(TAG, "User callback invoked!");
    mbedtls_ssl_context* ssl_ctx = NULL;

    switch (user_cb->user_cb_state)
    {
    case HTTPD_SSL_USER_CB_SESS_CREATE:
        ESP_LOGD(TAG, "At session creation");

        int sockfd = -1;
        esp_err_t esp_ret = esp_tls_get_conn_sockfd(user_cb->tls, &sockfd);
        if (esp_ret != ESP_OK)
        {
            ESP_LOGE(TAG, "Error obtaining socket FD");
            break;
        }
        ESP_LOGI(TAG, "Socket FD: %d", sockfd);

        ssl_ctx = (mbedtls_ssl_context*)esp_tls_get_ssl_context(user_cb->tls);
        if (ssl_ctx)
        {
            ESP_LOGI(TAG, "Current Ciphersuite: %s", mbedtls_ssl_get_ciphersuite(ssl_ctx));
        }
        break;

    case HTTPD_SSL_USER_CB_SESS_CLOSE:
        ESP_LOGD(TAG, "At session close");
        ssl_ctx = (mbedtls_ssl_context*)esp_tls_get_ssl_context(user_cb->tls);
        if (ssl_ctx)
        {
            const mbedtls_x509_crt* cert = mbedtls_ssl_get_peer_cert(ssl_ctx);
            if (cert)
            {
                char buf[1024];
                mbedtls_x509_crt_info(buf, sizeof(buf), "    ", cert);
                ESP_LOGI(TAG, "Peer certificate info:\n%s", buf);
            }
        }
        break;

    default:
        ESP_LOGE(TAG, "Illegal state!");
        break;
    }
}

// === HTTP Server Setup ===
/**
 * Configures and starts the HTTPS server with necessary SSL certificates and handlers.
 */
static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    ESP_LOGI(TAG, "Starting server");

    httpd_ssl_config_t conf = HTTPD_SSL_CONFIG_DEFAULT();
    conf.httpd.uri_match_fn = httpd_uri_match_wildcard; // Enables wildcard URI handling
    conf.httpd.max_uri_handlers = 1;                    // Only need the root wildcard handler

    // Set up SSL certificate
    extern const unsigned char servercert_start[] asm("_binary_fullchain_pem_start");
    extern const unsigned char servercert_end[] asm("_binary_fullchain_pem_end");
    conf.servercert = servercert_start;
    conf.servercert_len = servercert_end - servercert_start;

    extern const unsigned char prvtkey_pem_start[] asm("_binary_privkey_pem_start");
    extern const unsigned char prvtkey_pem_end[] asm("_binary_privkey_pem_end");
    conf.prvtkey_pem = prvtkey_pem_start;
    conf.prvtkey_len = prvtkey_pem_end - prvtkey_pem_start;

    conf.user_cb = https_server_user_callback;

    // Start the server
    esp_err_t ret = httpd_ssl_start(&server, &conf);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error starting server!");
        return NULL;
    }

    // Register the wildcard andler
    static const httpd_uri_t root_handler = {
        .uri = "*",
        .method = HTTP_GET,
        .handler = file_server_handler,
    };
    ESP_LOGI(TAG, "Registering wildcard URI handler");
    if (httpd_register_uri_handler(server, &root_handler) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register wildcard URI handler");
    }

    return server;
}

/**
 * Stops the HTTPS server and cleans up resources.
 */
static esp_err_t stop_webserver(httpd_handle_t server)
{
    return httpd_ssl_stop(server);
}

// === Event Handlers ===
/**
 * Starts the web server when the device successfully connects to a network.
 */
static void connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id,
                            void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*)arg;
    if (*server == NULL)
    {
        *server = start_webserver();
    }
}

/**
 * Stops the web server when the device disconnects from the network.
 */
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
            ESP_LOGE(TAG, "Failed to stop HTTPS server");
        }
    }
}

// === Application Main ===
/**
 * Main entry point for the application, initializing core subsystems, the filesystem, and setting
 * up event-driven web server management.
 */
void app_main(void)
{
    // TODO: Why is this null thing here? Is it necessary?
    static httpd_handle_t server = NULL;

    jms_ws_stop();

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    init_littlefs();
    log_littlefs_contents();

    ESP_ERROR_CHECK(
        esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED,
                                               &disconnect_handler, &server));

    ESP_ERROR_CHECK(example_connect());
}
