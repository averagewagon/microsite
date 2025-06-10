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

<!-- Extra slide (to make sure things aren't borked -->
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

<!-- Extra slide -->
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

</body>
</html>
