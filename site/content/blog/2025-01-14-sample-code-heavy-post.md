---
title: Sample Code-Heavy Post
date: "2025-01-14"
description:
  "A post demonstrating syntax highlighting for multiple programming languages,
  including C, Python, JavaScript, Shell, and Rust. Useful for testing
  code-heavy formatting in Microsite."
tags:
  - Code
  - Syntax Highlighting
  - Microsite
  - Sample
  - Markdown
---

This post exists to test syntax highlighting for different programming languages
and see how a code-heavy post looks in Microsite's styling. Below are several
code samples in different languages, including C, Python, JavaScript, Shell, and
Rust.

<!--more-->

## C: Serving a Static File in Microsite

C is the core language of Microsite’s ESP32 firmware. Here’s an example of how
the web server handles file requests:

```c
static jms_err_t serve_file(const jms_ws_request_t* request, char* filepath) {
    jms_fs_handle_t file_handle;
    char buffer[4096];
    size_t bytes_read = 0;

    ESP_LOGI("microsite", "Serving file: %s", filepath);

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

## Python: A Simple HTTP Server

Python makes it easy to spin up a basic web server with just a few lines of
code:

```python
from http.server import SimpleHTTPRequestHandler, HTTPServer

class MyHandler(SimpleHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        self.wfile.write(b"Hello from Python!")

server = HTTPServer(("0.0.0.0", 8000), MyHandler)
print("Serving on port 8000...")
server.serve_forever()
```

## JavaScript: Handling a Click Event

JavaScript isn’t used in Microsite, but here’s a basic example of handling a
button click:

```js
document.getElementById("testButton").addEventListener("click", function () {
  alert("Button clicked!");
});
```

## Shell: Compressing Files with Brotli

Microsite precompresses its static files using Brotli. Here’s the shell command
to compress a file:

```sh
brotli -f --best index.html -o index.html.br
```

To compress an entire directory of HTML files:

```sh
find site/public -type f -name "*.html" -exec brotli -f --best {} \;
```

## Rust: A Minimal Web Server

Rust has become a popular choice for building fast, memory-safe web
applications. Here’s a simple example using Actix Web:

```rust
use actix_web::{web, App, HttpResponse, HttpServer, Responder};

async fn hello() -> impl Responder {
    HttpResponse::Ok().body("Hello from Rust!")
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    HttpServer::new(|| App::new().route("/", web::get().to(hello)))
        .bind("127.0.0.1:8080")?
        .run()
        .await
}
```

## SQL: Selecting Data

For completeness, here’s a basic SQL query:

```sql
SELECT id, name, created_at FROM users WHERE active = 1 ORDER BY created_at DESC;
```

## Conclusion

This post serves as a reference for how code-heavy content looks in Microsite.
If syntax highlighting needs tweaking, this page should make it obvious. Let me
know if you want any other languages added!
