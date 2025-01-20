#include <stdio.h>
#include <string.h>

#include "jms_error.h"
#include "jms_mime.h"
#include "priv_mime_hash.h"

jms_err_t jms_mime_getType(const char* filename, char* out_mime_type, size_t out_mime_size)
{
    // Validate inputs
    if (filename == NULL || out_mime_type == NULL || out_mime_size == 0)
    {
        return JMS_ERR_INVALID_ARG;
    }

    // Find the last occurrence of '.' using strrchr
    const char* ext = strrchr(filename, '.');
    if (ext == NULL || *(ext + 1) == '\0')
    { // No extension or trailing '.'
        return JMS_ERR_MIME_INVALID;
    }
    ext++; // Skip the '.' character

    // Use gperf-generated function to find the MIME type
    const char* mime_type = in_word_set(ext, strlen(ext));
    if (mime_type)
    {
        size_t mime_len = strlen(mime_type);
        if (mime_len >= out_mime_size)
        {
            return JMS_ERR_INVALID_ARG; // Buffer too small
        }
        strncpy(out_mime_type, mime_type, out_mime_size);
        out_mime_type[out_mime_size - 1] = '\0'; // Ensure null-termination
        return JMS_OK;
    }

    // MIME type not found
    return JMS_ERR_MIME_NOT_FOUND;
}
