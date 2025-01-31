# Microsite - [joni-on-micro.site](https://joni-on-micro.site)

**Microsite** is a static website hosted on an **ESP32-S3 microcontroller**.
This project is an experiment in running a functional web server on
resource-constrained hardware. You can check it out live at
**[joni-on-micro.site](https://joni-on-micro.site)**.

## Project Overview

Microsite explores how to serve a static website from a microcontroller while
balancing efficiency, creativity, and the limitations of embedded hardware. It’s
a hands-on dive into constrained web design and minimal hosting.

### Current Features

- **ESP32-S3 Web Hosting**:

  - Hosted on an `ESP32-S3-DevKitC-1-N16R8` for its crypto capabilities and
    ample memory/storage.
  - Transitioning to [LILYGO T-ETH-LITE](https://lilygo.cc/products/t-eth-lite)
    to switch from WiFi to Ethernet, improving stability and latency.
  - Uses LittleFS to manage static files effectively.

- **Custom Static Site Generator**:

  - Built as a POSIX shell-based static site generator that handles template
    substitutions, Base64-encoded assets, and bundles all resources into a
    single `.html` file.
  - Designed to minimize HTTPS session overhead and reduce resource usage.

- **Small Test Website**:
  - Includes a simple test website created with the static site generator,
    deployed on the ESP32-S3 to showcase its capabilities.

## Development Roadmap

### Short-Term Goals

- Implement a watchdog which resets the board if it hasn't received a request in
  a while
- Serve gzip- or brotli-compressed resources for better performance.
- Host a highly compressed webm video.
- Implement a table of contents handler for the static site generator.
- Add support for perma-linkable headers in the static site generator.
- Securely store certificates with a hardware security module (HSM).
- Preload LittleFS contents into memory at boot for faster response times.
- Add interactive content, such as hosting
  [my motorcycle game](https://github.com/averagewagon/wasm4-zig-game).

### Long-Term Goals

- Add a site statistics page to show CPU usage, memory usage, uptime, power
  consumption, and requests served.
- Implement a dynamic DNS system to automate IP address updates.
- Stress test the site to gather interesting performance data.
- Build a microcontroller-based load balancer and set up a fleet of
  microcontroller web hosts to distribute traffic and improve scalability.
- Experiment with hosting personal image and file storage on a microcontroller.
- Try hosting a git server entirely on a microcontroller as an unconventional
  use case.
- Design and manufacture a custom PCB and housing, possibly sized to fit a
  3.5-inch HDD drive cage for compactness and style.
