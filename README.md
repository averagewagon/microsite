# Microsite - [averagewagon.dev](https://averagewagon.dev)

**Microsite** is a small static website hosted on an ESP32-S3 microcontroller,
live at **[averagewagon.dev](https://averagewagon.dev)**. This project
experiments with running a functional website on resource-limited hardware.

## Feature List

- **ESP32-Based Multi-Page Website**:
  - Running on the ESP32-S3 for fast crypto operations and memory/storage
    - Currently using an `ESP32-S3-DevKitC-1-N16R8`
    - Will upgrade to the
      [LILYGO T-ETH-LITE](https://lilygo.cc/products/t-eth-lite) to use Ethernet
      instead of WiFi
  - Uses the default WiFi stack for connectivity
  - Serves multiple static pages using LittleFS as the filesystem.
- **Custom Static Site Generator**:
  - The website is built using a custom POSIX shell-based static site generator
  - Supports multi-page hosting, template substitutions, and Base64-encoded
    assets for self-contained pages.
  - Focuses on including all resources into a single HTML file, to minimize
    HTTPS sessions (which are very expensive to set up on my dinky little MCU)

## Prerequisites

- ESP-IDF v5.1+
- Shellcheck
- mklittlefs

## To-Do List

### Short-Term

- Reuse HTTPS session when multiple requests are made
- Serve gzip'd resources instead of uncompressed resources
- Replace the ESP32's `protocol_examples_common` module with a small WiFi
  handler
  - WiFi SSID/Passphrase is currently handled through config fields
  - Unnecessary code size
- Implement a more secure certificate storage method, such as an HSM
  - Certificate/Pkey is currently copied into flash memory
- Put my [motorcycle game](https://github.com/averagewagon/wasm4-zig-game) on
  the site
- Load LittleFS contents into memory at boot and serve from a preloaded cache

### Long-Term

- Add a page for basic site statistics (e.g. CPU usage, memory usage, power
  usage, uptime, requests served)
- Implement proper dynamic DNS system instead of just manually updating
  Namecheap when my IP rotates
- Stress test the site to get some interesting statistics
- Investigate additional hardware modules
  - Ethernet PHY (maybe the W5500, but it has no PoE)
  - External storage (e.g. SPI NOR flash)
  - Hardware crypto accelerator
  - Hardware security module
  - Solar power, or some other cute power strategy
- Manufacture a custom PCB and housing
  - Wouldn't it be cute if it fit into 3.5 inch HDD drive cages
- Host a blog, with the first entry being about web hosting on MCUs
- Implement RSS/Atom feed publisher
