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

Key technical features include:

- Chunked file serving to minimize memory usage
- Pre-cached files in SPIRAM for frequently accessed content
- Modular component architecture with clear separation of concerns
- Support for both compressed (.br) and uncompressed file variants
- Custom error handling system with defined error codes

The project's architecture emphasizes minimalism and efficiency, avoiding
unnecessary abstractions and focusing on serving static content in a
resource-constrained environment. Future plans include Ethernet support, SD card
integration, and custom PCB development with HSM capabilities.

Current limitations:

- Maximum cache size of 7MB
- WiFi-only networking
- Flash storage constraints
- Limited concurrent HTTPS sessions due to mbedtls restrictions

The project serves as both a practical web hosting solution and an exploration
of embedded web server capabilities on minimal hardware.

## Future

### Short-Term Goals

- MCU webserver improvements:

  - Implement smarter bootup caching logic, based on... something?
  - Replace WiFi with Ethernet for lower latency and better reliability
  - Add SD card support
  - Finish porting the project to the
    [LILYGO T-ETH-LITE](https://lilygo.cc/products/t-eth-lite)
  - Implement a watchdog reset if the board becomes unresponsive
  - Figure out if there's a way around the mbedtls 0x0050 concurrency bottleneck

- Website improvements:

  - Fork ESP-IDF `httpd` to allow more low-level control over request handling,
    remove unnecessary copies, etc.
  - Add table of contents generation and permalinkable headers
  - Host compressed `.webm` video content

### Long-Term Goals

- Improving the "production-readiness" of the site:

  - Use OpenWRT-based router to enable round-robin load balancing across
    multiple Microsite boards.
  - Set up a way to monitor and update all boards remotely.
  - Conduct stress testing to measure performance limits.

- Custom Hardware & PCB:

  - Create a custom PCB with HSM, Ethernet, SD card
  - Design an enclosure sized to fit a 3.5-inch HDD drive cage, so it looks like
    a tiny version of a server chassis

- Creating content:
  - Add a site statistics page showing: CPU usage, memory usage, uptime, and
    request metrics.
  - Write a blog post about making the website itself
