#include "unity.h"

#include "jms_error.h"
#include "jms_mime.h"

#define BUFFER_SIZE 64

// Test valid MIME type detection
TEST_CASE("Valid MIME type detection", "[mime]")
{
    char buffer[BUFFER_SIZE];
    TEST_ASSERT_EQUAL(JMS_OK, jms_mime_getType("file.html", buffer, BUFFER_SIZE));
    TEST_ASSERT_EQUAL_STRING("text/html", buffer);

    TEST_ASSERT_EQUAL(JMS_OK, jms_mime_getType("style.css", buffer, BUFFER_SIZE));
    TEST_ASSERT_EQUAL_STRING("text/css", buffer);

    TEST_ASSERT_EQUAL(JMS_OK, jms_mime_getType("script.js", buffer, BUFFER_SIZE));
    TEST_ASSERT_EQUAL_STRING("application/javascript", buffer);
}

// Test invalid extensions
TEST_CASE("Invalid MIME type handling", "[mime]")
{
    char buffer[BUFFER_SIZE];
    TEST_ASSERT_EQUAL(JMS_ERR_MIME_NOT_FOUND, jms_mime_getType("unknown.xyz", buffer, BUFFER_SIZE));
}

// Test missing or invalid extensions
TEST_CASE("No extension or invalid file names", "[mime]")
{
    char buffer[BUFFER_SIZE];
    TEST_ASSERT_EQUAL(JMS_ERR_MIME_INVALID, jms_mime_getType("file", buffer, BUFFER_SIZE));
    TEST_ASSERT_EQUAL(JMS_ERR_MIME_INVALID, jms_mime_getType(".hiddenfile", buffer, BUFFER_SIZE));
}

// Test buffer size limitations
TEST_CASE("Buffer too small", "[mime]")
{
    char buffer[4]; // Intentionally small buffer
    TEST_ASSERT_EQUAL(JMS_ERR_INVALID_ARG, jms_mime_getType("file.html", buffer, sizeof(buffer)));
}
