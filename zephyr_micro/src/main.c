#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/kernel.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/printk.h>

void main(void)
{
    printk("Checking flash memory size...\n");

    /* Get flash controller device */
    const struct device* flash_dev = DEVICE_DT_GET_ANY(espressif_esp32_flash_controller);
    if (!device_is_ready(flash_dev))
    {
        printk("Flash device not found or not ready!\n");
        return;
    }

    /* Get total number of pages */
    size_t page_count = flash_get_page_count(flash_dev);
    printk("Flash Page Count: %zu\n", page_count);

    struct flash_pages_info info;
    size_t total_size = 0;

    /* Try to get the first page's size */
    if (flash_get_page_info_by_idx(flash_dev, 0, &info) == 0)
    {
        printk("Flash Page Size (First Page): %zu bytes\n", info.size);

        /* Check if pages are uniform */
        bool uniform = true;
        total_size = page_count * info.size;

        /* Iterate over all pages to check for non-uniform sizes */
        for (uint32_t i = 1; i < page_count; i++)
        {
            if (flash_get_page_info_by_idx(flash_dev, i, &info) == 0)
            {
                if (info.size != total_size / page_count)
                {
                    uniform = false;
                }
                total_size += info.size;
            }
        }

        if (uniform)
        {
            printk("Flash memory appears uniform.\n");
        }
        else
        {
            printk("Flash memory has non-uniform pages; total size calculated iteratively.\n");
        }

        printk("Total Flash Memory Size: %zu bytes\n", total_size);
    }
    else
    {
        printk("Failed to get flash page info.\n");
    }

    while (1)
    {
        k_sleep(K_SECONDS(1));
    }
}
