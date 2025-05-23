#include <esp_log.h>
#include <stdio.h>
#include <string.h>

#include "jms_error.h"
#include "jms_mime.h"

#include "mime_gperf_output.c"

static const char* TAG = "JMS_MIME";

jms_err_t jms_mime_get_type(const char* filename, const char** out_mime_type)
{
    if (filename == NULL || out_mime_type == NULL)
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
        *out_mime_type = entry->mime_type;
        return JMS_OK;
    }

    return JMS_ERR_MIME_NOT_FOUND;
}
