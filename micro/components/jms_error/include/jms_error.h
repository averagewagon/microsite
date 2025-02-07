#ifndef JMS_ERROR_H
#define JMS_ERROR_H

#include <assert.h>
#include <esp_log.h>
#include <stdint.h>

// Define the error type as a uint16_t
typedef uint16_t jms_err_t;

#define JMS_OK              ((jms_err_t)(0))
#define JMS_ERR_UNKNOWN     ((jms_err_t)(1001))
#define JMS_ERR_INVALID_ARG ((jms_err_t)(1002))

// Webserver Errors
#define JMS_ERR_WS_NOT_INITIALIZED ((jms_err_t)(2001))
#define JMS_ERR_WS_SEND_FAILED     ((jms_err_t)(2002))
#define JMS_ERR_WS_INIT_FAILED     ((jms_err_t)(2003))

// Filesystem Errors
#define JMS_ERR_FS_NOT_INITIALIZED ((jms_err_t)(3001))
#define JMS_ERR_FS_MOUNT_FAILED    ((jms_err_t)(3002))
#define JMS_ERR_FS_FILE_NOT_FOUND  ((jms_err_t)(3003))
#define JMS_ERR_FS_READ_FAILED     ((jms_err_t)(3004))
#define JMS_ERR_FS_INVALID_PATH    ((jms_err_t)(3005))
#define JMS_ERR_FS_IS_DIRECTORY    ((jms_err_t)(3006))

// MIME Errors
#define JMS_ERR_MIME_NOT_FOUND ((jms_err_t)(5001))
#define JMS_ERR_MIME_INVALID   ((jms_err_t)(5002))

// Cache Errors
#define JMS_ERR_CACHE_MISS            ((jms_err_t)(6001))
#define JMS_ERR_CACHE_FULL            ((jms_err_t)(6002))
#define JMS_ERR_CACHE_ALLOC           ((jms_err_t)(6003))
#define JMS_ERR_CACHE_NOT_INITIALIZED ((jms_err_t)(6004))
#define JMS_ERR_CACHE_FILE_NOT_FOUND  ((jms_err_t)(6005))
#define JMS_ERR_CACHE_FILE_READ       ((jms_err_t)(6006))
#define JMS_ERR_CACHE_OVERSIZED_ENTRY ((jms_err_t)(6007))

// Macro for handling and logging errors
#define JMS_HANDLE_ERR(tag, jms_err_code)                                                          \
    do                                                                                             \
    {                                                                                              \
        if ((jms_err_code) > JMS_OK)                                                               \
        {                                                                                          \
            ESP_LOGE(tag, "JMS Error Code: %d | File: %s | Line: %d", (jms_err_code), __FILE__,    \
                     __LINE__);                                                                    \
            assert((jms_err_code) == JMS_OK);                                                      \
        }                                                                                          \
    } while (0)

#endif // JMS_ERROR_H
