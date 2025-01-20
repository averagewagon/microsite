#include <esp_log.h>
#include <stdio.h>
#include <string.h>

#include "jms_error.h"
#include "jms_mime.h"

#include "mime_gperf_output.c"

static const char* TAG = "JMS_MIME";

jms_err_t jms_mime_getType(const char* filename, char* out_mime_type, size_t out_mime_size)
{
    if (filename == NULL || out_mime_type == NULL || out_mime_size == 0)
    {
        return JMS_ERR_INVALID_ARG;
    }

    const char* ext = strrchr(filename, '.');
    if (ext == NULL || *(ext + 1) == '\0')
    {
        return JMS_ERR_MIME_INVALID;
    }
    ext++; // Skip the '.' character

    const struct mime_entry* entry = in_word_set(ext, strlen(ext));
    if (entry)
    {
        size_t mime_len = strlen(entry->mime_type);
        if (mime_len >= out_mime_size)
        {
            return JMS_ERR_INVALID_ARG;
        }
        strncpy(out_mime_type, entry->mime_type, out_mime_size);
        out_mime_type[out_mime_size - 1] = '\0';
        return JMS_OK;
    }

    return JMS_ERR_MIME_NOT_FOUND;
}
