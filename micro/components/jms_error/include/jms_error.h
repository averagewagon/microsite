#ifndef JMS_ERROR_H
#define JMS_ERROR_H

#include <stdint.h>

#include <esp_log.h>

// Define the error type as a uint16_t
typedef uint16_t jms_err_t;

#define JMS_OK                 ((jms_err_t)(0))
#define JMS_ERR_UNKNOWN        ((jms_err_t)(0x1001))
#define JMS_ERR_INVALID_ARG    ((jms_err_t)(0x1002))
#define JMS_ERR_MIME_NOT_FOUND ((jms_err_t)(0x5001))
#define JMS_ERR_MIME_INVALID   ((jms_err_t)(0x5002))

// Macro for handling and logging errors
#define JMS_HANDLE_ERR(tag, jms_err_code)                                                          \
    do                                                                                             \
    {                                                                                              \
        if ((jms_err_code) > JMS_OK)                                                               \
        {                                                                                          \
            ESP_LOGE(tag, "JMS Error Code: 0x%04X | File: %s | Line: %d", (jms_err_code),          \
                     __FILE__, __LINE__);                                                          \
            assert((jms_err_code) == JMS_OK);                                                      \
        }                                                                                          \
    } while (0)

#endif // JMS_ERROR_H
