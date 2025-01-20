#ifndef JMS_MIME_H
#define JMS_MIME_H

#include <stddef.h>
#include <stdint.h>

#include "jms_error.h"

/**
 * @brief Determines the MIME type based on the file extension.
 *
 * @param filename      The name of the file, including its extension.
 * @param out_mime_type A buffer to store the MIME type.
 * @param out_mime_size The size of the output buffer.
 * @return JMS_OK on success, an error code on failure.
 */
jms_err_t jms_mime_getType(const char* filename, char* out_mime_type, size_t out_mime_size);

#endif // JMS_MIME_H
