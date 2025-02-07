---
title: About Microsite
description:
  "An overview of Microsite, a fully static website hosted on an ESP32-S3.
  Details its design, filesystem, compression strategies, and deployment
  process."
---

# About Microsite

Microsite is an experiment in hosting a fully static website on an ESP32-S3. The
goal is simple: take a tiny microcontroller with limited resources and make it
serve a complete website efficiently. No dynamic content, no JavaScript bloat,
no databases—just raw, optimized HTML served directly from flash storage.

This isn't just about proving it can be done; it's about exploring how far
embedded hardware can go in real-world web hosting. The site is self-contained,
compressed, and structured to run on a system with a fraction of the power of
even a Raspberry Pi.

## How It Works

Microsite is built from three main components:

1. Firmware
   ([GitHub](https://github.com/averagewagon/microsite/tree/main/micro))  
   The ESP32-S3 runs a custom web server based on ESP-IDF’s `httpd`, serving
   files from LittleFS.

2. Static Site
   ([GitHub](https://github.com/averagewagon/microsite/tree/main/site))  
   Hugo generates the site, embedding all assets inline to eliminate unnecessary
   requests.

3. Build & Flash Scripts
   ([GitHub](https://github.com/averagewagon/microsite/tree/main/scripts))  
   A set of scripts handle compression, filesystem packaging, and flashing to
   the ESP32.

## Webserver and Filesystem

The ESP32 isn’t exactly built to be a web host, but that doesn’t mean it can’t
be one. The server is based on ESP-IDF’s `httpd`, but with a few critical
modifications:

- It prioritizes caching where possible.
- It streams files in chunks instead of loading them all at once.
- It automatically serves compressed files if the client supports Brotli.

Instead of a traditional file system, it uses LittleFS, a flash-friendly format
designed to minimize wear. This is necessary because the ESP32's storage isn’t
like an SSD—you can't just rewrite data indefinitely without degrading the
hardware.

## Compression and Optimization

A standard HTML page has a lot of unnecessary weight. Microsite strips that down
aggressively:

- Brotli compression reduces file sizes before they even hit the ESP32.
- Base64 encoding inlines images, CSS, and fonts directly into the page.
- Pre-minification means the ESP32 never has to process or generate anything
  dynamically.

Every request gets the smallest possible file it can, already optimized before
it's even flashed onto the device.

### Example: Serving a Cached and Compressed File

```c
static jms_err_t serve_file(const jms_ws_request_t* request, char* filepath) {
    jms_fs_handle_t file_handle;
    char buffer[4096];
    size_t bytes_read = 0;
    const char* mime_type;

    ESP_LOGI("microsite", "Serving file: %s", filepath);

    // Check cache first
    if (jms_cache_get(filepath, &cached_data, &cached_size) == JMS_OK) {
        jms_ws_set_response_headers(request, "200 OK", "text/html", NULL, "max-age=86400");
        return jms_ws_response_send(request, (const char*)cached_data, cached_size);
    }

    // Open file from LittleFS if not cached
    if (jms_fs_open(filepath, &file_handle) != JMS_OK) {
        return JMS_ERR_FS_FILE_NOT_FOUND;
    }

    jms_ws_set_response_headers(request, "200 OK", "text/html", NULL, "max-age=86400");

    while (jms_fs_read_chunk(&file_handle, buffer, sizeof(buffer), &bytes_read) == JMS_OK && bytes_read > 0) {
        jms_ws_response_send_chunk(request, buffer, bytes_read);
    }

    jms_ws_response_send_chunk(request, NULL, 0);
    jms_fs_close(&file_handle);
    return JMS_OK;
}
```

This structure allows for fast, low-overhead serving of static content.

## Deployment Process

Everything is built, compressed, and flashed in a few steps:

1. Generate the static site
   ```sh
   hugo --minify
   ```
2. Compress files with Brotli
   ```sh
   brotli -f --best site/public/index.html -o site/public/index.html.br
   ```
3. Create a LittleFS image
   ```sh
   mklittlefs -c site/public -b 4096 -s 2MB littlefs_image.bin
   ```
4. Flash to the ESP32
   ```sh
   esptool.py --port /dev/ttyUSB0 write_flash 0x90000 littlefs_image.bin
   ```

Once flashed, the ESP32 boots up and immediately starts serving files.

## The Bigger Picture

This project is a proof of concept, but it’s not just an academic exercise. The
long-term plan is to see how far this idea can go:

- Ethernet support – WiFi is fine, but wired connections are more stable.
- SD card storage – Expanding beyond flash would allow for much larger sites.
- Optimized request handling – ESP-IDF’s `httpd` is good, but it could be
  better.
- Live telemetry – A simple way to monitor uptime, request counts, and
  performance.

This is also part of a broader interest in microcontroller-constrained web
design—rethinking how websites are built when running on the absolute minimum
hardware.
