#ifndef JMS_MIME_H
#define JMS_MIME_H

#include <stddef.h>
#include <stdint.h>

#include "jms_error.h"

/**
 * @brief Determines the MIME type based on the file extension.
 *
 * @param filename The name of the file, including its extension.
 * @param out_mime_type Pointer to store the MIME type string.
 * @return JMS_OK on success, an error code on failure.
 */
jms_err_t jms_mime_get_type(const char* filename, const char** out_mime_type);

#endif // JMS_MIME_H
