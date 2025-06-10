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

<!-- 1. Title Slide -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-title">
      <style>.slide-title {display: flex; flex-direction: column; justify-content: center; align-items: center; width: 100%; height: 100%; padding: 20px; text-align: center; box-sizing: border-box;}</style>
      <h1>Presentation Title</h1>
      <p>Subtitle or presenter name</p>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon">github.com/averagewagon</a></footer>
</div>

<!-- 2. Two Column Layout -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-two-column">
      <style>
      .slide-two-column {display: flex; width: 100%; height: 100%; padding: 20px; box-sizing: border-box;}
      .slide-two-column .column {flex: 1; padding: 10px; display: flex; flex-direction: column; justify-content: center; overflow: auto;}
      .slide-two-column .column:first-child {margin-right: 20px;}
      </style>
      <div class="column">
        <h2>Column 1 Title</h2>
        <p>Content for the first column</p>
        <ul><li>Item 1</li><li>Item 2</li><li>Item 3</li></ul>
      </div>
      <div class="column">
        <h2>Column 2 Title</h2>
        <p>Content for the second column</p>
        <ol><li>First point</li><li>Second point</li><li>Third point</li></ol>
      </div>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon">github.com/averagewagon</a></footer>
</div>

<!-- 3. Image and Text Layout -->
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
        <img src="data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSI2MDAiIGhlaWdodD0iNDAwIiB2aWV3Qm94PSIwIDAgNjAwIDQwMCI+PHJlY3Qgd2lkdGg9IjYwMCIgaGVpZ2h0PSI0MDAiIGZpbGw9IiNmZmZmZmYiLz48L3N2Zz4=" alt="Placeholder image">
      </div>
      <div class="text-container">
        <h2>Image Description</h2>
        <p>Text content that relates to the image</p>
        <ul><li>Key point about the image</li><li>Additional information</li><li>Final observation</li></ul>
      </div>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon">github.com/averagewagon</a></footer>
</div>

<!-- 4. Bullet Points Only -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-bullets">
      <style>
      .slide-bullets {display: flex; flex-direction: column; justify-content: center; width: 80%; height: 100%; padding: 20px; box-sizing: border-box;}
      .slide-bullets h2 {text-align: center; margin-bottom: 30px;}
      .slide-bullets ul {font-size: 1.2em; line-height: 1.8;}
      .slide-bullets li {margin-bottom: 15px;}
      </style>
      <h2>Key Points</h2>
      <ul>
        <li>First main point of your presentation</li>
        <li>Second important concept to cover</li>
        <li>Third critical idea to communicate</li>
        <li>Additional supporting information</li>
        <li>Final takeaway message</li>
      </ul>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon">github.com/averagewagon</a></footer>
</div>

<!-- 5. Full Image Slide -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-full-image">
      <style>
      .slide-full-image {width: 100%; height: 100%; position: relative; display: flex; justify-content: center; align-items: center; overflow: hidden;}
      .slide-full-image .image-container {width: 100%; height: 100%; display: flex; justify-content: center; align-items: center;}
      .slide-full-image img {max-width: 100%; max-height: 100%; object-fit: contain;}

      </style>
      <div class="image-container">
        <img src="data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSI2MDAiIGhlaWdodD0iNDAwIiB2aWV3Qm94PSIwIDAgNjAwIDQwMCI+PHJlY3Qgd2lkdGg9IjYwMCIgaGVpZ2h0PSI0MDAiIGZpbGw9IiNmZmZmZmYiLz48L3N2Zz4=" alt="Full slide image">
      </div>
      <div class="caption">Descriptive caption for the image</div>
    </div>

  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon">github.com/averagewagon</a></footer>
</div>

<!-- 6. Three Column Layout -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-three-column">
      <style>
      .slide-three-column {display: flex; width: 100%; height: 100%; padding: 20px; box-sizing: border-box;}
      .slide-three-column .column {flex: 1; padding: 10px; display: flex; flex-direction: column; justify-content: center;}
      .slide-three-column .column:not(:last-child) {margin-right: 15px;}
      </style>
      <div class="column">
        <h3>Column 1</h3>
        <p>Content for the first column</p>
      </div>
      <div class="column">
        <h3>Column 2</h3>
        <p>Content for the second column</p>
      </div>
      <div class="column">
        <h3>Column 3</h3>
        <p>Content for the third column</p>
      </div>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon">github.com/averagewagon</a></footer>
</div>

<!-- 7. Text with Side Image -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-text-side-image">
      <style>
      .slide-text-side-image {display: flex; width: 100%; height: 100%; padding: 20px; box-sizing: border-box;}
      .slide-text-side-image .main-content {flex: 3; display: flex; flex-direction: column; justify-content: center; margin-right: 20px; overflow: auto;}
      .slide-text-side-image .side-image {flex: 1; display: flex; justify-content: center; align-items: center; overflow: hidden;}
      .slide-text-side-image img {max-width: 100%; max-height: 80%; object-fit: contain;}
      </style>
      <div class="main-content">
        <h2>Main Content Title</h2>
        <p>Primary content goes here with more detailed information</p>
        <ul><li>Key point 1</li><li>Key point 2</li><li>Key point 3</li></ul>
      </div>
      <div class="side-image">
        <img src="data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyMDAiIGhlaWdodD0iMjAwIiB2aWV3Qm94PSIwIDAgMjAwMjAwIj48cmVjdCB3aWR0aD0iMjAwIiBoZWlnaHQ9IjIwMCIgZmlsbD0iI2ZmMDAwMCIvPjwvc3ZnPg==" alt="Side image">
      </div>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon">github.com/averagewagon</a></footer>
</div>

<!-- 8. Centered Content with Subpoints -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-centered-subpoints">
      <style>
      .slide-centered-subpoints {display: flex; flex-direction: column; justify-content: center; align-items: center; width: 80%; height: 100%; padding: 20px; box-sizing: border-box; text-align: center;}
      .slide-centered-subpoints .subpoints {display: flex; flex-direction: row; justify-content: space-between; width: 100%; margin-top: 30px;}
      .slide-centered-subpoints .subpoint {flex: 1; padding: 10px;}
      .slide-centered-subpoints .subpoint:not(:last-child) {margin-right: 20px;}
      </style>
      <h2>Main Concept</h2>
      <p>Brief explanation of the main concept</p>
      <div class="subpoints">
        <div class="subpoint">
          <h3>Subpoint 1</h3>
          <p>Brief description</p>
        </div>
        <div class="subpoint">
          <h3>Subpoint 2</h3>
          <p>Brief description</p>
        </div>
        <div class="subpoint">
          <h3>Subpoint 3</h3>
          <p>Brief description</p>
        </div>
      </div>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon">github.com/averagewagon</a></footer>
</div>

<!-- 9. Section Divider Slide -->
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
      <h1>Section Title</h1>
      <div class="divider"></div>
      <p>Brief description of what this section will cover</p>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon">github.com/averagewagon</a></footer>
</div>

<!-- 10. Quote Slide -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-quote">
      <style>
      .slide-quote {display: flex; flex-direction: column; justify-content: center; align-items: center; width: 80%; height: 100%; padding: 20px; box-sizing: border-box; text-align: center;}
      .slide-quote .quote-text {font-size: 1.8em; margin-bottom: 40px; line-height: 1.5;}
      .slide-quote .quote-author {align-self: flex-end; font-weight: bold;}
      </style>
      <div class="quote-text">
        <p>"This is an inspiring quote that relates to your presentation topic. It should be meaningful and thought-provoking for your audience."</p>
      </div>
      <div class="quote-author">
        <p>— Author Name</p>
      </div>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon">github.com/averagewagon</a></footer>
</div>

<!-- 11. Process Flow Slide -->
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
      .slide-process .process-step {width: 80%; padding: 15px; margin-bottom: 20px; display: flex; align-items: center;}
      .slide-process .step-number {width: 30px; height: 30px; border-radius: 50%; display: flex; justify-content: center; align-items: center; margin-right: 15px; background: #333;}
      .slide-process .step-content {flex: 1;}
      .slide-process .connector {height: 20px; width: 2px; margin-left: 15px; background: #444;}
      .slide-process .title {margin-bottom: 30px; font-size: 1.8em; text-align: center;}
      </style>
      <div class="title">Process Flow</div>
      <div class="process-container">
        <div class="process-step">
          <div class="step-number">1</div>
          <div class="step-content">First step in the process</div>
        </div>
        <div class="connector"></div>
        <div class="process-step">
          <div class="step-number">2</div>
          <div class="step-content">Second step in the process</div>
        </div>
        <div class="connector"></div>
        <div class="process-step">
          <div class="step-number">3</div>
          <div class="step-content">Third step in the process</div>
        </div>
        <div class="connector"></div>
        <div class="process-step">
          <div class="step-number">4</div>
          <div class="step-content">Final step in the process</div>
        </div>
      </div>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon">github.com/averagewagon</a></footer>
</div>

<!-- 12. Comparison Slide -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-comparison">
      <style>
      .slide-comparison {display: flex; flex-direction: column; width: 100%; height: 100%; padding: 20px; box-sizing: border-box;}
      .slide-comparison .comparison-container {flex: 1; display: flex; flex-direction: row; justify-content: space-around; width: 100%;}
      .slide-comparison .comparison-item {width: 45%; padding: 15px; display: flex; flex-direction: column; justify-content: center; align-items: center;}
      .slide-comparison .comparison-item h3 {margin-bottom: 20px;}
      .slide-comparison .comparison-item img {width: 80%; height: 60%; object-fit: contain; margin-bottom: 20px; background: #444;}
      .slide-comparison .title {margin-bottom: 30px; font-size: 1.8em; text-align: center;}
      </style>
      <div class="title">Comparison of Two Concepts</div>
      <div class="comparison-container">
        <div class="comparison-item">
          <h3>Concept A</h3>
          <img src="data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyMDAiIGhlaWdodD0iMTUwIiB2aWV3Qm94PSIwIDAgMjAwMTUwIj48cmVjdCB3aWR0aD0iMjAwIiBoZWlnaHQ9IjE1MCIgZmlsbD0iI2ZmMDAwMCIvPjwvc3ZnPg==" alt="Concept A">
          <p>Description of Concept A and its key characteristics.</p>
        </div>
        <div class="comparison-item">
          <h3>Concept B</h3>
          <img src="data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyMDAiIGhlaWdodD0iMTUwIiB2aWV3Qm94PSIwIDAgMjAwMTUwIj48cmVjdCB3aWR0aD0iMjAwIiBoZWlnaHQ9IjE1MCIgZmlsbD0iI2RmMDAwMCIvPjwvc3ZnPg==" alt="Concept B">
          <p>Description of Concept B and its key characteristics.</p>
        </div>
      </div>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon">github.com/averagewagon</a></footer>
</div>

<!-- 13. Code Slide -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-code">
      <style>
      .slide-code {display: flex; flex-direction: column; width: 100%; height: 100%; padding: 20px; box-sizing: border-box;}
      .slide-code .code-container {flex: 1; background: #2a2a2a; border-radius: 5px; padding: 20px; overflow: auto; font-family: monospace;}
      .slide-code pre {margin: 0; white-space: pre-wrap; font-size: 0.9em; line-height: 1.4;}
      </style>
      <h2>Code Example</h2>
      <div class="code-container">
        <pre>
function exampleFunction(param1, param2) {
   // This is a code example
   if (param1 > param2) {
      return param1;
   } else {
      return param2;
   }
}</pre>
      </div>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon">github.com/averagewagon</a></footer>
</div>

<!-- 13.5 Code Slide with proper Hugo code block -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
    <hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-code">
      <style>
      .slide-code {
        display: flex;
        flex-direction: column;
        width: 100%;
        height: 100%;
        padding: 20px;
        box-sizing: border-box;
      }
      .slide-code .code-container {
        flex: 1;
        background: #2a2a2a;
        border-radius: 5px;
        padding: 20px;
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
      <h2>Python HTTP Server Example</h2>
      <div class="code-container">
        {{< highlight python >}}
from http.server import SimpleHTTPRequestHandler, HTTPServer

class MyHandler(SimpleHTTPRequestHandler): def do_GET(self):
self.send_response(200) self.send_header("Content-type", "text/html")
self.end_headers() self.wfile.write(b"Hello from Python!")

server = HTTPServer(("0.0.0.0", 8000), MyHandler) print("Serving on port
8000...") server.serve_forever() {{< /highlight >}} </div> </div>

  </div>
  <footer class="slide-footer">
    Teardown 2025 - Presentation<br>
    <a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> |
    <a href="https://github.com/averagewagon">github.com/averagewagon</a>
  </footer>
</div>

<!-- 14. Contact/Closing Slide -->
<div class="slide-container">
  <header class="slide-header">
    <h2><a href="/">Joni on Microsite</a></h2>
    <span class="subtitle"><a href="/about-the-microsite">This website runs on an MCU</a></span>
<hr/>
  </header>
  <div class="slide-content">
    <div class="slide slide-contact">
      <style>
      .slide-contact {display: flex; flex-direction: column; justify-content: center; align-items: center; width: 100%; height: 100%; padding: 20px; text-align: center; box-sizing: border-box;}
      .slide-contact h1 {font-size: 3em; margin-bottom: 20px;}
      .slide-contact .contact-info {margin-top: 40px; display: flex; flex-direction: column; align-items: center;}
      .slide-contact .contact-item {margin-bottom: 15px;}
      </style>
      <h1>Thank You</h1>
      <p>Questions?</p>
      <div class="contact-info">
        <div class="contact-item">Joni Hendrickson</div>
        <div class="contact-item">hendrickson@joni.site</div>
        <div class="contact-item">github.com/averagewagon</div>
      </div>
    </div>
  </div>
  <footer class="slide-footer">Teardown 2025 - Presentation<br><a href="mailto:hendrickson@joni.site">hendrickson@joni.site</a> | <a href="https://github.com/averagewagon">github.com/averagewagon</a></footer>
</div>

</body>
</html>
