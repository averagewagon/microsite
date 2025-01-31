#include "jms_webserver.h"
#include <esp_https_server.h>
#include <esp_log.h>
#include <string.h>

#define TAG "JMS_WS"

static jms_ws_handler_t request_handler = NULL;
static httpd_handle_t server_handle = NULL;

/**
 * @brief Parses an HTTP request from `httpd_req_t` into `jms_ws_request_t`.
 */
static void jms_ws_parse_request(httpd_req_t* req, jms_ws_request_t* out_request)
{
    out_request->method = http_method_str(req->method);
    out_request->path = req->uri;
    out_request->internal_req = req; // Store opaque pointer

    // Locate query string in URI (if present)
    char* query_start = strchr(req->uri, '?');
    if (query_start)
    {
        out_request->query = query_start + 1;
        out_request->query_length = strcspn(out_request->query, "#");
    }
    else
    {
        out_request->query = NULL;
        out_request->query_length = 0;
    }

    // Headers (Use fixed-size buffers)
    if (httpd_req_get_hdr_value_str(req, "Accept", out_request->accept,
                                    sizeof(out_request->accept)) != ESP_OK)
    {
        out_request->accept[0] = '\0';
    }

    if (httpd_req_get_hdr_value_str(req, "Accept-Encoding", out_request->accept_encoding,
                                    sizeof(out_request->accept_encoding)) != ESP_OK)
    {
        out_request->accept_encoding[0] = '\0';
    }
}

/**
 * @brief HTTP request handler for all routes.
 */
static esp_err_t jms_ws_request_handler(httpd_req_t* req)
{
    if (!request_handler)
    {
        ESP_LOGE(TAG, "No request handler set.");
        return ESP_FAIL;
    }

    // Parse the request
    jms_ws_request_t request;
    jms_ws_parse_request(req, &request);

    // Call the user-provided handler
    return request_handler(&request);
}

/**
 * @brief Sends an HTTP response.
 */
jms_err_t jms_ws_send_response(const jms_ws_request_t* request, const char* status,
                               const char* content_type, const char* content_encoding,
                               const char* cache_control, const char* content,
                               size_t content_length)
{
    if (!request || !request->internal_req || !status || !content_type)
    {
        return JMS_ERR_INVALID_ARG;
    }

    httpd_req_t* req = (httpd_req_t*)request->internal_req;

    if (httpd_resp_set_status(req, status) != ESP_OK)
    {
        return JMS_ERR_INVALID_ARG;
    }

    if (httpd_resp_set_type(req, content_type) != ESP_OK)
    {
        return JMS_ERR_INVALID_ARG;
    }

    if (content_encoding && httpd_resp_set_hdr(req, "Content-Encoding", content_encoding) != ESP_OK)
    {
        return JMS_ERR_INVALID_ARG;
    }

    if (cache_control && httpd_resp_set_hdr(req, "Cache-Control", cache_control) != ESP_OK)
    {
        return JMS_ERR_INVALID_ARG;
    }

    // Send response body
    if (content && content_length > 0)
    {
        if (httpd_resp_send(req, content, content_length) != ESP_OK)
        {
            return JMS_ERR_INVALID_ARG;
        }
    }
    else
    {
        if (httpd_resp_send(req, NULL, 0) != ESP_OK)
        {
            return JMS_ERR_INVALID_ARG;
        }
    }

    return JMS_OK;
}

/**
 * @brief Starts the HTTPS server with SSL certificates.
 */
jms_err_t jms_ws_start(jms_ws_handler_t handler)
{
    if (!handler)
    {
        ESP_LOGE(TAG, "Request handler must be provided.");
        return JMS_ERR_INVALID_ARG;
    }

    request_handler = handler;

    httpd_ssl_config_t conf = HTTPD_SSL_CONFIG_DEFAULT();
    conf.httpd.uri_match_fn = httpd_uri_match_wildcard;
    conf.httpd.max_uri_handlers = 1;

    // Load SSL certificates
    extern const unsigned char servercert_start[] asm("_binary_fullchain_pem_start");
    extern const unsigned char servercert_end[] asm("_binary_fullchain_pem_end");
    conf.servercert = servercert_start;
    conf.servercert_len = servercert_end - servercert_start;

    extern const unsigned char prvtkey_pem_start[] asm("_binary_privkey_pem_start");
    extern const unsigned char prvtkey_pem_end[] asm("_binary_privkey_pem_end");
    conf.prvtkey_pem = prvtkey_pem_start;
    conf.prvtkey_len = prvtkey_pem_end - prvtkey_pem_start;

    if (httpd_ssl_start(&server_handle, &conf) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start HTTPS server.");
        return JMS_ERR_WS_INIT_FAILED;
    }

    // Register the wildcard handler
    static const httpd_uri_t uri_handler = {
        .uri = "*",
        .method = HTTP_GET,
        .handler = jms_ws_request_handler,
    };

    if (httpd_register_uri_handler(server_handle, &uri_handler) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register URI handler.");
        httpd_ssl_stop(server_handle);
        return JMS_ERR_WS_INIT_FAILED;
    }

    ESP_LOGI(TAG, "HTTPS server started.");
    return JMS_OK;
}

/**
 * @brief Stops the HTTPS server.
 */
jms_err_t jms_ws_stop(void)
{
    if (server_handle)
    {
        httpd_ssl_stop(server_handle);
        server_handle = NULL;
        ESP_LOGI(TAG, "Web server stopped.");
        return JMS_OK;
    }
    return JMS_ERR_WS_NOT_INITIALIZED;
}
