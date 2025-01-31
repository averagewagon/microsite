#ifndef JMS_WEBSERVER_H
#define JMS_WEBSERVER_H

#include "jms_error.h"
#include <stdint.h>

#define JMS_WS_ACCEPT_SIZE   64
#define JMS_WS_ENCODING_SIZE 32

/**
 * @brief Struct representing an incoming HTTP request.
 */
typedef struct
{
    const char* method;
    const char* path;
    const char* query;
    size_t query_length;
    char accept[JMS_WS_ACCEPT_SIZE];
    char accept_encoding[JMS_WS_ENCODING_SIZE];
    void* internal_req; // Opaque pointer to `httpd_req_t`
} jms_ws_request_t;

/**
 * @brief Function pointer type for handling HTTP requests.
 *
 * @param request Parsed HTTP request details.
 * @return JMS_OK on success, an error code on failure.
 */
typedef jms_err_t (*jms_ws_handler_t)(const jms_ws_request_t* request);

/**
 * @brief Sends an HTTP response.
 *
 * @param request The request object containing the internal reference.
 * @param status The full status line (e.g., "200 OK", "404 Not Found").
 * @param content_type The MIME type (e.g., "text/html").
 * @param content_encoding The content encoding (e.g., "br", NULL if none).
 * @param cache_control The cache control policy (e.g., "max-age=86400").
 * @param content The response body.
 * @param content_length The length of the response body.
 * @return JMS_OK on success, `JMS_ERR_INVALID_ARG` on failure.
 */
jms_err_t jms_ws_send_response(const jms_ws_request_t* request, const char* status,
                               const char* content_type, const char* content_encoding,
                               const char* cache_control, const char* content,
                               size_t content_length);

/**
 * @brief Starts the web server.
 *
 * @param handler The function to handle HTTP requests.
 * @return JMS_OK on success, `JMS_ERR_INVALID_ARG` if handler is NULL.
 */
jms_err_t jms_ws_start(jms_ws_handler_t handler);

/**
 * @brief Stops the web server.
 *
 * @return JMS_OK on success, `JMS_ERR_WS_NOT_INITIALIZED` if the server was not running.
 */
jms_err_t jms_ws_stop(void);

#endif // JMS_WEBSERVER_H
