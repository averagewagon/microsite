# Microsite - [joni-on-micro.site](https://joni-on-micro.site)

**Microsite** is a static website hosted on an **ESP32-S3 microcontroller**.
This project is an experiment in running a functional web server on
resource-constrained hardware. You can check it out live at
**[joni-on-micro.site](https://joni-on-micro.site)**.

## Current Features

- **ESP32-S3 Web Hosting**:

  - Hosted on an `ESP32-S3-DevKitC-1-N16R8` for its crypto capabilities and
    ample memory/storage.
  - Integrates LittleFS with Espressif's httpd to serve static files out of the
    filesystem
  - Handles basic web behavior like HTTPS session management, hierarchical
    pages, brotli compression, hotcaching pages, 404 page redirection, etc.

- **Custom Static Site Generator**:

  - Built as a POSIX shell-based static site generator that handles template
    substitutions, Base64-encoded assets, and bundles all resources into a
    single `.html` file.
  - Designed to minimize HTTPS session overhead and reduce resource usage.

- **Small Test Website**:
  - Includes a simple test website created with the static site generator,
    deployed on the ESP32-S3 to showcase its capabilities.

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
  - Make it tolerable to actually write for the blog, like a markdown-to-html
    generator
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
