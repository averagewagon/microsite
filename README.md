# Microsite - [joni-on-micro.site](https://joni-on-micro.site)

Microsite is a static website hosted on an ESP32-S3 microcontroller. This
project is an experiment in running a functional web server on
resource-constrained hardware. You can check it out live at
[joni-on-micro.site](https://joni-on-micro.site).

The project consists of four main components:

1. **MCU Firmware** in `micro/`:

   An ESP-IDF-based web server (using httpd).

   - Uses LittleFS for efficient flash storage management
   - Implements intelligent caching strategies
   - Handles Brotli compression for HTML files
   - Provides HTTPS support with built-in certificate management

2. **Static Blog Site** in `site/`:

   A Hugo-based personal blog.

   - Embeds all assets (images, CSS, fonts) directly into HTML files
   - Implements Base64 encoding for media content
   - Provides a blog system with tag filtering and RSS support
   - Uses custom shortcodes for handling media embedding

3. **Helper Scripts** in `scripts/`:

   Build, deployment, and general helper scripts.

   - Generate and compress the static site
   - Create LittleFS images
   - Handle flashing to the ESP32
   - Manage cache file generation
   - Install system services for monitoring the site

4. **System Services** in `services/`:

   A set of `systemd` helper services, intended to run on a Raspberry Pi.

   - Capture and log UART output from the ESP32-S3
   - Monitor website availability and restart the board via USB power cycling
   - Run automatically on a Raspberry Pi 4B for continuous operation
   - Are installed via `install-services.sh` in the `scripts/` folder

## Future

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

- Write some scripts which run against the monitor service's output so I can
  determine uptime, outages, traffic, etc. just from the UART logs

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

- Create a dynamic "site statistics" page showing information about the MCU
  itself
  - CPU usage
  - Memory usage
  - Uptime
  - Activity/request metrics
