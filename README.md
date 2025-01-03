# Microsite - [averagewagon.dev](https://averagewagon.dev)

**Microsite** is a small static website hosted on an ESP32 microcontroller, live
at **[averagewagon.dev](https://averagewagon.dev)**. This project experiments
with running a functional website on resource-limited hardware.

## Feature List

- **ESP32-Based Multi-Page Website**:
  - Used the ESP32-PICO-KIT_V4 because that's what I had lying around
  - Uses the default WiFi stack for connectivity
  - Serves multiple static pages using LittleFS as the filesystem.
- **Zola Static Site Generator**:
  - The website is built using the Zola static site generator
  - Utilizes the tabi theme for Zola

## Prerequisites

- ESP-IDF v5.1+
- Zola
- Shellcheck
- mklittlefs

## To-Do List

### Short-Term

- Serve gzip'd resources instead of uncompressed resources
- Add support for wildcard URI handling
  - esp_http_server doesn't supported wildcard URI handling
  - Currently can't support a custom 404 page, for instance
  - Map requests for `{url}/page` to `{url}/page.html`
  - There are some cursed dynamic memory allocations to work around URI handler
    limits
- Replace the ESP32's `protocol_examples_common` module with a small WiFi
  handler
  - Unnecessary code size
  - WiFi SSID/Passphrase is currently handled through config fields
- Implement a more secure certificate storage method, such as an HSM
  - Certificate/Pkey is currently copied into flash memory
- Put my [motorcycle game](https://github.com/averagewagon/wasm4-zig-game) on
  the site

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
