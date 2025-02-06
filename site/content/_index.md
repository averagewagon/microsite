---
title: Home
---

<a href=https://github.com/averagewagon/microsite>{{< media "microsite.svg" "Link to Microsite's GitHub" "max-width:15%;min-width:40px;float:right;" >}}</a>

# Joni on Microsite

Welcome to **Microsite**, an experiment in running a fully static website on an
ESP32-S3 microcontroller. This project pushes the limits of resource-constrained
hardware, serving a self-contained Hugo-generated website directly from flash
storage.

This project is **under construction**, and currently only hosts content
generated with LLMs.

## What is Microsite?

Microsite is a minimalist web server that serves a static site entirely from an
ESP32-S3. It uses **LittleFS**, Brotli compression, and an optimized HTTP stack
to deliver content efficiently. The goal is to explore the feasibility of
embedded web hosting while keeping the system lean and performant.

## Project Goals

- **Minimalist yet fully functional**: No dynamic content, no JavaScript
  bloat—just pure static HTML.
- **Embedded-first**: Optimized for constrained environments with efficient
  caching and compression.
- **Self-contained deployment**: Everything is served from flash memory,
  reducing HTTPS overhead.

Microsite will be presented at **CrowdSupply Teardown 2025**, where I'll be
sharing insights into building functional web servers on tiny hardware.

For more details, check out the [project README](https://joni-on-micro.site).
