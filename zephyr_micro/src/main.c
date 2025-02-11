#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// Heap stuff
#include <soc/soc_memory_layout.h>
#include <zephyr/multi_heap/shared_multi_heap.h>

// Flash stuff
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>

LOG_MODULE_REGISTER(BASIC_CHECKS, LOG_LEVEL_INF);

void print_psram_info(void)
{
    uint32_t *p_mem, k;

    LOG_INF("Checking PSRAM...");

    p_mem = shared_multi_heap_aligned_alloc(SMH_REG_ATTR_EXTERNAL, 32,
                                            1024 * sizeof(uint32_t));

    if (p_mem == NULL)
    {
        LOG_ERR("PSRAM memory allocation failed!");
        return;
    }

    for (k = 0; k < 1024; k++)
    {
        p_mem[k] = k;
    }

    for (k = 0; k < 1024; k++)
    {
        if (p_mem[k] != k)
        {
            LOG_ERR("p_mem[%" PRIu32 "]: %" PRIu32 " (expected value %" PRIu32 ")", k,
                    p_mem[k], k);
            break;
        }
    }

    shared_multi_heap_free(p_mem);

    if (k < 1024)
    {
        LOG_ERR("Failed checking memory contents.");
        return;
    }

    LOG_INF("PSRAM check completed.");
}

void print_flash_info(void)
{
    const struct device* flash_dev = DEVICE_DT_GET_ANY(espressif_esp32_flash_controller);

    if (!device_is_ready(flash_dev))
    {
        LOG_ERR("Flash device not ready!");
        return;
    }

    size_t total_pages = flash_get_page_count(flash_dev);
    struct flash_pages_info page_info;

    // Get size of the first page
    if (flash_get_page_info_by_idx(flash_dev, 0, &page_info) != 0)
    {
        LOG_ERR("Failed to get flash page info");
        return;
    }

    size_t first_page_size = page_info.size;
    bool uniform_size = true;

    // Iterate through all pages to check for size consistency
    for (size_t i = 1; i < total_pages; i++)
    {
        if (flash_get_page_info_by_idx(flash_dev, i, &page_info) != 0)
        {
            LOG_ERR("Error reading page %zu info", i);
            return;
        }

        if (page_info.size != first_page_size)
        {
            uniform_size = false;
            LOG_ERR("Page %zu has different size: %zu bytes", i, page_info.size);
        }
    }

    if (uniform_size)
    {
        size_t total_flash_size = total_pages * first_page_size;
        LOG_INF("All flash pages have the same size: %zu bytes", first_page_size);
        LOG_INF("Total available flash size: %zu bytes (%zu KB, %zu MB)",
                total_flash_size, total_flash_size / 1024,
                total_flash_size / (1024 * 1024));
    }
    else
    {
        LOG_INF("Flash pages have variable sizes");
    }
}

int main(void)
{
    print_psram_info();
    print_flash_info();

    while (1)
    {
        k_sleep(K_SECONDS(1));
    }

    return 0;
}
