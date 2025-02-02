#ifndef JMS_CACHE_H
#define JMS_CACHE_H

#include "jms_error.h"
#include <stddef.h>
#include <stdint.h>

#define JMS_CACHE_MAX_PATH 128 // Maximum length for cached file paths

/**
 * @brief Initializes the cache by preloading files into SPIRAM.
 *
 * @param files An array of file paths to cache.
 * @param file_count The number of files in the list.
 * @param max_cache_size The maximum memory (bytes) to use for caching.
 * @return JMS_OK if cache initialized successfully, or an error code.
 */
jms_err_t jms_cache_init(const char** files, size_t file_count, size_t max_cache_size);

/**
 * @brief Retrieves a cached file.
 *
 * @param path The file path to look up.
 * @param out_data Pointer to store the file's memory location.
 * @param out_size Pointer to store the file size.
 * @return JMS_OK if file is found, or JMS_ERR_CACHE_MISS if not cached.
 */
jms_err_t jms_cache_get(const char* path, const uint8_t** out_data, size_t* out_size);

#endif // JMS_CACHE_H
