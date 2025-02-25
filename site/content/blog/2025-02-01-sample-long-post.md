---
title: Sample Long Post
date: "2025-02-01"
description:
  "A content-rich test post for evaluating Microsite’s styling. Showcases
  formatting elements like headings, tables, lists, and embedded media for
  refining the site’s appearance."
tags:
  - Markdown
  - Microsite
  - Sample
---

This page is a **testbed for Microsite’s CSS styling**. It contains sample
content that exercises different formatting elements—headings, images, tables,
lists, blockquotes, and embedded media. If you’re working on refining the site’s
appearance, this page should provide a **full spectrum** of formatting cases.

Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor
incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis
nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.
Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu
fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in
culpa qui officia deserunt mollit anim id est laborum.

## What is Microsite?

Microsite is an embedded web server running on an ESP32-S3, designed to serve a
fully static website with minimal overhead. This post demonstrates how content
is structured on the site while walking through key aspects of the project.

Dui pellentesque justo mi aptent nunc libero eleifend. Volutpat posuere lacus
gravida aenean ut. Penatibus suscipit pellentesque purus aliquam nisi nibh
ornare suspendisse. Sodales dolor etiam cras consequat in. Aliquet nibh suscipit
natoque justo sem penatibus. Integer erat urna ligula lacinia; placerat per
turpis.

Vestibulum aliquet condimentum quam ullamcorper montes pharetra habitant. Montes
erat pulvinar mus hac feugiat vulputate. Nisl praesent at placerat vehicula id
varius. Lorem ultrices dictum varius; risus fames natoque aenean montes netus.
Arcu diam nisi pulvinar egestas ex fusce vivamus ridiculus adipiscing? Erat
massa fermentum ornare, magna pellentesque id facilisis conubia. Tincidunt
vulputate proin netus netus at aliquam torquent.

### Why Build a Web Server on a Microcontroller?

Modern web servers are powerful but complex. Microsite strips things down to the
essentials:

- **No databases**
- **No dynamic content**
- **No external dependencies**

Just a pure static site hosted on a device with a fraction of the power of a
traditional server. Mauris morbi amet suspendisse suspendisse elementum nec
turpis. Integer sollicitudin ad fusce interdum hac; quam libero eu. Natoque
curae condimentum vel integer pharetra. Varius adipiscing rutrum aliquet
elementum at ante ad magna. Et class vestibulum efficitur praesent natoque.
Vehicula finibus at dictumst sodales feugiat cubilia. Ex velit porttitor platea
pulvinar; accumsan dui parturient consequat.

#### Core Features

- **LittleFS**: A wear-leveling flash filesystem optimized for embedded devices.
- **Brotli compression**: Precompressed HTML, CSS, and assets for faster load
  times.
- **Minimalist request handling**: No unnecessary processing—just raw, optimized
  responses.

{{< media "samples/header-image.webp" "Header Image" >}}

## A Look at the Web Server Code

Microsite uses a modified version of ESP-IDF’s `httpd`. Here’s a simple example
of how it handles file requests:

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

    // Open file if not cached
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

This setup allows Microsite to serve cached content instantly and fall back to
reading from flash storage if needed. Phasellus lobortis efficitur bibendum
consectetur vulputate elit. Cursus laoreet magnis semper ridiculus integer diam
curabitur. Parturient fermentum amet cras cubilia dapibus dui justo urna.
Efficitur primis ac quis quis dictum aliquet parturient. Mus parturient congue
iaculis netus, porttitor fames vel duis montes. Conubia senectus nullam curae
ipsum himenaeos potenti. Curabitur quisque nulla proin ultrices orci vulputate
tortor fusce. Habitasse nascetur quisque non aliquam malesuada at ultricies
vivamus.

Consectetur neque nascetur at nullam risus vehicula laoreet gravida lacus. Eu
erat proin fames eleifend tellus lectus cursus semper magna. Curabitur blandit
neque himenaeos ultrices facilisis per posuere per. Himenaeos primis aliquet
imperdiet urna mollis. Ultricies urna sagittis sem pellentesque facilisis;
luctus phasellus ligula felis. Ligula habitasse hendrerit mattis senectus
nascetur massa quisque. Blandit orci curabitur dui quis suscipit efficitur dis.
Potenti pellentesque euismod ante bibendum mi justo eget euismod.

## Static Site Generation

Microsite’s website is built using **Hugo**, a static site generator. All assets
are embedded directly into the HTML using Base64 encoding, eliminating extra
HTTP requests.

Example of embedding an image:

```md
{‎{< media "samples/circuit-board.webp" "Example Circuit Board" >}}
```

Which renders as:

{{< media "samples/circuit-board.webp" "Example Circuit Board" >}}

## Typography Showcase

This section is designed to test **typography**.

- **Bold text** for emphasis.
- _Italicized text_ for secondary emphasis.
- **_Bold and italic combined._**
- **Underlined text** for importance.
- ~~Strikethrough text~~ for deprecated content.

**Lorem** _ex malesuada_ **consequat mi congue.** **Faucibus faucibus cubilia
penatibus** cubilia volutpat sodales aptent mi finibus. **Pharetra aliquet**
_tincidunt vestibulum_ ~~at eros penatibus rhoncus~~ amet. **Tortor eleifend
hac** _gravida nam elementum_ justo amet **_fringilla penatibus._** **_Bibendum
posuere quis magnis_** _purus ut magna magnis penatibus._ **Aliquet amet
natoque;** penatibus sagittis per mus tempus nulla.

**_Dolor vehicula blandit_** ~~lorem aptent~~, **integer quisque magnis
lobortis!** **Nibh ultrices commodo felis nostra praesent** _aenean quisque._
**Nullam eros cras, erat arcu nostra** ~~eget parturient netus.~~ **_Est congue
malesuada_** penatibus odio donec, tempus magnis congue. **Vulputate blandit
aliquam metus auctor nunc**; **phasellus euismod.** _Nostra tempus praesent eget
a nunc._ **_Vulputate vel vivamus posuere semper lacinia_** orci venenatis.

_Odio blandit maximus mollis cursus dis sit sem potenti._ **Sit nascetur fusce
nisi habitasse lobortis himenaeos justo.** **_Pulvinar bibendum eleifend lacus
non habitasse pretium molestie congue._** **Interdum etiam tellus** fusce
condimentum dapibus aliquam. **Purus taciti suspendisse cubilia** _felis in nunc
luctus urna._ **_Vulputate porta varius viverra a nec iaculis nisi nostra._**
**Lacinia tellus elit mollis feugiat pulvinar ultrices bibendum a aliquam.**
**Eleifend bibendum orci fringilla risus tellus pharetra.** ~~Luctus taciti
netus mi scelerisque~~ curabitur sociosqu tempor ridiculus.

### Footnotes

Here’s a sentence with a footnote[^1]. Another one follows later[^2].

[^1]: Footnotes are useful for references and additional information.
[^2]: They appear at the bottom of the page and can be referenced inline.

Sem et varius mus nostra ultricies nibh tristique laoreet. Dis vestibulum risus
a litora diam facilisi bibendum litora potenti? Elit arcu elit dolor ex ante.
Euismod duis mattis torquent turpis rutrum himenaeos dolor penatibus. Vel luctus
maximus cursus convallis duis eros mollis lacinia eros. Pulvinar orci fermentum
primis; ut dui quisque[^3]. Ridiculus neque class posuere aliquam dictumst
tincidunt neque. Varius class etiam maecenas penatibus, potenti montes dapibus.
Aliquam imperdiet mi et per fermentum massa volutpat ridiculus elementum. Cursus
sit tortor leo euismod cursus.

[^3]:
    Integer rutrum dictum metus varius, efficitur rutrum purus vitae
    ullamcorper. Hendrerit lacus molestie tempor gravida ante dapibus. Nisi
    ornare habitant; id cubilia rhoncus maximus. Varius nisi posuere bibendum
    varius habitasse risus? Efficitur suspendisse risus neque a augue. Eleifend
    curae felis sem rutrum dictum orci elit. Lacinia vehicula purus nec
    imperdiet diam urna.

### Definition Lists

Microsite uses various components:

**LittleFS**:  
: A filesystem optimized for flash storage with wear leveling.

**Brotli Compression**:  
: A method of reducing file sizes for faster delivery.

**ESP-IDF `httpd`**:  
: The HTTP server framework used for request handling.

### Task List

- [x] Implement static file serving
- [x] Enable Brotli compression
- [ ] Add Ethernet support
- [ ] Expand storage with SD card integration

### A Small Table

Tables should be properly formatted and legible:

| Feature             | Status     | Notes                         |
| ------------------- | ---------- | ----------------------------- |
| Static Site Support | ✅ Yes     | Built with Hugo               |
| JavaScript Support  | ❌ No      | JS-free for efficiency        |
| Compression         | ✅ Yes     | Brotli precompression enabled |
| Storage             | ✅ Limited | Runs from flash (LittleFS)    |

### Nested Lists

1. **Server Features**
   - Serves static files
   - Uses Brotli compression
   - Caches frequently accessed files
2. **Deployment Steps**
   1. Build the Hugo site
   2. Compress assets
   3. Flash to the ESP32

- Facilisi convallis dis urna, augue sed quam. Amet mollis per hac fames eu
  tortor bibendum. Nisi habitasse ultricies mauris; sed nisi taciti. Apenatibus
  placerat morbi mauris dis mauris fringilla.
  `Hac nisl finibus facilisis vitae; netus nisi commodo.` Eleifend vulputate
  class sapien orci aenean curabitur justo consequat senectus. Nascetur justo
  per aenean adipiscing porttitor parturient.
  - Laoreet placerat odio magna nostra enim torquent. Netus ullamcorper iaculis
    tempor pharetra pellentesque velit morbi sem sem. Fames vitae dapibus mi
    blandit, mattis magna lectus fusce mi. Congue volutpat maximus augue nec
    auctor adipiscing cursus? Hendrerit non nisi sollicitudin habitant phasellus
    fames magnis. Vivamus ornare congue vestibulum platea ultrices torquent eros
    vulputate. Nostra venenatis nascetur lacus, nam viverra ultricies dui primis
    dignissim.
  - Odio blandit maximus mollis cursus dis sit sem potenti. Sit nascetur fusce
    nisi habitasse lobortis himenaeos justo. Pulvinar bibendum eleifend lacus
    non habitasse pretium molestie congue. Interdum etiam tellus fusce
    condimentum dapibus aliquam. Purus taciti suspendisse cubilia felis in nunc
    luctus urna. Vulputate porta varius viverra a nec iaculis nisi nostra.
    Lacinia tellus elit mollis feugiat pulvinar ultrices bibendum a aliquam.
    Eleifend bibendum orci fringilla risus tellus pharetra. Luctus taciti netus
    mi scelerisque curabitur sociosqu tempor ridiculus.

## Performance Optimizations

### Precompressed HTML and CSS

By using Brotli, files are precompressed before being stored on the ESP32:

```sh
brotli -f --best site/public/index.html -o site/public/index.html.br
```

If the client supports Brotli, the `.br` file is served automatically, reducing
file size significantly.

### Caching for Faster Response Times

Microsite implements a simple **LRU cache** to store frequently accessed files
in memory, reducing flash wear and improving response times. Nunc non dapibus a
venenatis euismod habitant nostra posuere class. Tristique convallis varius
himenaeos litora, blandit non. Etiam vestibulum nullam dolor vestibulum etiam.
Sagittis egestas lobortis rutrum potenti arcu penatibus. Accumsan vulputate
placerat imperdiet commodo duis porttitor ex. In nulla volutpat ornare ultricies
fusce accumsan fermentum aptent.

## Deployment

Flashing the site to an ESP32-S3 involves a few steps:

1. **Generate the static site**
   ```sh
   hugo --minify
   ```
2. **Compress assets**
   ```sh
   brotli -f --best site/public/index.html -o site/public/index.html.br
   ```
3. **Package into a LittleFS image**
   ```sh
   mklittlefs -c site/public -b 4096 -s 2MB littlefs_image.bin
   ```
4. **Flash to the ESP32**
   ```sh
   esptool.py --port /dev/ttyUSB0 write_flash 0x90000 littlefs_image.bin
   ```

## Challenges and Future Plans

> "Serving a site from an ESP32 is cool, but making it _fast_ is the real
> challenge."

The project is still evolving, with upcoming improvements including:

- **Ethernet support** for better stability
- **Expanded storage** via SD cards
- **More advanced request handling** by modifying ESP-IDF’s `httpd`

> Arcu leo montes purus natoque ante penatibus gravida; mauris semper. Iaculis
> bibendum ultrices nascetur fermentum suspendisse est. Himenaeos consectetur
> quisque bibendum porttitor scelerisque dictum ridiculus class. Pulvinar rutrum
> iaculis massa rhoncus platea tristique aliquet porta. Morbi porttitor hac sit
> quam sem ut. Malesuada nullam sodales lacinia arcu ultrices. Sed condimentum
> mus nostra tempus potenti cubilia mollis. Gravida in ante hendrerit ex fames
> est tortor nostra? Lacinia sed facilisi suscipit libero mi vestibulum. Aliquet
> magna class quisque dignissim fames.

{{< media "samples/beach.webm" "Demo Video" >}}

## Some other elements

### Table with Manual Alignment

You can manually align table columns by using colons (`:`) in the table header
row.

| Left-Aligned |     Centered      |    Right-Aligned |
| :----------- | :---------------: | ---------------: |
| Text one     |     **Bold**      |            12345 |
| Longer text  |     _Italic_      |            67890 |
| Short        | ~~Strikethrough~~ | **_Emphasized_** |

### Superscript and Subscript

- This is **superscript:** `10^2^` → 10²
- This is **subscript:** `H~2~O` → H₂O
- Combining both: `E = mc^2^` → E = mc²

### Block Quote with Attribution

> "The most dangerous phrase in the language is, 'We've always done it this
> way.'"  
> — Grace Hopper

## Conclusion

This post is designed to showcase all the formatting elements available on
Microsite. If you’re working on styling or layout tweaks, this page should
reflect them clearly.

For more details, check out the
[GitHub repo](https://github.com/averagewagon/microsite).
