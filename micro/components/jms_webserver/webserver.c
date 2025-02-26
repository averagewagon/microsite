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
    out_request->internal_req = req;

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

    jms_ws_request_t request;
    jms_ws_parse_request(req, &request);
    return request_handler(&request);
}

/**
 * @brief Starts the web server with optional SSL certificates.
 */
jms_err_t jms_ws_start(jms_ws_handler_t handler, const unsigned char* cert,
                       size_t cert_len, const unsigned char* pkey, size_t pkey_len)
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

    // Set up SSL only if cert and key are provided
    if (cert && cert_len > 0 && pkey && pkey_len > 0)
    {
        conf.servercert = cert;
        conf.servercert_len = cert_len;
        conf.prvtkey_pem = pkey;
        conf.prvtkey_len = pkey_len;
        ESP_LOGI(TAG, "Starting HTTPS server with SSL certificates.");
    }
    else
    {
        ESP_LOGE(TAG, "No SSL certificate provided");
    }

    if (httpd_ssl_start(&server_handle, &conf) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start HTTPS server.");
        return JMS_ERR_WS_INIT_FAILED;
    }

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

    ESP_LOGI(TAG, "Web server started.");
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

/**
 * @brief Sets the status line for the response.
 */
jms_err_t jms_ws_set_response_status(const jms_ws_request_t* request, const char* status)
{
    if (!request || !request->internal_req || !status)
    {
        return JMS_ERR_INVALID_ARG;
    }

    httpd_req_t* req = (httpd_req_t*)request->internal_req;
    if (httpd_resp_set_status(req, status) != ESP_OK)
    {
        return JMS_ERR_INVALID_ARG;
    }

    return JMS_OK;
}

/**
 * @brief Sets the content type for the response.
 */
jms_err_t jms_ws_set_response_content_type(const jms_ws_request_t* request,
                                           const char* content_type)
{
    if (!request || !request->internal_req || !content_type)
    {
        return JMS_ERR_INVALID_ARG;
    }

    httpd_req_t* req = (httpd_req_t*)request->internal_req;
    if (httpd_resp_set_type(req, content_type) != ESP_OK)
    {
        return JMS_ERR_INVALID_ARG;
    }

    return JMS_OK;
}

/**
 * @brief Sets a custom header for the response.
 */
jms_err_t jms_ws_set_response_header(const jms_ws_request_t* request, const char* header,
                                     const char* value)
{
    if (!request || !request->internal_req || !header || !value)
    {
        return JMS_ERR_INVALID_ARG;
    }

    httpd_req_t* req = (httpd_req_t*)request->internal_req;
    if (httpd_resp_set_hdr(req, header, value) != ESP_OK)
    {
        return JMS_ERR_INVALID_ARG;
    }

    return JMS_OK;
}

/**
 * @brief Sends the entire response body in one call.
 */
jms_err_t jms_ws_response_send(const jms_ws_request_t* request, const char* content,
                               size_t content_length)
{
    if (!request || !request->internal_req || (content_length > 0 && !content))
    {
        return JMS_ERR_INVALID_ARG;
    }

    httpd_req_t* req = (httpd_req_t*)request->internal_req;

    if (httpd_resp_send(req, content, content_length) != ESP_OK)
    {
        return JMS_ERR_INVALID_ARG;
    }

    return JMS_OK;
}

/**
 * @brief Sends a chunk of response data. Send NULL and 0 to signal the end.
 */
jms_err_t jms_ws_response_send_chunk(const jms_ws_request_t* request, const char* content,
                                     size_t content_length)
{
    if (!request || !request->internal_req)
    {
        return JMS_ERR_INVALID_ARG;
    }

    httpd_req_t* req = (httpd_req_t*)request->internal_req;

    if (httpd_resp_send_chunk(req, content, content_length) != ESP_OK)
    {
        return JMS_ERR_INVALID_ARG;
    }

    return JMS_OK;
}