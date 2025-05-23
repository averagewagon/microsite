#ifndef JMS_WEBSERVER_H
#define JMS_WEBSERVER_H

#include "jms_error.h"
#include <stdint.h>

#define JMS_WS_ENCODING_SIZE 64

/**
 * @brief Struct representing an incoming HTTP request.
 */
typedef struct
{
    const char* method;
    const char* path;
    const char* query;
    size_t query_length;
    char accept_encoding[JMS_WS_ENCODING_SIZE];
    void* internal_req;
} jms_ws_request_t;

/**
 * @brief Struct representing a single HTTP header which should be set.
 */
typedef struct
{
    const char* key;
    const char* value;
} jms_header_t;

/**
 * @brief Function pointer type for handling HTTP requests.
 *
 * @param request Parsed HTTP request details.
 * @return JMS_OK on success, an error code on failure.
 */
typedef jms_err_t (*jms_ws_handler_t)(const jms_ws_request_t* request);

/**
 * @brief Starts the HTTPS server with optional SSL certificates.
 *
 * @param handler The function to handle HTTP requests.
 * @param cert Pointer to the SSL certificate (PEM format), or NULL for HTTP-only.
 * @param cert_len Length of the certificate.
 * @param pkey Pointer to the private key (PEM format), or NULL for HTTP-only.
 * @param pkey_len Length of the private key.
 * @return JMS_OK on success, `JMS_ERR_INVALID_ARG` if handler is NULL.
 */
jms_err_t jms_ws_start(jms_ws_handler_t handler, const unsigned char* cert,
                       size_t cert_len, const unsigned char* pkey, size_t pkey_len);

/**
 * @brief Stops the web server.
 *
 * @return JMS_OK on success, `JMS_ERR_WS_NOT_INITIALIZED` if the server was not running.
 */
jms_err_t jms_ws_stop(void);

/**
 * @brief Sets the status line for the response.
 *
 * @param request The incoming request.
 * @param status The full status line (e.g., "200 OK").
 * @return JMS_OK on success, JMS_ERR_INVALID_ARG if parameters are invalid.
 */
jms_err_t jms_ws_set_response_status(const jms_ws_request_t* request, const char* status);

/**
 * @brief Sets the content type for the response.
 *
 * @param request The incoming request.
 * @param content_type The MIME type (e.g., "text/html").
 * @return JMS_OK on success, JMS_ERR_INVALID_ARG if parameters are invalid.
 */
jms_err_t jms_ws_set_response_content_type(const jms_ws_request_t* request,
                                           const char* content_type);

/**
 * @brief Sets a custom header for the response.
 *
 * @param request The incoming request.
 * @param header The header name.
 * @param value The header value.
 * @return JMS_OK on success, JMS_ERR_INVALID_ARG if parameters are invalid.
 */
jms_err_t jms_ws_set_response_header(const jms_ws_request_t* request, const char* header,
                                     const char* value);

/**
 * @brief Sends the entire response body in one call.
 *
 * @param request The incoming request.
 * @param content The response body.
 * @param content_length The length of the response body.
 * @return JMS_OK on success, JMS_ERR_INVALID_ARG on failure.
 */
jms_err_t jms_ws_response_send(const jms_ws_request_t* request, const char* content,
                               size_t content_length);

/**
 * @brief Sends a chunk of response data. Send NULL and 0 to signal the end.
 *
 * @param request The incoming request.
 * @param content A pointer to the chunk data.
 * @param content_length The size of the chunk.
 * @return JMS_OK on success, JMS_ERR_INVALID_ARG on failure.
 */
jms_err_t jms_ws_response_send_chunk(const jms_ws_request_t* request, const char* content,
                                     size_t content_length);

#endif // JMS_WEBSERVER_H
