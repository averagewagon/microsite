### **Microsite Build Documentation**

#### **Folder Structure**

```
content/    # All HTML files to be served
images/     # Image files (e.g., PNG, JPG, WebP)
styles/     # CSS files
templates/  # Shared HTML templates (e.g., header.html, footer.html)
build/      # Temporary build directory (generated during build)
build/output/  # Final output directory
```

---

#### **Template Strings**

Use these template strings in HTML files for automated substitutions:

1. **Inject CSS Styles**:

   ```html
   <!-- {{STYLE:main.css}} -->
   ```

   Replaced with `<style>` containing the full contents of `styles/main.css`.

2. **Inject Templates**:

   ```html
   <!-- {{TEMPLATE:header.html}} -->
   ```

   Replaced with the contents of `templates/header.html`.

3. **Embed Images as Base64**:
   ```html
   <img
     src="data:image/png;base64,<!-- {{IMAGE:logos/favicon.png}} -->"
     alt="Logo"
   />
   ```
   Replaced with Base64-encoded content of `images/logos/favicon.png`.

---

#### **Image Compression Workflow**

To prepare highly-compressed WebP images, use the `compress-image.sh` script.
This script compresses an input image three times to optimize size.

**Steps**:

1. **Run the Script**:

   ```sh
   ./compress-image.sh <input_file> <output_prefix>
   ```

   Example:

   ```sh
   ./compress-image.sh images/blog/image.png images/blog/compressed_image
   ```

2. **Final Compressed File**: The final compressed image will be saved as:

   ```
   images/blog/compressed_image_2.webp
   ```

3. **Use the Compressed Image in HTML**: Update the `{{IMAGE:...}}` placeholder
   to use the final compressed file:
   ```html
   <img
     src="data:image/webp;base64,<!-- {{IMAGE:blog/compressed_image_2.webp}} -->"
     alt="Optimized Image"
   />
   ```

---

#### **Build Steps**

1. **Run the Build Script**:

   ```sh
   ./generate-site.sh
   ```

   - Copies `content/*.html` to `build/`.
   - Substitutes template strings in HTML.
   - Minifies HTML.
   - Outputs both uncompressed and Brotli-compressed `.html.br` files in
     `build/output/`.

2. **Verify Output**:
   ```sh
   tree build/output/
   ```
   Ensure both `.html` and `.html.br` versions exist, and the structure mirrors
   `content/`.

---

#### **Example Workflow**

1. **Add New HTML File**: Place `content/blog/new-post.html`:

   ```html
   <!DOCTYPE html>
   <html lang="en">
     <head>
       <title>My New Blog Post</title>
       <link
         rel="icon"
         href="<!-- {{IMAGE:logos/favicon.ico}} -->"
         type="image/x-icon"
       />
       <!-- {{STYLE:main.css}} -->
     </head>
     <body>
       <!-- {{TEMPLATE:header.html}} -->
       <main>
         <h1>My New Post</h1>
         <p>Hello, world!</p>
         <img
           src="<!-- {{IMAGE:blog/compressed_image_2.webp}} -->"
           alt="Blog Image"
         />
       </main>
       <!-- {{TEMPLATE:footer.html}} -->
     </body>
   </html>
   ```

2. **Add Supporting Assets**:

   - `images/blog/image.png` → Compress with `compress-image.sh`
   - `styles/main.css`
   - `templates/header.html`
   - `templates/footer.html`

3. **Rebuild**:
   ```sh
   ./generate-site.sh
   ```

---

#### **Notes**

- Subdirectories are allowed for all folders.
- Only modify `content/`, `images/`, `styles/`, and `templates/`.
- Ensure template strings are correctly formatted (`<!-- {{...}} -->`).
- Brotli-compressed files are used for deployment, while uncompressed files
  serve as fallbacks.
