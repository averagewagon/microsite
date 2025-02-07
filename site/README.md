# Microsite: Website-Secific Code

This site is built using Hugo, a fast and efficient static site generator. The
design is optimized for hosting on an ESP32-S3 microcontroller, which has
limited processing power and storage. To minimize HTTP requests and avoid the
overhead of setting up HTTPS sessions, each page is fully self-contained,
embedding all assets directly.

## Design Constraints & Philosophy

- Each page is a single HTML file with all images, styles, and media embedded
  via Base64 encoding.
- No external dependencies – No external CSS, JavaScript, or images; everything
  is processed at build time.
- Optimized for bandwidth and storage – Pages are minified and structured for
  low-power devices.
- Minimalist yet functional – No unnecessary features; content and efficiency
  take priority.

## Features

### Blog System (`/blog/`)

- All posts are listed with automatic tag filtering.
- RSS feed available at `/blog/index.xml`.
- Previous/Next article navigation for improved reading flow.

### Self-Contained Asset Handling

- Images, videos, and SVGs are Base64-encoded and embedded inline.
- Custom Hugo shortcode (`media.html`) for handling assets dynamically.

### Performance Enhancements

- Minified inline CSS for reduced page size.
- Table of Contents (optional per post) via `toc: true` in front matter.
- Scrollable tables for mobile-friendly display.
- Consistent visited/unvisited link colors, excluding navigation.

## Custom Media Shortcodes

To keep images, videos, and SVGs fully self-contained, a custom Hugo shortcode  
(`media.html`) was implemented. It Base64-encodes and embeds assets within the
page.

### Usage

```md
{{< media "microsite.svg" "This is an SVG" >}}
{{< media "new-post.png" "This is a PNG image">}}
{{< media "sample.webm" "A webm video" >}}
```

### Arguments

| Argument                      | Description                                                                                       | Default      |
| ----------------------------- | ------------------------------------------------------------------------------------------------- | ------------ |
| 1. `filename` _(Required)_    | The media filename, located in `assets/media/`.                                                   | _(Required)_ |
| 2. `altText` _(Optional)_     | Alternative text for accessibility and captions.                                                  | `""` (empty) |
| 3. `showCaption` _(Optional)_ | `"true"`: Wraps media in a `<figure>` with `<figcaption>`. <br> `"false"`: Embeds media directly. | `"true"`     |
| 4. `mediaStyle` _(Optional)_  | CSS styles applied to `<img>` or `<video>`.                                                       | `""` (none)  |
| 5. `figureStyle` _(Optional)_ | CSS styles applied to `<figure>` (only if `showCaption` is `"true"`).                             | `""` (none)  |

### How It Works

The shortcode fetches the file from `assets/media/`, determines its type, and:

- Embeds it as a Base64-encoded `<img>` for images (PNG, JPG, SVG, WebP).
- Embeds it as a Base64-encoded `<video>` for videos (WebM, MP4, OGG).

This ensures every page is fully self-contained, reducing the number of HTTP
requests required.

## Running the Site Locally

To preview the site: `sh hugo server` Access it at `http://localhost:1313/`.

To generate a production build: `sh hugo --minify `

## Credits

This site was adapted from Hugo XMin, a minimalist theme by Yihui Xie, which
aligned well with the goal of creating a lightweight, fully static site.

- Original theme repository: [Hugo XMin](https://github.com/yihui/hugo-xmin)
- Author: Yihui Xie ([Website](https://yihui.org))
