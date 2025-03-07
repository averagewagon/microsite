#include "esp_log.h"
#include "unity.h"

#include "jms_error.h"
#include "jms_mime.h"

#define BUFFER_SIZE 64
char* MIME_TAG = "TEST_MIME";

// Test valid MIME type detection
TEST_CASE("Valid MIME type detection", "[mime]")
{
    char buffer[BUFFER_SIZE];

    ESP_LOGI(MIME_TAG, "Testing valid MIME for file.html (expect text/html)");
    TEST_ASSERT_EQUAL(JMS_OK, jms_mime_get_type("file.html", buffer, BUFFER_SIZE));
    TEST_ASSERT_EQUAL_STRING("text/html", buffer);

    ESP_LOGI(MIME_TAG, "Testing valid MIME for style.css (expect text/css)");
    TEST_ASSERT_EQUAL(JMS_OK, jms_mime_get_type("style.css", buffer, BUFFER_SIZE));
    TEST_ASSERT_EQUAL_STRING("text/css", buffer);

    ESP_LOGI(MIME_TAG, "Testing valid MIME for script.js (expect application/javascript)");
    TEST_ASSERT_EQUAL(JMS_OK, jms_mime_get_type("script.js", buffer, BUFFER_SIZE));
    TEST_ASSERT_EQUAL_STRING("text/javascript", buffer);
}

// Test invalid extensions
TEST_CASE("Invalid MIME type handling", "[mime]")
{
    char buffer[BUFFER_SIZE];

    ESP_LOGI(MIME_TAG, "Testing invalid MIME for unknown.xyz (expect error)");
    TEST_ASSERT_EQUAL(JMS_ERR_MIME_NOT_FOUND,
                      jms_mime_get_type("unknown.xyz", buffer, BUFFER_SIZE));
}

// Test missing or invalid extensions
TEST_CASE("No extension or invalid file names", "[mime]")
{
    char buffer[BUFFER_SIZE];

    ESP_LOGI(MIME_TAG, "Testing file with no extension (expect error)");
    TEST_ASSERT_EQUAL(JMS_ERR_MIME_INVALID, jms_mime_get_type("file", buffer, BUFFER_SIZE));

    ESP_LOGI(MIME_TAG, "Testing hidden file with no extension (expect error)");
    TEST_ASSERT_EQUAL(JMS_ERR_MIME_NOT_FOUND,
                      jms_mime_get_type(".hiddenfile", buffer, BUFFER_SIZE));
}

// Test buffer size limitations
TEST_CASE("Buffer too small", "[mime]")
{
    char buffer[4]; // Intentionally small buffer

    ESP_LOGI(MIME_TAG, "Testing buffer too small for file.html (expect error)");
    TEST_ASSERT_EQUAL(JMS_ERR_INVALID_ARG, jms_mime_get_type("file.html", buffer, sizeof(buffer)));
}
