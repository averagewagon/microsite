# Microsite - [joni-on-micro.site](https://joni-on-micro.site)

Microsite is a static website hosted on an ESP32-S3 microcontroller. This
project is an experiment in running a functional web server on
resource-constrained hardware. You can check it out live at
[joni-on-micro.site](https://joni-on-micro.site).

The project consists of three main components:

1. **MCU Firmware**: A custom ESP-IDF-based web server that:

   - Uses LittleFS for efficient flash storage management
   - Implements intelligent caching strategies
   - Handles Brotli compression for HTML files
   - Provides HTTPS support with built-in certificate management

2. **Static Site Generator**: A Hugo-based website that:

   - Embeds all assets (images, CSS, fonts) directly into HTML files
   - Implements Base64 encoding for media content
   - Provides a blog system with tag filtering and RSS support
   - Uses custom shortcodes for handling media embedding

3. **Build & Deployment Tools**: Shell scripts that:
   - Generate and compress the static site
   - Create LittleFS images
   - Handle flashing to the ESP32
   - Manage cache file generation

## Future

- Set up a Raspberry Pi or such to act as a monitor and watchdog

  - Reboot the MCU if the website fails to load, check every 5 minutes
  - Store the UART output of the device in files with timestamps
  - Write some scripts so I can determine uptime, outages, traffic, etc. just
    from the UART logs

- Conduct some stress testing of the website

  - Website currently really struggles with concurrent connections, lots of
    dropped things
  - Should come up with benchmarks so that I can tune the ESP-IDF configuration,
    and use these benchmarks in my first article
  - When I make future improvements, the benchmarks will help me assess their
    effectiveness

- Write up the first article (on the Microsite progress up to this point)

  - Reached a reasonable stopping point for "first draft", gotta write it down
    before I forget

### Maybes/Eventualies

- Port the project to the Zephyr RTOS

  - Check out their HTTPD implementation, maybe it solves some of my issues with
    ESP-IDF's httpd
  - Want to be able to more easily swap between development boards

- Finish porting the project to the
  [LILYGO T-ETH-LITE](https://lilygo.cc/products/t-eth-lite)

  - Replace WiFi with Ethernet for lower latency and better reliability
  - Add SD card support

- Create a custom PCB

  - Add Ethernet support (W5500)
    - Optional: POE?
  - Add an SD card slot
  - Use an HSM with a crypto coprocessor, maybe the NXP SE050

    - Will be a more secure way of handling my LetsEncrypt credentials than my
      current solution of "put the HTTPS private key in the flash"
    - Handles the ECDSA ServerKeyExchange message signing in hardware
    - Handles the ECDHE session key generation in hardware
    - ESP32-S3 already has accelerators for AES-256-GCM, which uses the session
      key to encrypt payloads

- Set up a "Microsite Server Rack" using 3.5 inch HDD cages

  - Configure round-robin load balancing to distribute load between Microsite
    boards
  - Consider airflow, use a fan for cooling the rack
  - House the custom PCBs in form factors that allow for hot-swapping, the same
    way you'd hot swap a 3.5 inch drive

- Create a "site statistics" page showing information about the MCU itself
  - CPU usage
  - Memory usage
  - Uptime
  - Activity/request metrics
