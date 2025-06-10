<!DOCTYPE html>
<html>
<head>
<style>
/* Global styles */
body {
   margin: 0;
   padding: 0;
   width: 100%;
   height: 100%;
   background: #1e1e1e;
   color: white;
   font-family: Arial, sans-serif;
}
.slide-container {
   width: 100%;
   height: 100vh;
   box-sizing: border-box;
   page-break-after: always;
   display: flex;
   flex-direction: column;
   position: relative;
   min-height: 100vh;
}
.slide-header {
   padding: 10px 20px;
   flex-shrink: 0;
   height: 60px;
}
.slide-footer {
   padding: 10px 20px;
   border-top: 1px solid #333;
   flex-shrink: 0;
   font-size: 0.8em;
   text-align: center;
   width: 100%;
   height: 50px;
}
.slide-content {
   flex: 1;
   display: flex;
   justify-content: center;
   align-items: center;
   overflow: hidden;
   min-height: calc(100vh - 110px);
   box-sizing: border-box;
}
.slide {
   width: 100%;
   height: 100%;
   box-sizing: border-box;
   display: flex;
   justify-content: center;
   align-items: center;
   max-height: calc(100vh - 110px);
}
/* Increase font sizes for common elements */
.slide-content h1, .slide-content h2, .slide-content h3, .slide-content h4, .slide-content h5, .slide-content h6 {
   font-size: 3em;
}
.slide-content p, .slide-content li, .slide-content a {
   font-size: 1.2em;
}
ul, ol {
   font-size: 1.2em;
}
@media print {
   body, .slide-container {
      -webkit-print-color-adjust: exact !important;
      print-color-adjust: exact !important;
   }
   .slide-container {
      page-break-after: always;
      height: auto;
      min-height: 100vh;
      position: relative;
   }
   .slide-footer {
      position: fixed;
      bottom: 0;
      left: 0;
      right: 0;
      page-break-inside: avoid;
   }
   .slide-content {
      min-height: calc(100vh - 110px);
      max-height: calc(100vh - 110px);
   }
   .slide {
      max-height: calc(100vh - 110px);
   }
}
</style>
</head>
<body>

<!-- Title -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-title">
      <style>.slide-title {display: flex; flex-direction: column; justify-content: center; align-items: center; width: 100%; height: 100%; padding: 20px; text-align: center; box-sizing: border-box;}</style>
      <h1>Hosting a Blog on a Microcontroller</h1>
      <p style="font-size: 2.2em">Joni Hendrickson</p>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a></footer>
</div>

<!-- What is it? -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-two-images">
      <style>
        .slide-two-images {
          display: flex;
          flex-direction: column;
          width: 100%;
          height: 100%;
          padding: 20px;
          box-sizing: border-box;
        }
        .slide-two-images .title {
          text-align: center;
          margin-bottom: 30px;
          width: 100%;
        }
        .slide-two-images .images-container {
          display: flex;
          flex: 1;
          width: 100%;
        }
        .slide-two-images .image-container {
          flex: 1;
          display: flex;
          justify-content: center;
          align-items: center;
          overflow: hidden;
        }
        .slide-two-images .image-container:first-child {
          margin-right: 20px;
        }
        .slide-two-images img {
          max-width: 100%;
          max-height: 100%;
          object-fit: contain;
        }
      </style>
      <div style="font-size: .6em">
        <h1>What is the Microsite?</h1>
      </div>
      <div class="images-container">
        <div class="image-container">
          {{< media "pres/microsite_ups.jpg" "The Microsite hardware setup in my studio apartment" "true" "border: 1px solid white;height: 400px;margin-bottom: 20px;width:99%;" "">}}
        </div>
        <div class="image-container">
          {{< media "pres/website_screenshot.png" "A screenshot of the front page of the Microsite blog" "true" "border: 1px solid white;height: 400px;margin-bottom: 20px;width:99%;" "">}}
        </div>
      </div>
    </div>
  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<!-- Why? -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-process">
      <style>
      .slide-process {display: flex; flex-direction: column; width: 100%; height: 100%; padding: 20px; box-sizing: border-box;}
      .slide-process .process-container {flex: 1; display: flex; flex-direction: column; justify-content: center; align-items: center;}
      .slide-process .process-step {width: 100%; padding: 15px; margin-bottom: 20px; display: flex; align-items: center;}
      .slide-process .step-number {width: 30px; height: 30px; border-radius: 50%; display: flex; justify-content: center; align-items: center; margin-right: 15px; background: #333;}
      .slide-process .step-content {flex: 1;}
      .slide-process .connector {height: 20px; width: 2px; margin-left: 15px; background: #444;}
      .slide-process .title {margin-bottom: 30px; font-size: 1.8em; text-align: center;}
      </style>
      <div class="title">Why did I do this?</div>
      <div class="process-container" style="font-size: 1.5em;">
        <div class="process-step">
          <div class="step-number">1</div>
          <div class="step-content">I want to get better at technical writing</div>
        </div>
        <div class="connector"></div>
        <div class="process-step">
          <div class="step-number">2</div>
          <div class="step-content">I should start a blog about microcontrollers</div>
        </div>
        <div class="connector"></div>
        <div class="process-step">
          <div class="step-number">3</div>
          <div class="step-content">I need something to host my blog on</div>
        </div>
        <div class="connector"></div>
        <div class="process-step">
          <div class="step-number">4</div>
          <div class="step-content">I need something to write about</div>
        </div>
      </div>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a></footer>
</div>

<!-- Why? Joke reveal -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-process">
      <style>
      .slide-process {display: flex; flex-direction: column; width: 100%; height: 100%; padding: 20px; box-sizing: border-box;}
      .slide-process .process-container {flex: 1; display: flex; flex-direction: column; justify-content: center; align-items: center;}
      .slide-process .process-step {width: 100%; padding: 15px; margin-bottom: 20px; display: flex; align-items: center;}
      .slide-process .step-number {width: 30px; height: 30px; border-radius: 50%; display: flex; justify-content: center; align-items: center; margin-right: 15px; background: #333;}
      .slide-process .step-content {flex: 1;}
      .slide-process .connector {height: 20px; width: 2px; margin-left: 15px; background: #444;}
      .slide-process .title {margin-bottom: 30px; font-size: 1.8em; text-align: center;}
      </style>
      <div class="title">Why did I do this?</div>
    <div class="process-container" style="font-size: 1.5em;">
        <div class="process-step">
          <div class="step-number">∞</div>
          <div class="step-content">I thought it would be funny</div>
        </div>
      </div>
    </div>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a></footer>
</div>

<!-- What's a web server? -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-two-images">
      <style>
        .slide-two-images {
          display: flex;
          flex-direction: column;
          width: 100%;
          height: 100%;
          padding: 20px;
          box-sizing: border-box;
        }
        .slide-two-images .title {
          text-align: center;
          margin-bottom: 30px;
          width: 100%;
        }
        .slide-two-images .images-container {
          display: flex;
          flex: 1;
          width: 100%;
        }
        .slide-two-images .image-container {
          flex: 1;
          display: flex;
          justify-content: center;
          align-items: center;
          overflow: hidden;
        }
        .slide-two-images .image-container:first-child {
          margin-right: 20px;
        }
        .slide-two-images img {
          max-width: 100%;
          max-height: 100%;
          object-fit: contain;
        }
      </style>
      <div style="font-size: .6em">
        <h1>A Brief Overview of Webservers</h1>
      </div>
      <div class="images-container">
        <div class="image-container">
          {{< media "pres/webserver_diagram.png" "Web server architecture - By Ade56facc - Own work, CC BY-SA 4.0, https://commons.wikimedia.org/w/index.php?curid=112590977" "true" "height: 400px;margin-bottom: 20px;width:99%;" "">}}
        </div>
        <div class="image-container">
          {{< media "pres/Wikimedia_Servers.jpg" "The Wikimedia server farm - By Helpameout - Own work, CC BY-SA 3.0, https://commons.wikimedia.org/w/index.php?curid=20337780" "true" "height: 400px;margin-bottom: 20px;width:99%;" "">}}
        </div>
      </div>
    </div>
  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<!-- Hardware -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-image-text">
      <style>
        .slide-image-text {display: flex; width: 100%; height: 100%; padding: 20px; box-sizing: border-box;}
        .slide-image-text .image-container {flex: 1; display: flex; justify-content: center; align-items: center; margin-right: 20px; overflow: hidden;}
        .slide-image-text .text-container {flex: 1; display: flex; flex-direction: column; justify-content: center; overflow: auto;}
        .slide-image-text img {max-width: 100%; max-height: 100%; object-fit: contain;}
      </style>
      <div class="image-container">
        {{< media "pres/esp32s3_hand_crop.webp" "The ESP32-S3 dev kit, which now runs the Microsite" "true" "" "max-width: 80%">}}
      </div>
      <div class="text-container">
        <h2 style="font-size: 2.5em">The Hardware - an ESP32-S3</h2>

<div style="max-width: 75%">

| Specification                 | ESP32-S3-DevKitC-1-N32R8V                                                                                       |
| ----------------------------- | --------------------------------------------------------------------------------------------------------------- |
| **Cost (at time of writing)** | $13 ([DigiKey](https://www.digikey.com/en/products/detail/espressif-systems/ESP32-S3-DEVKITC-1U-N8R8/16162636)) |
| **Processor**                 | 2 cores at 240 MHz                                                                                              |
| **Flash Storage**             | 32 MB                                                                                                           |
| **RAM**                       | 512 KB SRAM, 8 MB PSRAM                                                                                         |

</div>

<p>Roughly analogous to a Power Mac G3 desktop from 1997</p>

{{< media "pres/Beige_Power_Macintosh_G3_Minitower.jpg" "Power Macintosh G3 - By fr:User:Klodo6975 - https://en.wikipedia.org/wiki/File:Power_Mac_G3_B%26W.jpg, Public Domain" "true" "max-height: 160px;" "margin-right: 160px">}}

</div>
</div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a></footer>
</div>

<!-- The Framework -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-single-image">
      <style>
        .slide-single-image {
          display: flex;
          flex-direction: column;
          width: 100%;
          height: 100%;
          padding: 20px;
          box-sizing: border-box;
          text-align: center;
        }
        .slide-single-image .title {
          margin-bottom: 10px;
          width: 100%;
        }
        .slide-single-image .image-container {
          flex: 1;
          display: flex;
          justify-content: center;
          align-items: center;
          overflow: hidden;
        }
        .slide-single-image img {
          max-width: 80%;
          max-height: 80%;
          object-fit: contain;
        }
      </style>
      <div class="title">
        <h1 style="font-size: 1.6em;">The Framework - ESP-IDF</h1>
      </div>
      <div class="image-container">
        {{< media "pres/espidf_full.png" "https://docs.espressif.com/projects/esp-idf/en/v5.4.1/esp32s3/index.html" "true" "max-height: 500px" "">}}
      </div>
    </div>
  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> |
    <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<!-- Example HTTPD usage -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content" style=" margin-top: -40px;">
    <div class="slide slide-code">
      <style>
      .slide-code {
        display: flex;
        flex-direction: column;
        width: 100%;
        height: 100%;
        box-sizing: border-box;
      }
      .slide-code .code-container {
        flex: 1;
        background: #2a2a2a;
        overflow: auto;
        font-family: monospace;
      }
      .slide-code pre {
        margin: 0;
        white-space: pre;
        font-size: 0.9em;
        line-height: 1.4;
      }
      </style>
      <h2 style="font-size: 1.4em; padding: 0; margin:0; margin-bottom:20px;">Example <code>esp-httpd</code> Usage</h2>
        <div class="code-container">
{{< highlight c >}}
esp_err_t incoming_get_request_handler_example(httpd_req_t *req) {
    if (strcmp(req->uri, "/") == 0) {
        httpd_resp_set_status(req, "200 OK");
        httpd_resp_set_type(req, "text/html");

        httpd_resp_send(req, "<h1>Hello World!</h1>", HTTPD_RESP_USE_STRLEN);
    }
    else {
        httpd_resp_set_status(req, "404 Not Found");
        httpd_resp_send(req, "404 Not Found", HTTPD_RESP_USE_STRLEN);
    }
    return ESP_OK;

}

httpd_uri_t uri_handler = {  
 .uri = "/※",  
 .method = HTTP_GET,  
 .handler = incoming_get_request_handler_example,  
 .user_ctx = NULL  
};

httpd_register_uri_handler(server, &root_uri);

{{< /highlight >}}

</div>
</div>

  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<!-- Hello World -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-image-text">
      <style>
        .slide-image-text {display: flex; width: 100%; height: 100%; padding: 20px; box-sizing: border-box;}
        .slide-image-text .image-container {flex: 1; display: flex; justify-content: center; align-items: center; margin-right: 20px; overflow: hidden;}
        .slide-image-text .text-container {flex: 1; display: flex; flex-direction: column; justify-content: center; overflow: auto;}
        .slide-image-text img {max-width: 100%; max-height: 100%; object-fit: contain;}
      </style>
      <div class="image-container">
        {{< media "pres/hello_world.png" "A screenshot of the website, served by the MCU" "true" "" "max-width: 80%">}}
      </div>
      <div class="text-container">
        <h2 style="font-size: 2.5em">An MCU-Hosted Webpage!</h2>

<p>We're done! It's a microcontroller web server!</p>

</div>
</div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a></footer>
</div>

<!-- What's missing? -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content" style="flex: 1; display: flex; justify-content: center; align-items: center; overflow: hidden; min-height: calc(100vh - 110px); box-sizing: border-box;">
    <div class="slide" style="width: 100%; height: 100%; box-sizing: border-box; display: flex; flex-direction: column; justify-content: center; align-items: center; padding: 20px; text-align: center;">
      <h1 style="font-size: 3em; margin-bottom: 30px; letter-spacing: 1px;">What's Missing?</h1>
      <ul style="font-size: 1.5em; line-height: 1.6; width: 80%; margin-left: 650px; letter-spacing: 0.5px;">
        <li style="margin-bottom: 15px; text-align: left; letter-spacing: 0.5px;">A filesystem</li>
        <li style="margin-bottom: 15px; text-align: left; letter-spacing: 0.5px;">A way of hooking up the filesystem to requests</li>
        <li style="margin-bottom: 15px; text-align: left; letter-spacing: 0.5px;">A cache, probably</li>
        <li style="margin-bottom: 15px; text-align: left; letter-spacing: 0.5px;">URI parsing and HTTP status codes and MIME type handling and Content-Encoding and TLS credentials and HTTPS sessions and cache population and file compression and error handling and filesystem image generation</li>
        <li style="margin-bottom: 15px; text-align: left; letter-spacing: 0.5px;">Oh, and a blog.</li>
      </ul>
    </div>
  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<!-- LittleFS -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-image-text">
      <style>
        .slide-image-text {display: flex; width: 100%; height: 100%; padding: 20px; box-sizing: border-box;}
        .slide-image-text .image-container {flex: 1; display: flex; justify-content: center; align-items: center; margin-right: 20px; overflow: hidden;}
        .slide-image-text .text-container {flex: 1; display: flex; flex-direction: column; justify-content: center; overflow: auto;}
        .slide-image-text img {max-width: 100%; max-height: 100%; object-fit: contain;}
      </style>
      <div class="image-container">
        {{< media "pres/littlefs.png" "https://github.com/littlefs-project/littlefs" "true" "" "max-width: 80%">}}
      </div>
      <div class="text-container">
        <div class="code-container" style="margin-right: 100px;">

{{< highlight c >}}esp_vfs_littlefs_conf_t conf = {  
 .base_path = "/littlefs",  
 .partition_label = "littlefs",  
 .format_if_mount_failed = true,  
 .dont_mount = false,  
});

esp_vfs_littlefs_register(&conf);

char buf[64];  
FILE※ f = fopen("/littlefs/index.html", "r");  
fread(buf, 1, sizeof(buf), f);  
fclose(f);

{{< /highlight >}}

</div>

</div>
</div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a></footer>
</div>

<!-- Cache -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content" style="flex: 1; display: flex; justify-content: center; align-items: center; overflow: hidden; min-height: calc(100vh - 110px); box-sizing: border-box;">
    <div class="slide" style="width: 100%; height: 100%; box-sizing: border-box; display: flex; flex-direction: column; justify-content: center; align-items: center; padding: 20px; text-align: center;">
      <h1 style="font-size: 3em; margin-bottom: 30px; letter-spacing: 1px;">The Cache</h1>
      <ul style="font-size: 1.5em; line-height: 1.6; width: 80%; margin-left: 350px; letter-spacing: 0.5px;">
        <li style="margin-bottom: 15px; text-align: left; letter-spacing: 0.5px;">
          Single SPIRAM memory block, about 7.5 MB
        </li>
        <li style="margin-bottom: 15px; text-align: left; letter-spacing: 0.5px;">
          Bidirectional allocation
          <ul style="margin-top: 5px; font-size: .7em;">
            <li>Metadata grows from front</li>
            <li>File data grows from back</li>
          </ul>
        </li>
        <li style="margin-bottom: 15px; text-align: left; letter-spacing: 0.5px;">
          No deallocator
        </li>
        <li style="margin-bottom: 15px; text-align: left; letter-spacing: 0.5px;">
          Populates with the files from LittleFS
          <ul style="margin-top: 5px; font-size: .7em;">
            <li>Chooses cache items using a cache-priority-list.txt I manually write</li>
          </ul>
        </li>
        <li style="margin-bottom: 15px; text-align: left; letter-spacing: 0.5px;">
          Linear performance for lookup
        </li>
      </ul>
    </div>
  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<!-- The Components -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-single-image">
      <style>
        .slide-single-image {
          display: flex;
          flex-direction: column;
          width: 100%;
          height: 100%;
          padding: 20px;
          box-sizing: border-box;
          text-align: center;
        }
        .slide-single-image .title {
          margin-bottom: 10px;
          width: 100%;
        }
        .slide-single-image .image-container {
          flex: 1;
          display: flex;
          justify-content: center;
          align-items: center;
          overflow: hidden;
        }
        .slide-single-image img {
          max-width: 80%;
          max-height: 80%;
          object-fit: contain;
        }
      </style>
      <div class="title">
        <h1 style="font-size: 1.6em;">My components</h1>
      </div>
      <div class="image-container">
        {{< media "pres/comps.png" "All written as ESP-IDF Components" "true" "height: 450px" "">}}
      </div>
    </div>
  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> |
    <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<!-- phew -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-section-divider">
      <style>
      .slide-section-divider {display: flex; flex-direction: column; justify-content: center; align-items: center; width: 100%; height: 100%; text-align: center; padding: 20px; box-sizing: border-box;}
      .slide-section-divider h1 {font-size: 3em; margin-bottom: 0;}
      .slide-section-divider .divider {width: 200px; height: 2px; margin: 20px auto; background: #444;}
      </style>
      <h1>phew</h1>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a></footer>
</div>

<!-- wait I still have to make a blog -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-section-divider">
      <style>
      .slide-section-divider {display: flex; flex-direction: column; justify-content: center; align-items: center; width: 100%; height: 100%; text-align: center; padding: 20px; box-sizing: border-box;}
      .slide-section-divider h1 {font-size: 3em; margin-bottom: 0;}
      .slide-section-divider .divider {width: 200px; height: 2px; margin: 20px auto; background: #444;}
      </style>
      <h1>wait I still have to make the blog</h1>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a></footer>
</div>

<!-- Hugo & Xmin -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-two-images">
      <style>
        .slide-two-images {
          display: flex;
          flex-direction: column;
          width: 100%;
          height: 100%;
          padding: 20px;
          box-sizing: border-box;
        }
        .slide-two-images .title {
          text-align: center;
          margin-bottom: 30px;
          width: 100%;
        }
        .slide-two-images .images-container {
          display: flex;
          flex: 1;
          width: 100%;
        }
        .slide-two-images .image-container {
          flex: 1;
          display: flex;
          justify-content: center;
          align-items: center;
          overflow: hidden;
        }
        .slide-two-images .image-container:first-child {
          margin-right: 20px;
        }
        .slide-two-images img {
          max-width: 100%;
          max-height: 100%;
          object-fit: contain;
        }
      </style>
      <div style="font-size: .6em">
        <h1>Using a Static Site Generator</h1>
      </div>
      <p>https://brutalist-web.design/</p>
      <div class="images-container">
        <div class="image-container">
          {{< media "pres/hugo.png" "https://gohugo.io/" "true" "height: 400px;margin-bottom: 20px;width:99%;" "">}}
        </div>
        <div class="image-container">
          {{< media "pres/xmin.png" "https://github.com/yihui/hugo-xmin" "true" "height: 400px;margin-bottom: 20px;width:99%;" "">}}
        </div>
      </div>
    </div>
  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<!-- Resource multiplexing -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-single-image">
      <style>
        .slide-single-image {
          display: flex;
          flex-direction: column;
          width: 100%;
          height: 100%;
          padding: 20px;
          box-sizing: border-box;
          text-align: center;
        }
        .slide-single-image .title {
          margin-bottom: 10px;
          width: 100%;
        }
        .slide-single-image .image-container {
          flex: 1;
          display: flex;
          justify-content: center;
          align-items: center;
          overflow: hidden;
        }
        .slide-single-image img {
          max-width: 80%;
          max-height: 80%;
          object-fit: contain;
        }
      </style>
      <div class="title">
        <h1 style="font-size: 1.6em;">New Client Sessions are Expensive</h1>
        <p>Resource multiplexing didn't come around until HTTP/2, and I don't have that</p>
      </div>
      <div class="image-container">
        {{< media "pres/webserver_diagram.png" "Web server architecture - By Ade56facc - Own work, CC BY-SA 4.0, https://commons.wikimedia.org/w/index.php?curid=112590977" "true" "height: 450px;width:99%;" "">}}
      </div>
    </div>
  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> |
    <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<!-- Hugo shortcodes for media inclusion -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content" style=" margin-top: -40px;">
    <div class="slide slide-code">
      <style>
      .slide-code {
        display: flex;
        flex-direction: column;
        width: 100%;
        height: 100%;
        box-sizing: border-box;
      }
      .slide-code .code-container {
        flex: 1;
        background: #2a2a2a;
        overflow: auto;
        font-family: monospace;
      }
      .slide-code pre {
        margin: 0;
        white-space: pre;
        font-size: 0.9em;
        line-height: 1.4;
      }
      </style>
      <h2 style="font-size: 1.4em; padding: 0; margin:0; margin-bottom:20px;">Custom Hugo Shortcode for Base64 Media</h2>
        <div class="code-container">
{{< highlight html >}}
<figure{{ if $figureStyle }}style="{{ $figureStyle }}"{{ end }}>

    {{- if in (slice "svg" "svg+xml") $mediaType -}}
        {{- $encodedSVG := $file.Content | base64Encode -}}
        <img src="data:image/svg+xml;base64,{{ $encodedSVG }}" ... />

    {{- else if in (slice "png" "jpeg" "gif" "webp") $mediaType -}}
        <img src="data:image/{{ $mediaType }};base64,{{ $file.Content | base64Encode }}" ... />

    {{- else if in (slice "webm" "mp4" "ogg") $mediaType -}}
        <video controls {{ if $mediaStyle }}style="{{ $mediaStyle }}"{{ end }}>
            <source src="data:video/{{ $mediaType }};base64,{{ $file.Content | base64Encode }}" ... >
        </video>

    {{- end -}}

    {{- if and (eq $showCaption "true") $alt }}<figcaption>{{ $alt }}</figcaption>{{ end }}

</figure>

// In the actual source files, I write
{{\< media "path/to/image.png" "custom alt text and figcaption">}}

{{< /highlight >}}

</div>
</div>

  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<!-- How load website? -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-section-divider">
      <style>
      .slide-section-divider {display: flex; flex-direction: column; justify-content: center; align-items: center; width: 100%; height: 100%; text-align: center; padding: 20px; box-sizing: border-box;}
      .slide-section-divider h1 {font-size: 3em; margin-bottom: 0;}
      .slide-section-divider .divider {width: 200px; height: 2px; margin: 20px auto; background: #444;}
      </style>
      <h1>How do I load the website onto the ESP32-S3?</h1>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a></footer>
</div>

<!-- Script to generate littlefs image and brotli-compress everything before going onto the ESP32-S3 -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content" style=" margin-top: -40px;">
    <div class="slide slide-code">
      <style>
      .slide-code {
        display: flex;
        flex-direction: column;
        width: 100%;
        height: 100%;
        box-sizing: border-box;
      }
      .slide-code .code-container {
        flex: 1;
        background: #2a2a2a;
        overflow: auto;
        font-family: monospace;
      }
      .slide-code pre {
        margin: 0;
        white-space: pre;
        font-size: 0.9em;
        line-height: 1.4;
      }
      </style>
      <h2 style="font-size: 1.4em; padding: 0; margin:0; margin-bottom:20px;">Converting Hugo Output into LittleFS Images</h2>
        <div class="code-container">
{{< highlight bash >}}
# Copy all files from SITE_DIR to OUTPUT_RESOURCES
echo "Copying files from $SITE_DIR to $OUTPUT_RESOURCES..."
cp -r "$SITE_DIR/"* "$OUTPUT_RESOURCES/"

#: Compress files with Brotli  
compress_with_brotli

#: Generate cache.txt, so the MCU knows what to cache  
"${REPO_ROOT}/scripts/generate-cachefile.sh"

#: Pre-check directory size  
check_directory_size

#: Package the resources directory into a LittleFS image  
mklittlefs -c "$OUTPUT_RESOURCES" -p 256 -b 4096 -s "$PARTITION_SIZE" "$IMAGE"

echo "LittleFS image created at $IMAGE"

{{< /highlight >}}

</div>
</div>

  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<!-- Showing the website, as it is -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-single-image">
      <style>
        .slide-single-image {
          display: flex;
          flex-direction: column;
          width: 100%;
          height: 100%;
          padding: 20px;
          box-sizing: border-box;
          text-align: center;
        }
        .slide-single-image .title {
          margin-bottom: 10px;
          width: 100%;
        }
        .slide-single-image .image-container {
          flex: 1;
          display: flex;
          justify-content: center;
          align-items: center;
          overflow: hidden;
        }
        .slide-single-image img {
          max-width: 80%;
          max-height: 80%;
          object-fit: contain;
        }
      </style>
      <div class="title">
        <h1 style="font-size: 1.6em;">It's a blog!</h1>
      </div>
      <div class="image-container">
        {{< media "pres/screenshott_blog.png" "A screenshot of the front page of the Microsite blog" "true" "height: 700px;width:99%;" "">}}
      </div>
    </div>
  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> |
    <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<!-- Funny how-to connect -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content" style="flex: 1; display: flex; justify-content: center; align-items: center; overflow: hidden; min-height: calc(100vh - 110px); box-sizing: border-box;">
    <div class="slide" style="width: 100%; height: 100%; box-sizing: border-box; display: flex; flex-direction: column; justify-content: center; align-items: center; padding: 20px; text-align: center;">
      <h1 style="font-size: 3em; margin-bottom: 30px; letter-spacing: 1px;">Instructions For Reading My Blog</h1>
      <ul style="font-size: 1.5em; line-height: 1.6; width: 80%; margin-left: 300px; letter-spacing: 0.5px;">
        <li style="margin-bottom: 15px; text-align: left; letter-spacing: 0.5px;">Come to my apartment</li>
        <li style="margin-bottom: 15px; text-align: left; letter-spacing: 0.5px;">Connect to the <code>brat wifi</code> network
        <ul style="margin-top: 5px; font-size: .7em;">
            <li>The password is <code>you want to guess</code></li>
          </ul></li>
        <li style="margin-bottom: 15px; text-align: left; letter-spacing: 0.5px;">Navigate to <code>192.168.1.181</code> in your favorite web browser</li>
        <li style="margin-bottom: 15px; text-align: left; letter-spacing: 0.5px;">You're ready to start reading!</li>
      </ul>
    </div>
  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<!-- Making it public (OpenWRT router stuff, can access through my router's Public IP)-->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-two-images">
      <style>
        .slide-two-images {
          display: flex;
          flex-direction: column;
          width: 100%;
          height: 100%;
          padding: 20px;
          box-sizing: border-box;
        }
        .slide-two-images .title {
          text-align: center;
          margin-bottom: 30px;
          width: 100%;
        }
        .slide-two-images .images-container {
          display: flex;
          flex: 1;
          width: 100%;
        }
        .slide-two-images .image-container {
          flex: 1;
          display: flex;
          justify-content: center;
          align-items: center;
          overflow: hidden;
        }
        .slide-two-images .image-container:first-child {
          margin-right: 20px;
        }
        .slide-two-images img {
          max-width: 100%;
          max-height: 100%;
          object-fit: contain;
        }
      </style>
      <div style="font-size: .6em">
        <h1>Publishing my Website</h1>
      </div>
      <div class="images-container">
        <div class="image-container">
          {{< media "pres/openwrtone.webp" "My OpenWRT One router, nicknamed 'Wendy'" "true" "height: 400px;margin-bottom: 20px;width:99%;" "">}}
        </div>
        <div class="image-container">
          {{< media "pres/portforward.png" "The actual documentation page on openwrt.org" "true" "height: 400px;margin-bottom: 20px;width:99%;" "">}}
        </div>
      </div>
    </div>
  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<!-- Making it public domain from Namecheap, Advanced DNS, -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-single-image">
      <style>
        .slide-single-image {
          display: flex;
          flex-direction: column;
          width: 100%;
          height: 100%;
          padding: 20px;
          box-sizing: border-box;
          text-align: center;
        }
        .slide-single-image .title {
          margin-bottom: 10px;
          width: 100%;
        }
        .slide-single-image .image-container {
          flex: 1;
          display: flex;
          justify-content: center;
          align-items: center;
          overflow: hidden;
        }
        .slide-single-image img {
          max-width: 80%;
          max-height: 80%;
          object-fit: contain;
        }
      </style>
      <div class="title">
        <h1 style="font-size: 1.6em;">Now, you can read the blog online!</h1>
      </div>
      <div class="image-container">
        {{< media "pres/ip.png" "A screenshot of the front page of the Microsite blog" "false" "max-height: 700px;width:99%;" "">}}
      </div>
    </div>
  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> |
    <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-single-image">
      <style>
        .slide-single-image {
          display: flex;
          flex-direction: column;
          width: 100%;
          height: 100%;
          padding: 20px;
          box-sizing: border-box;
          text-align: center;
        }
        .slide-single-image .title {
          margin-bottom: 10px;
          width: 100%;
        }
        .slide-single-image .image-container {
          flex: 1;
          display: flex;
          justify-content: center;
          align-items: center;
          overflow: hidden;
        }
        .slide-single-image img {
          max-width: 80%;
          max-height: 80%;
          object-fit: contain;
        }
      </style>
      <div class="title">
        <h1 style="font-size: 1.6em;">Getting a Domain from Namecheap</h1>
      </div>
      <div class="image-container">
        {{< media "pres/namecheap.jpg" "A screenshot of the front page of the Microsite blog" "false" "height: 400px;width:99%;" "">}}
      </div>
    </div>
  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> |
    <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-single-image">
      <style>
        .slide-single-image {
          display: flex;
          flex-direction: column;
          width: 100%;
          height: 100%;
          padding: 20px;
          box-sizing: border-box;
          text-align: center;
        }
        .slide-single-image .title {
          margin-bottom: 10px;
          width: 100%;
        }
        .slide-single-image .image-container {
          flex: 1;
          display: flex;
          justify-content: center;
          align-items: center;
          overflow: hidden;
        }
        .slide-single-image img {
          max-width: 80%;
          max-height: 80%;
          object-fit: contain;
        }
      </style>
      <div class="title">
        <h1 style="font-size: 1.6em;">Indecision</h1>
      </div>
      <div class="image-container">
        {{< media "pres/domains.png" "A screenshot of the front page of the Microsite blog" "false" "width: 500px;" "">}}
      </div>
    </div>
  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> |
    <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-single-image">
      <style>
        .slide-single-image {
          display: flex;
          flex-direction: column;
          width: 100%;
          height: 100%;
          padding: 20px;
          box-sizing: border-box;
          text-align: center;
        }
        .slide-single-image .title {
          margin-bottom: 10px;
          width: 100%;
        }
        .slide-single-image .image-container {
          flex: 1;
          display: flex;
          justify-content: center;
          align-items: center;
          overflow: hidden;
        }
        .slide-single-image img {
          max-width: 80%;
          max-height: 80%;
          object-fit: contain;
        }
      </style>
      <div class="title">
        <h1 style="font-size: 1.6em;">Hooray!</h1>
      </div>
      <div class="image-container">
        {{< media "pres/insecure.png" "A screenshot of the front page of the Microsite blog" "false" "max-height: 500px;width:99%;" "">}}
      </div>
    </div>
  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> |
    <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>

<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-single-image">
      <style>
        .slide-single-image {
          display: flex;
          flex-direction: column;
          width: 100%;
          height: 100%;
          padding: 20px;
          box-sizing: border-box;
          text-align: center;
        }
        .slide-single-image .title {
          margin-bottom: 10px;
          width: 100%;
        }
        .slide-single-image .image-container {
          flex: 1;
          display: flex;
          justify-content: center;
          align-items: center;
          overflow: hidden;
        }
        .slide-single-image img {
          max-width: 80%;
          max-height: 80%;
          object-fit: contain;
        }
      </style>
      <div class="image-container">
        {{< media "pres/sadinsecure.png" "A screenshot of the front page of the Microsite blog" "false" "max-height: 500px;width:99%;" "">}}
      </div>
    </div>
  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="https://joni-on-micro.site">joni-on-micro.site</a> |
    <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a>
  </footer>
</div>
<!-- Am I done NOW? -->
<!-- Lock icon -->
<!-- LetsEncrypt and cert-loading directly into the flash -->
<!-- I'm done.... for now -->
<!-- Performance measurements and conclusion -->

</body>
</html>
