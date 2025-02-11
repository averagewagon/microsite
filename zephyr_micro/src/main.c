#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// Wifi stuff
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>

// Heap stuff
#include <soc/soc_memory_layout.h>
#include <zephyr/multi_heap/shared_multi_heap.h>

// Flash stuff
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>

LOG_MODULE_REGISTER(JMS_MAIN, LOG_LEVEL_INF);

#ifndef WIFI_SSID
#define WIFI_SSID "default_ssid"
#endif

#ifndef WIFI_PSK
#define WIFI_PSK "default_psk"
#endif

static struct net_mgmt_event_callback wifi_cb;
static struct net_mgmt_event_callback ipv4_cb;
static K_SEM_DEFINE(wifi_connected, 0, 1);
static K_SEM_DEFINE(ipv4_address_obtained, 0, 1);

static void handle_wifi_connect_result(struct net_mgmt_event_callback* cb)
{
    const struct wifi_status* status = (const struct wifi_status*)cb->info;

    if (status->status)
    {
        LOG_ERR("WiFi connection failed (%d)", status->status);
    }
    else
    {
        LOG_INF("WiFi connected!");
        k_sem_give(&wifi_connected);
    }
}

static void handle_wifi_disconnect_result(struct net_mgmt_event_callback* cb)
{
    LOG_WRN("WiFi disconnected");
    k_sem_take(&wifi_connected, K_NO_WAIT);
}

static void handle_ipv4_result(struct net_if* iface)
{
    char buf[NET_IPV4_ADDR_LEN];

    for (int i = 0; i < NET_IF_MAX_IPV4_ADDR; i++)
    {
        if (iface->config.ip.ipv4->unicast[i].ipv4.addr_type != NET_ADDR_DHCP)
        {
            continue;
        }

        LOG_INF("IPv4 Address: %s",
                net_addr_ntop(AF_INET,
                              &iface->config.ip.ipv4->unicast[i].ipv4.address.in_addr,
                              buf, sizeof(buf)));
        LOG_INF("Subnet: %s",
                net_addr_ntop(AF_INET, &iface->config.ip.ipv4->unicast[i].netmask, buf,
                              sizeof(buf)));
        LOG_INF("Router: %s",
                net_addr_ntop(AF_INET, &iface->config.ip.ipv4->gw, buf, sizeof(buf)));
        LOG_INF("Lease time: %u seconds", iface->config.dhcpv4.lease_time);
    }

    k_sem_give(&ipv4_address_obtained);
}

static void wifi_mgmt_event_handler(struct net_mgmt_event_callback* cb,
                                    uint32_t mgmt_event, struct net_if* iface)
{
    switch (mgmt_event)
    {
    case NET_EVENT_WIFI_CONNECT_RESULT:
        handle_wifi_connect_result(cb);
        break;
    case NET_EVENT_WIFI_DISCONNECT_RESULT:
        handle_wifi_disconnect_result(cb);
        break;
    case NET_EVENT_IPV4_ADDR_ADD:
        handle_ipv4_result(iface);
        break;
    default:
        break;
    }
}

void wifi_connect(void)
{
    struct net_if* iface = net_if_get_default();
    struct wifi_connect_req_params params = {0};

    params.ssid = WIFI_SSID;
    params.psk = WIFI_PSK;
    params.ssid_length = strlen(WIFI_SSID);
    params.psk_length = strlen(WIFI_PSK);
    params.channel = WIFI_CHANNEL_ANY;
    params.security = WIFI_SECURITY_TYPE_PSK;
    params.band = WIFI_FREQ_BAND_2_4_GHZ;
    params.mfp = WIFI_MFP_OPTIONAL;

    LOG_INF("Connecting to SSID: %s", params.ssid);

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params)))
    {
        LOG_ERR("WiFi Connection Request Failed");
    }
}

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
    const struct device* flash_dev = DEVICE_DT_GET(DT_PARENT(DT_NODELABEL(flash0)));

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

    LOG_INF("Starting WiFi Connection...");

    LOG_INF("WiFi SSID: %s", WIFI_SSID);

    net_mgmt_init_event_callback(&wifi_cb, wifi_mgmt_event_handler,
                                 NET_EVENT_WIFI_CONNECT_RESULT |
                                     NET_EVENT_WIFI_DISCONNECT_RESULT);
    net_mgmt_init_event_callback(&ipv4_cb, wifi_mgmt_event_handler,
                                 NET_EVENT_IPV4_ADDR_ADD);

    net_mgmt_add_event_callback(&wifi_cb);
    net_mgmt_add_event_callback(&ipv4_cb);

    wifi_connect();

    k_sem_take(&wifi_connected, K_FOREVER);
    k_sem_take(&ipv4_address_obtained, K_FOREVER);

    LOG_INF("WiFi Ready!");

    return 0;
}
