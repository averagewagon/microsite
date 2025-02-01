#ifndef JMS_FILESYSTEM_H
#define JMS_FILESYSTEM_H

#include "jms_error.h"
#include <stdint.h>
#include <stdio.h>

typedef struct
{
    FILE* file;
    size_t file_size;
} jms_fs_handle_t;

/**
 * @brief Initializes the filesystem.
 *
 * @return JMS_OK on success, appropriate error code on failure.
 */
jms_err_t jms_fs_init(void);

/**
 * @brief Checks if a file exists.
 *
 * @param path The file path.
 * @return JMS_OK if the file exists, JMS_ERR_FS_FILE_NOT_FOUND otherwise.
 */
jms_err_t jms_fs_exists(const char* path);

/**
 * @brief Opens a file for reading.
 *
 * @param path The file path.
 * @param handle A pointer to a jms_fs_handle_t struct.
 * @return JMS_OK on success, JMS_ERR_FS_FILE_NOT_FOUND if the file doesn't exist.
 */
jms_err_t jms_fs_open(const char* path, jms_fs_handle_t* handle);

/**
 * @brief Reads a chunk from an open file.
 *
 * @param handle The open file handle.
 * @param buffer The buffer to read data into.
 * @param buffer_size The maximum size to read.
 * @param out_bytes_read Pointer to store number of bytes actually read.
 * @return JMS_OK on success, JMS_ERR_FS_READ_FAILED if a read error occurs.
 */
jms_err_t jms_fs_read_chunk(jms_fs_handle_t* handle, void* buffer, size_t buffer_size,
                            size_t* out_bytes_read);

/**
 * @brief Closes an open file.
 *
 * @param handle The file handle.
 */
void jms_fs_close(jms_fs_handle_t* handle);

#endif // JMS_FILESYSTEM_H
