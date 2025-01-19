#include "jms/mime/mime.h"
#include "jms/mime/priv_mime_hash.h"
#include <stdio.h>
#include <string.h>

static const char* find_last_dot(const char* str, size_t max_len)
{
    const char* last_dot = NULL;
    for (size_t i = 0; i < max_len && str[i] != '\0'; ++i)
    {
        if (str[i] == '.')
        {
            last_dot = &str[i];
        }
    }
    return last_dot;
}

jms_err_t jms_mime_getType(const char* filename, char* out_mime_type, size_t out_mime_size)
{
    // Validate inputs
    if (filename == NULL || out_mime_type == NULL || out_mime_size == 0)
    {
        return JMS_ERR_INVALID_ARG;
    }

    // Ensure the filename is null-terminated within a reasonable length
    size_t filename_len = strnlen(filename, 256); // Limit to 256 chars
    if (filename_len == 256)
    {
        return JMS_ERR_INVALID_ARG; // Not null-terminated within bounds
    }

    // Find the last occurrence of '.' in the filename
    const char* ext = find_last_dot(filename, filename_len);
    if (ext == NULL || *(ext + 1) == '\0')
    { // No extension or trailing '.'
        return JMS_ERR_MIME_INVALID;
    }
    ext++; // Skip the '.' character

    // Use gperf-generated function to find the MIME type
    const char* mime_type = in_word_set(ext, strnlen(ext, filename_len - (ext - filename)));
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
