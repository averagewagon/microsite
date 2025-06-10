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

<!-- Fuzzy Project Parameters -->

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

{{< media "pres/Beige_Power_Macintosh_G3_Minitower.jpg" "Image Source: Wikipedia" "true" "max-height: 160px;" "margin-right: 160px">}}

</div>
</div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="https://joni-on-micro.site">joni-on-micro.site</a> | <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon/microsite">github.com/averagewagon/microsite</a></footer>
</div>

</body>
</html>
