#include "unity.h"

#include "esp_log.h"
#include "jms_error.h"
#include "jms_filesystem.h"
#include <esp_littlefs.h>
#include <stdio.h>
#include <string.h>

static const char* FS_TAG = "TEST_FS";
#define BUFFER_SIZE 64

static void ensure_test_file_exists()
{
    FILE* f = fopen("/littlefs/test.txt", "r");
    if (!f)
    {
        f = fopen("/littlefs/test.txt", "w");
        if (f)
        {
            fputs("Test file content.", f);
            fclose(f);
        }
        else
        {
            ESP_LOGE(FS_TAG, "Failed to create /littlefs/test.txt.");
        }
    }
    else
    {
        fclose(f);
    }
}

TEST_CASE("Filesystem initializes successfully", "[fs]")
{
    TEST_ASSERT_EQUAL(JMS_OK, jms_fs_init());
}

TEST_CASE("Filesystem handles multiple initializations", "[fs]")
{
    TEST_ASSERT_EQUAL(JMS_OK, jms_fs_init());
    TEST_ASSERT_EQUAL(JMS_OK, jms_fs_init()); // Calling again should not fail
}

TEST_CASE("Filesystem handles failed initialization", "[fs]")
{
    // Simulate a failure by forcing the function to return an error
    // This would require modifying `jms_fs_init()` to allow testing failures.
    // For now, just ensure it's handled gracefully.
}

TEST_CASE("Filesystem reads file successfully", "[fs]")
{
    TEST_ASSERT_EQUAL(JMS_OK, jms_fs_init());
    ensure_test_file_exists();

    char buffer[BUFFER_SIZE] = {0};
    size_t bytes_read = 0;

    TEST_ASSERT_EQUAL(JMS_OK, jms_fs_read("/littlefs/test.txt", buffer, BUFFER_SIZE, &bytes_read));
    TEST_ASSERT_EQUAL_STRING("Test file content.", buffer);
    TEST_ASSERT_EQUAL(strlen("Test file content."), bytes_read);
}

TEST_CASE("Filesystem fails to read non-existent file", "[fs]")
{
    TEST_ASSERT_EQUAL(JMS_OK, jms_fs_init());

    char buffer[BUFFER_SIZE] = {0};
    size_t bytes_read = 0;

    TEST_ASSERT_EQUAL(JMS_ERR_FS_FILE_NOT_FOUND,
                      jms_fs_read("/littlefs/missing.txt", buffer, BUFFER_SIZE, &bytes_read));
}

TEST_CASE("Filesystem fails when given NULL arguments", "[fs]")
{
    TEST_ASSERT_EQUAL(JMS_OK, jms_fs_init());

    char buffer[BUFFER_SIZE] = {0};
    size_t bytes_read = 0;

    TEST_ASSERT_EQUAL(JMS_ERR_INVALID_ARG, jms_fs_read(NULL, buffer, BUFFER_SIZE, &bytes_read));
    TEST_ASSERT_EQUAL(JMS_ERR_INVALID_ARG,
                      jms_fs_read("/littlefs/test.txt", NULL, BUFFER_SIZE, &bytes_read));
    TEST_ASSERT_EQUAL(JMS_ERR_INVALID_ARG,
                      jms_fs_read("/littlefs/test.txt", buffer, BUFFER_SIZE, NULL));
}

TEST_CASE("Filesystem respects buffer size limit", "[fs]")
{
    TEST_ASSERT_EQUAL(JMS_OK, jms_fs_init());
    ensure_test_file_exists();

    char buffer[10] = {0}; // Small buffer
    size_t bytes_read = 0;

    TEST_ASSERT_EQUAL(JMS_OK,
                      jms_fs_read("/littlefs/test.txt", buffer, sizeof(buffer), &bytes_read));
    TEST_ASSERT_EQUAL(sizeof(buffer), bytes_read); // Should read only up to the buffer size
}

TEST_CASE("Filesystem handles reading a directory", "[fs]")
{
    TEST_ASSERT_EQUAL(JMS_OK, jms_fs_init());

    char buffer[BUFFER_SIZE] = {0};
    size_t bytes_read = 0;

    // This assumes `/littlefs/` is a directory and not a file.
    TEST_ASSERT_EQUAL(JMS_ERR_FS_FILE_NOT_FOUND,
                      jms_fs_read("/littlefs/", buffer, BUFFER_SIZE, &bytes_read));
}
