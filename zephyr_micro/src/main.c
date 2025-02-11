#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

void main(void)
{
    printk("Checking PSRAM...\n");

    if (DT_NODE_EXISTS(DT_NODELABEL(psram0)))
    {
        size_t psram_size = DT_REG_SIZE(DT_NODELABEL(psram0));
        printk("PSRAM detected: %d MB\n", psram_size / (1024 * 1024));
    }
    else
    {
        printk("PSRAM not detected in devicetree\n");
    }

    while (1)
    {
        k_sleep(K_SECONDS(1));
    }
}
