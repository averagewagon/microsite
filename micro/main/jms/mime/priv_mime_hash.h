#ifndef JMS_PRIV_MIME_HASH_H
#define JMS_PRIV_MIME_HASH_H

#include <stddef.h>

/**
 * @brief Hashes the given string to an index in the MIME lookup table.
 *
 * @param str The input string (e.g., file extension).
 * @param len The length of the input string.
 * @return The hash value.
 */
unsigned int hash(const char* str, size_t len);

/**
 * @brief Looks up a file extension in the MIME type table.
 *
 * This function uses a perfect hash function to quickly determine if the given
 * file extension exists in the MIME lookup table. If found, it returns the
 * corresponding MIME type string.
 *
 * @param str The file extension string (e.g., "html").
 * @param len The length of the string.
 * @return A pointer to the MIME type string if found, or NULL if not.
 */
const char* in_word_set(const char* str, size_t len);

#endif // JMS_PRIV_MIME_HASH_H
