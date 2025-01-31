#include "unity.h"
#include <stdio.h>

#include "test_filesystem.c"
#include "test_mime.c"

void app_main(void)
{
    /* Run all the registered tests and print a summary. */
    UNITY_BEGIN();
    unity_run_all_tests();
    UNITY_END();
}
