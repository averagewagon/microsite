---
title: "A title"
date: "2025-06-26"
description: "A description"
tags:
  - Microsite
  - Embedded
  - Web
  - Projects
---

The website you're viewing is hosted on a microcontroller.

<!-- TODO: image of the rpi + esp32-s3  I use a Raspberry Pi 5 for monitoring, remote flashing, and a watchdog, -->

There is no reverse proxy or CDN - the entirety of
[joni-on-micro.site](https://joni-on-micro.site) is served directly from the
ESP32-S3. Despite being running on a 13 dollar microcontroller, my web server
and personal blog site earns a
[performance score of 96/100 from PageSpeed Insights](https://pagespeed.web.dev/),
Google's web performance evaluator.

<!-- TODO: Insert image of PageSpeed Insights - https://pagespeed.web.dev/analysis/https-joni-on-micro-site/fz9a0f5cyj?form_factor=mobile -->

This article covers the technical design of the Microsite, including the
firmware, website, and overall publishing process. All of the code is open
source can be viewed in the
[Microsite GitHub repository](https://github.com/averagewagon/microsite).

---

## What is a web server?

When your web browser loads a typical modern webpage, it sends an HTTPS request
to the web server hosting that page. The server responds with the requested
resources, usually some combination of HTML, CSS, JavaScript, and media. Your
browser uses these resources to stitch together a webpage, which is displayed on
your screen.

<!-- Image of the web architecture from wikipedia -->

At the high end, web servers might be entire fleets of rack-mounted computers
which cost millions upon millions of dollars. At the low end, a web server might
be a single low-power machine plugged into a home network somewhere, but it's
the same basic idea.

<!-- Image of the racks of servers -->

Web servers are primarily sized based on the traffic they're expected to serve.
Popular websites with millions of daily views need large web servers, but niche
web pages with only a few thousand viewers per day can get away with
significantly less horsepower.

## The Hardware - an ESP32-S3

For my web server, I chose to use an ESP32-S3-DevKitC-1-N16R8V.

<!-- Image of the ESP32-S3 in my hand -->

These development boards cost around 13 dollars on DigiKey at time of writing,
and have the following specs:

- 2 cores at 240 MHz each
- 512 KB SRAM (fastish memory)
- 8 MB PSRAM (slower external memory)
- 16 MB flash-based storage
- Built-in WiFi connectivity

For a microcontroller, these specs are on the high end. The ESP32-S3 also has a
built-in cryptographic accelerator, which is useful for the TLS operations
required to support HTTPS.

I chose to use this board because I have lots of them lying around, and I've
spent a fair amount of time working with Espressif's framework on my other
projects.

## The Framework - ESP-IDF

When working with Espressif's chips, I have access to the Espressif IoT
Development Framework, or ESP-IDF. This is an open source framework which
bundles several libraries together with the goal of supporting IoT-based
projects.

Among its libraries, ESP-IDF includes a network stack capable of secure TLS
connections, as well as a
[simple built-in HTTP server called `esp-httpd`](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/protocols/esp_http_server.html).
The HTTP server is conceptually simple:

```c
#include <esp_https_server.h>

esp_err_t incoming_get_request_handler_example(httpd_req_t *req) {
    if (strcmp(req->uri, "/") == 0) // If the request is for the root of the site
    {
        httpd_resp_set_status(req, "200 OK"); // Set the HTTP header to indicate success
        httpd_resp_set_type(req, "text/html"); // Set the resource MIME type to HTML
        httpd_resp_send(req, "<h1>Hello World!</h1>", HTTPD_RESP_USE_STRLEN); // Send the payload of the HTML file
    }
    else
    {
        httpd_resp_set_status(req, "404 Not Found"); // Set an error status to indicate a missing resource
        httpd_resp_send(req, "404 Not Found", HTTPD_RESP_USE_STRLEN); // Send the 404 response payload
    }
    return ESP_OK;
}

httpd_uri_t uri_handler = {
    .uri = "/*", // Use a wildcard so that this handler is used for every incoming request
    .method = HTTP_GET,
    .handler = incoming_get_request_handler_example,
    .user_ctx = NULL
};

httpd_register_uri_handler(server, &root_uri); // Register the uri_handler callback with esp-httpd
```

I integrated this code into a simple ESP-IDF project set some configuration
parameters, and flashed the ESP32-S3. The logs for the device gave me a local
IPv4 address, which shows this webpage:

<!-- TODO: Image of the Hello World -->

This is technically a web server. However, there are quite a few things
missing - starting with the ability to represent multiple different web pages.

---

## Serving multiple web pages simultaneously

A typical web server will store each individual webpage as a separate
`my_page.html` file, with internal links to other resource files (such as
`style.css` or `scripts.js`).

```
site/
├── image.jpeg
├── my_page.html
├── style.css
└── scripts.js
```

I started by trying serve multiple webpages without a filesystem by representing
each resource as an entry in the ESP32-S3's key-value store. However, this ended
up looking really similar to a filesystem anyways, without the advantages of
integrity checks, partial reads, and well-defined APIs. I chose to abandon my
KVS and integrate a filesystem into the Microsite, instead.

### Setting up a filesystem - LittleFS

Many IoT projects don't have use for a filesystem, and so ESP-IDF only includes
a very minimal one called the
[SPIFFS Filesystem](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/spiffs.html).
This filesystem is limited in several ways, most notably because it is missing
directories - all files are stored under the root `/spiffs` directory.

I chose to use [LittleFS](https://github.com/littlefs-project/littlefs) for this
project instead. LittleFS is a little fail-safe filesystem designed for
microcontrollers, with features such as:

- Support for hierarchical directory structure
- Minimal memory consumption, which is also strictly bounded
- Power-loss resilience
- Dynamic wear leveling
- Bad block detection

To integrate LittleFS into my ESP-IDF project, I used the
[esp_littlefs port from Jolt Wallet](https://github.com/joltwallet/esp_littlefs).
The LittleFS library mostly uses the standard filesystem APIs from ANSI C:

```c
#include <esp_littlefs.h>
#include <stdio.h>

esp_vfs_littlefs_conf_t conf = {
    .base_path = "/littlefs",
    .partition_label = "littlefs",
    .format_if_mount_failed = false,
    .dont_mount = false,
};

esp_vfs_littlefs_register(&conf);

char buf[MAX_FILE_SIZE];
FILE* f = fopen("/littlefs/index.html", "r");
fread(buf, 1, sizeof(buf), f); // Read the contents of the file into buf
fclose(f);
```

I also had to configure the flash partition table for my ESP32-S3 to accommodate
the new filesystem.

```
# Name,   Type, SubType, Offset,   Size,      Flags
nvs,      data, nvs,     0x9000,   0x5000,
phy_init, data, phy,     0xe000,   0x1000,
app0,     app,  factory, 0x10000,  0x170000,
littlefs, data, littlefs,0x180000, 0xE80000
```

This gives me a maximum total partition size of about 14.5 MB, which is highly
limited by modern web standards. However, if I design a website with
highly-compressed resources and minimal bloat, I could fit a few dozen blog
posts on my website before I need to upgrade the storage with an SD card reader.

### A script for flashing the website

I also need a way of loading all the files onto the Microsite. I made a script
using
[the `mklittlefs` utility from Earle Philhower](https://github.com/earlephilhower/mklittlefs)
which generates a filesystem image by packaging everything contained in a single
directory:

```sh
# Copy all files from SITE_DIR to OUTPUT_RESOURCES
cp -r "$SITE_DIR/"* "$OUTPUT_RESOURCES/"

# Package the resources directory into a LittleFS image
mklittlefs -c "$OUTPUT_RESOURCES" -p 256 -b 4096 -s "$PARTITION_SIZE" "$OUTPUT_IMAGE"

# Erase the littlefs partition and flash the new image
esptool.py --port /dev/ttyACM0 erase_region "$OFFSET" "$PARTITION_SIZE_DEC"
esptool.py --port /dev/ttyACM0 --baud 115200 write_flash "$OFFSET" "$OUTPUT_IMAGE"
```

This script, `flash-site.sh`, will be referenced extensively when I start making
optimizations later.

### Turning a file extension into a MIME type

When a web server sends a resource to a client, it includes a MIME type in the
HTTP header. This MIME type is essentially a label which tells the browser what
kind of data it is receiving. For example, a MIME type of `text/html` indicates
that the resource is an HTML file, while `image/jpeg` signifies a JPEG image.

An easy way to determine MIME types is to associate each MIME type to a specific
file extension. There are
[hundreds of possible MIME types](https://www.iana.org/assignments/media-types/media-types.xhtml),
but only a
[handful of MIME types are commonly used on the web](https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/MIME_types/Common_types).

Since I want to have a dictionary which associates file extensions with MIME
types, I turned to [GNU gperf](https://www.gnu.org/software/gperf/). This is a
utility which generates perfect hash functions in C. The perfect hash function
maps a set of keys to unique values without any collisions, which is optimized
for speed.

I put all of the MIME types I care about into
[a file called mime.gperf](https://github.com/averagewagon/microsite/blob/57d3181732f95a3e50a5f22990e3166011a799ea/micro/components/jms_mime/mime.gperf),
then ran `gperf --output-file=mime_gperf_output.c mime.gperf`. I implemented a
lookup function which used the underlying gperf output, and I'm now able to take
an input filename and get its associated MIME type, while doing minimal parsing
and without allocating extra memory.

```c
#include "mime_gperf_output.c"

/**
 * @brief Determines the MIME type based on the file extension.
 *
 * @param filename The name of the file, including its extension.
 * @param out_mime_type Pointer to store the MIME type string.
 * @return JMS_OK on success, an error code on failure.
 */
jms_err_t jms_mime_get_type(const char* filename, const char** out_mime_type)
{
    if (filename == NULL || out_mime_type == NULL)
    {
        return JMS_ERR_INVALID_ARG;
    }

    // Find the file extension, which starts with a .
    // File extensions are technically allowed to contain multiple periods,
    // but I'll just self-impose a restriction that I won't ever do that
    const char* ext = strrchr(filename, '.');
    if (ext == NULL || *(ext + 1) == '\0')
    {
        return JMS_ERR_MIME_INVALID;
    }
    ext++; // Skip the '.' character

    // in_word_set is the lookup function generated by gperf
    const struct mime_entry* entry = in_word_set(ext, strlen(ext));
    if (entry)
    {
        *out_mime_type = entry->mime_type;
        return JMS_OK;
    }

    return JMS_ERR_MIME_NOT_FOUND;
}
```

This is probably overengineering - a list of structs with a binary search
algorithm would have worked nearly as well. However, using `gperf` minimizes the
amount of string parsing involved, and it was pretty fun to set it up.

### Stitching the pieces together

Now, I have all the pieces needed to serve multiple web pages. I expanded my URI
handler callback which was registered with `esp-httpd`, so that it:

- Parses the incoming URI and determines which file is being requested
- Sets the MIME type of the resource using my gperf-based lookup function
- Reads and sends the contents of the file

This, combined with some error handling, and the web server is functional! It's
capable of serving multiple different pages, which I set by putting some files
into the site directory and running `flash-site.sh`.

---

## Publishing the Website

Now, at this point, the website isn't very performant, and I don't have any
content on it beyond a simple example:

```
site/
├── image.jpeg
├── my_page.html
├── style.css
└── scripts.js
```

I think it's ready for publishing!

To expose my web server to the public internet, I need to do some configuration
with my router, my internet service provider (ISP), a DNS registrar, and a TLS
certificate provider.

### Exposing my ESP32-S3 to the public internet on an OpenWRT router

My router is an OpenWRT One:

<!-- TODO: Picture of Wendy -->

My objective is to forward all HTTP requests directed at my router's public IPv4
address to the microcontroller, and otherwise leave my network as-is.

This sort of thing should be accomplished through a DMZ - where I create an
isolated LAN just for the microcontroller, and a separate LAN for my regular
home network. This way, any security vulnerabilities on the microcontroller
don't result in total access to my home devices. I'll refrain from going too
deeply into the details about this - you should refer to the
[OpenWRT documentation on DMZ configuration](https://openwrt.org/docs/guide-user/firewall/fw3_configurations/fw3_dmz).

Once my DMZ was configured, and my microcontroller is the sole device using it,
I assigned the microcontroller a static private IPv4 address.

```sh
# Assign a static lease for the microcontroller
dhcp.@host[0]=host
dhcp.@host[0].name='microsite'
dhcp.@host[0].mac='3C:84:27:CA:6A:DC'
dhcp.@host[0].ip='192.168.1.181'
```

Now, port forwarding. I set up a firewall rule so that traffic aimed at my
public IP address on port 80 is redirected to port 80 on the microcontroller.
This is the port which handles HTTP.

```sh
# Redirect HTTP traffic (port 80) to the microcontroller
firewall.@redirect[0]=redirect
firewall.@redirect[0].name='microsite_redirect_http'
firewall.@redirect[0].src='wan'
firewall.@redirect[0].dest='lan'
firewall.@redirect[0].proto='tcp'
firewall.@redirect[0].src_dport='80'
firewall.@redirect[0].dest_ip='192.168.1.181'
firewall.@redirect[0].dest_port='80'

```

Now, with this configuration done, all HTTP traffic sent to my router's public
IPv4 address is handled directly by the microcontroller. My website is public!

### Getting a human-readable domain from a DNS registrar

As is common for residential internet, my ISP gives me a dynamic public IPv4
address. At the time of writing, my public IPv4 address is
[136.27.23.63](https://136.27.23.63/), but I don't plan to update this article
if my DHCP lease renews and my IP changes. While this address is public, it's
not very memorable.

Additionally, since we've been gradually
[running out of IPv4 addresses for a while](https://en.wikipedia.org/wiki/IPv4_address_exhaustion),
my ISP hasn't promised to never change my public IP address. If you bookmarked
this website via the IP address, and my ISP decided to switch it up, then all of
my precious traffic would disappear.

This problem is solvable by registering a domain name with a DNS registrar. A
DNS registrar acts as a sort of telephone operator, associating IP addresses
with domain names.

I purchased the `joni-on-micro.site` domain for about 10 dollars on Namecheap,
and then registered my public IP address as its A record.

<!-- TODO: Insert an image of the A record from namecheap -->

Now, whenever anyone enters `joni-on-micro.site` into a browser, the DNS
resolver will find Namecheap's DNS record which points to my public IP address,
and you'll load my website. However, there's an issue:

### Setting up HTTPS using LetsEncrypt

Since I've only configured my website to work with HTTP, browsers will report
that my website is insecure. To upgrade to a secure connection, I'll need to set
up HTTPS.

Data sent over HTTP is sent as plain text, without encryption. This has obvious
ramifications for privacy, as people would be able to read any sensitive
information you put into forms, or the content of any queries you make to
sketchy websites.

Using HTTP without encryption also has some other annoying consequences. Search
engines are hesitant to index HTTP-only websites. Your ISP is able to
[inject advertisements directly into the content of websites you visit](https://datatracker.ietf.org/doc/html/rfc6108),
and attackers are able to pull off similar data tampering feat.

There's also another crucial reason - the insecure icon is really ugly:

<!-- TODO: Insert the image of the lock with a slash -->

To set up HTTPS, I'll first need a TLS certificate issued by a Certificate
Authority (CA). [Let's Encrypt](https://letsencrypt.org/) is a non-profit
Certificate Authority which issues free TLS certificates to help websites set up
TLS.

The Electronic Frontiers Foundation (EFF) provides a CLI utility called
[Certbot](https://certbot.eff.org/) which can be
[configured to retrieve TLS certificates from Let's Encrypt](https://letsencrypt.org/getting-started/).

I use the following certbot command to get TLS certificates for the Microsite:

```sh
sudo certbot certonly --manual --preferred-challenges dns \
    --key-type ecdsa --elliptic-curve secp256r1 \
    --cert-name joni-on-micro.site \
    -d joni-on-micro.site -v
```

The CLI runs you through a quick instructional, guiding you to add a few more
records to your DNS configuration, and resulting in a `fullchain.pem`
certificate file and a `privkey.pem` private key.

I put both of these files into the flash memory of the ESP32-S3, and configured
`esp-httpd` to use HTTPS and set up secure sessions using these credentials.

I also addded some new configuration to my router which redirects traffic on
port 443 to the microcontroller, as HTTPS uses port 443 rather than port 80:

```
# Redirect HTTPS traffic (port 443) to the microcontroller
firewall.@redirect[0]=redirect
firewall.@redirect[0].name='microsite_redirect_https'
firewall.@redirect[0].src='wan'
firewall.@redirect[0].dest='lan'
firewall.@redirect[0].proto='tcp'
firewall.@redirect[0].src_dport='443'
firewall.@redirect[0].dest_ip='192.168.1.181'
firewall.@redirect[0].dest_port='443'
```

After adding all the necessary configuration, my website now uses HTTPS:

<!-- TODO: Image of the secure website -->

I should note that storing my private key in flash memory is horrifically
insecure - if these credentials leak, then attackers would be able to mitigate
all of the protections of HTTPS.

Flash memory has no inherent protections, and it would be a better idea to burn
this private key into the eFuse memory on the ESP32-S3. This is a special
write-once region of memory which is designed for secure key storage . However,
putting a private key into the eFuse memory is permanent, and I'm a fickle being
who rotates keys pretty frequently.

Another good option would be to use an HSM - a separate chip which functions as
a secure enclave. I'm leaving this extension to the future, when I start looking
into putting multiple Microsite boards into a cluster.

---

## Improving the performance

At this point, the website is publicly viewable, but it isn't very performant -
there are several optimizations I'd like to make on the firmware side before
moving on to the content of the blog website itself.

### Implementing a cache for popular resources

Right now, each time a client requests a particular file, I have to read that
entire file from the ESP32-S3's flash memory and send it in chunks. This is
roughly 1000x slower than serving the files directly from RAM - and I have 8 MB
of additional SPIRAM which isn't currently being used. This situation is ripe
for a cache.

To simplify things for myself, I'll implement an allocation-only cache. Most
modern web servers will cache pages dynamically based on usage metrics, but for
my MCU-based server I should reap most of the benefit by simply caching the
files I expect to be most frequently requested.

The interface I'd like to set up is something like:

```c
/**
 * @brief Initializes the cache by preloading files listed in cache.txt into SPIRAM.
 *
 * @param max_cache_size The maximum memory (bytes) to use for caching.
 * @return JMS_OK if cache initialized successfully, or an error code.
 */
jms_err_t jms_cache_init(size_t max_cache_size);

/**
 * @brief Retrieves a cached file.
 *
 * @param path The file path to look up.
 * @param out_data Pointer to store the file's memory location.
 * @param out_size Pointer to store the file size.
 * @return JMS_OK if file is found, or JMS_ERR_CACHE_MISS if not cached.
 */
jms_err_t jms_cache_get(const char* path, const uint8_t** out_data, size_t* out_size);
```

The cache will operate like a simple dictionary - the filepath will act as the
key, and the file's contents will act as the value.

The cache itself is implemented as a double-ended stack/bump allocator. When the
cache is initializd, I allocate the remaining memory on the MCU into one big
memory region. As I load files into the cache. the metadata for each file is
stored at the front of the memory region:

```c
typedef struct
{
    char* filepath; // Pointer to file path (stored at cache_region_front)
    uint8_t* data;  // Pointer to file contents (stored at cache_region_back)
    size_t size;    // File size
} jms_cache_entry_t;
```

And I put the actual contents of each cached file at the back of the memory
region. Since I never change the contents of the cache after initialization, I
can maximally use all of the available cache space, without worrying about
memory fragmentation.

To decide what exactly gets cached, I include a file named `cache.txt` in the
site directory. `cache.txt` is a prioritized list of files that I expect will be
frequently visited, starting with files like `index.html` and `favicon.ico`.
Since `cache.txt` is included in the site directory, it is loaded onto the
microcontroller whenever I run `flash-site.sh`, and can be found at
[joni-on-micro.site/cache.txt](https://joni-on-micro.site/cache.txt).

When `jms_cache_init` is ran, it looks for `cache.txt`, and attempts to cache
each file one-by-one until it runs out of space. Now, all I have to do is define
a `cache.txt` when I'm making my website, and I can serve popular webpages
directly from PSRAM instead of waiting to read from flash memory.

### Compressing files with Brotli

Currently, whenever a client requests a resource (e.g. an HTML file), the raw,
uncompressed version of that file is sent in its totality. Modern web browsers
are capable of requesting compressed versions of resources, which helps reduce
the size of the files in transit and thus speed up the transmission process. The
web browser handles the decompression of the file locally, so to the user, the
resulting webpage still looks identical.

For highly dynamic content, a web server might compress the files right before
sending them to clients. This requires powerful hardware, as compression is a
computationally-intensive task. Thankfully, I'm serving static content - this
means I can pre-compress the files before I flash them onto the microcontroller.

Historically, the compression algorithm used by web browsers has been `gzip`.
However, `brotli` compression is a newer algorithm which usually achieves better
compression ratios on web resources.
[According to caniuse.com, brotli compression has been supported across all modern browsers since 2017](https://caniuse.com/brotli).
However, in the interest of compatibility with older and more niche browsers,
I'll continue to have uncompressed versions of my web resources available.

I modified the `flash-site.sh` script to compress all my web resources before
flashing the microcontroller. The new script looks something like this:

```sh
# Copy all files from SITE_DIR to OUTPUT_RESOURCES
cp -r "$SITE_DIR/"* "$OUTPUT_RESOURCES/"

# Make brotli compressed versions of all the files in OUTPUT_RESOURCES
find "$OUTPUT_RESOURCES" -type f | while IFS= read -r file; do
    brotli -f --best "$file" -o "$file.br"
done

# Package the resources directory into a LittleFS image
mklittlefs -c "$OUTPUT_RESOURCES" -p 256 -b 4096 -s "$PARTITION_SIZE" "$OUTPUT_IMAGE"

# Erase the littlefs partition and flash the new image
esptool.py --port /dev/ttyACM0 erase_region "$OFFSET" "$PARTITION_SIZE_DEC"
esptool.py --port /dev/ttyACM0 --baud 115200 write_flash "$OFFSET" "$OUTPUT_IMAGE"
```

Next, I have to handle serving the compressed versions of files from the
microcontroller. Now, a client's incoming HTTP request might look something like
this:

```http
GET / HTTP/1.1
Host: joni-on-micro.site
Accept-Encoding: br, gzip, deflate
...
```

The presence of `br` in the `Accept-Encoding` field indicates that this client
is capable of receiving brotli-compressed files. In my firmware, I check this
field and serve the brotli-compressed version of the file if it's supported.
Otherwise, I serve the raw uncompressed version. With that, I'm able to
significantly decrease the size of my resources before serving them to the
clients.

---

## Hugo

TODO
