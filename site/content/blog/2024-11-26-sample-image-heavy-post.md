---
title: Sample Image-Heavy Post
date: "2024-11-26"
description:
  "A test post showcasing how images, GIFs, and videos are displayed in
  Microsite’s layout. Includes examples of various media styles, size
  constraints, and float-based positioning."
tags:
  - Images
  - Formatting
  - Layout
  - Media
  - Styling
---

# Image and Media Formatting Test

This post is designed to test how images, GIFs, videos, and different file types
interact with the layout of a blog post on Microsite. It includes a variety of
media styles, some with size constraints, others positioned using float rules to
mimic a **newspaper-style layout**.

## A Simple Header Image

To start, here's a basic centered image with no additional styling:

{{< media "samples/header-image.webp" "A generic header image" >}}

This should take up the full width of the content column by default.

---

## Floating Images for Text Wrapping

A newspaper layout often has images wrapped by text. Below are examples of how
**float:right** and **float:left** affect the flow of content.

### Image Floating Right

This image should **float to the right**, allowing the text to wrap around it
naturally.

{{< media
  "samples/circuit-board.webp"
  "A close-up of a PCB"
  "true"
  ""
  "max-width:30%;float:right;margin-left:15px;" >}}

Microsite is built to serve static web content directly from flash storage,
using an embedded HTTP server on an ESP32-S3. The entire site is generated using
Hugo, with all assets encoded into the HTML to minimize request overhead.

By embedding images directly using Base64 encoding, the server avoids additional
HTTP requests, reducing load times and improving performance. The tradeoff is a
larger HTML file size, but on an embedded system, eliminating extra requests is
often worth it.

### Image Floating Left

Now, let's try the same thing but with an image floating to the left:

{{< media
  "samples/esp32-module.webp"
  "An ESP32-S3 development board"
  "true"
  ""
  "max-width:30%;float:left;margin-right:15px;" >}}

The ESP32-S3 is a powerful microcontroller, featuring built-in WiFi and
Bluetooth capabilities. While it’s not traditionally used for web hosting, the
Microsite project demonstrates that, with the right optimizations, it can serve
a complete static site directly from flash memory.

One of the key advantages of this approach is that it allows for a **fully
self-contained** website—every page, image, and style is preloaded, so there’s
no need for an external content delivery network or database.

---

## Large Centered Image

Sometimes, you just want an image to be **big and centered**, without any
wrapping text:

{{< media
  "samples/server-rack.webp"
  "A rack-mounted server, the opposite of Microsite"
  "true"
  "max-width:80%;margin:auto;display:block;" >}}

In contrast to traditional server racks, which require dedicated infrastructure
and high power consumption, Microsite runs on a tiny ESP32-S3 board, consuming
just a few milliwatts of power. The goal is not to replace conventional hosting,
but to explore the limits of embedded web servers.

---

## Inline GIFs for Visual Interest

GIFs are often used for animations and simple motion graphics. Let’s add a
small, **inline** animated GIF for emphasis:

{{< media
  "samples/loading-animation.gif"
  "A simple loading animation"
  "false"
  "max-width:15%;min-width:40px;float:right;margin-left:10px;" >}}

With static hosting, there's no server-side processing—every page is just **raw
HTML**. This means no dynamic page generation, no user logins, and no backend
scripts. If an update is needed, the site is simply rebuilt and flashed to the
ESP32-S3 as a **read-only** file system.

This is ideal for applications like **low-power information kiosks, IoT
dashboards, or personal websites** that don’t need frequent updates.

---

## A Full-Width Banner

Let’s try an image that **spans the entire width** of the post, ideal for
section breaks:

{{< media
  "samples/wide-banner.webp"
  "A wide banner image"
  "true"
  "width:100%;margin:auto;display:block;" >}}

Microsite is built with a focus on performance, stripping away unnecessary
complexity. The site is precompressed using Brotli before being stored in
**LittleFS**, a lightweight filesystem optimized for flash memory.

---

## A Thumbnail-Sized Image

Not every image needs to be large. Here’s a **small, centered thumbnail**:

{{< media
  "samples/chip-thumbnail.webp"
  "A tiny microcontroller chip"
  "false"
  "max-width:10%;min-width:40px;margin:auto;display:block;" >}}

While the ESP32-S3 is physically small, its capabilities are impressive. With
**8MB of PSRAM and 16MB of flash storage**, it’s more than capable of running a
small web server with efficient resource management.

---

## Embedded Video

Videos can also be embedded within blog posts. Here’s a **sample `.webm`
video**, displayed in-line:

{{< media "samples/beach.webm" "Short video of a woman on a beach" >}}

This ensures that video elements are properly styled and don’t interfere with
the rest of the post layout.

---

## Wrapping Up

This post demonstrates different ways to lay out images and videos in
Microsite’s blog format. If any spacing, alignment, or responsiveness issues
come up, this should help identify and fix them.

For reference, the media files used here include:

- **JPEG** (header, wide banner)
- **PNG** (floating PCB, ESP32 module)
- **WEBP** (server rack)
- **GIF** (loading animation)
- **WEBM** (embedded video)

By testing multiple formats and layouts, this post ensures that Microsite’s
styling remains **consistent and flexible** across different content types.

---

All media assets are stored in the `samples/` subfolder for easy cleanup if
needed. Let me know if you need any further adjustments!
