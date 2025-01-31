#ifndef JMS_FILESYSTEM_H
#define JMS_FILESYSTEM_H

#include "jms_error.h"
#include <stddef.h>

/**
 * @brief Initializes and mounts the filesystem.
 *
 * @return JMS_OK on success, or an error code if the filesystem could not be mounted.
 */
jms_err_t jms_fs_init(void);

/**
 * @brief Reads an entire file into a provided buffer.
 *
 * @param path The file path.
 * @param buffer The buffer to store the file content.
 * @param buffer_size The size of the buffer.
 * @param out_bytes_read Pointer to store the actual number of bytes read.
 * @return JMS_OK on success, or an error code on failure.
 */
jms_err_t jms_fs_read(const char* path, void* buffer, size_t buffer_size, size_t* out_bytes_read);

#endif // JMS_FILESYSTEM_H
