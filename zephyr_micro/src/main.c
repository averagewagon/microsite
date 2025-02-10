#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/kernel/mm.h> // Shouldn't this be enough for k_mem_free_get?
#include <zephyr/sys/printk.h>

static void print_size(const char* label, size_t bytes)
{
    if (bytes < 1024)
    {
        printk("%s: %d B\n", label, bytes);
    }
    else if (bytes < 1024 * 1024)
    {
        printk("%s: %d KB\n", label, bytes / 1024);
    }
    else
    {
        printk("%s: %d MB\n", label, bytes / (1024 * 1024));
    }
}

void main(void)
{
    // Print CPU frequency
    uint32_t cpu_freq = sys_clock_hw_cycles_per_sec();
    printk("CPU Frequency: %d MHz\n", cpu_freq / 1000000);

    // Print memory details
    print_size("Free Heap", k_mem_free_get());

    // Print PSRAM size if available
    if (DT_NODE_EXISTS(DT_NODELABEL(psram0)))
    {
        const size_t psram_size = DT_REG_SIZE(DT_NODELABEL(psram0));
        print_size("PSRAM Size", psram_size);
    }
    else
    {
        printk("PSRAM not found in devicetree\n");
    }

    // Print flash size
    if (DT_NODE_EXISTS(DT_NODELABEL(flash0)))
    {
        const size_t flash_size = DT_REG_SIZE(DT_NODELABEL(flash0));
        print_size("Flash Size", flash_size);
    }
    else
    {
        printk("Flash not found in devicetree\n");
    }

    while (1)
    {
        k_sleep(K_SECONDS(1));
    }
}