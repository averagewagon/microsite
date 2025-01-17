#!/bin/sh

# Enable strict error handling
set -eu

# Run shellcheck on the current script if available
if command -v shellcheck >/dev/null 2>&1; then
    shellcheck "$0" || echo "Shellcheck found issues."
else
    echo "Shellcheck not found, skipping..."
fi

# Function to print an error message and exit
error() {
    printf "\033[31mError: %s\033[0m\n" "$1" >&2
    exit 1
}

# Define folder paths
CONTENT_DIR="content"
IMAGES_DIR="images"
STYLES_DIR="styles"
TEMPLATES_DIR="templates"
CODE_DIR="code"
BUILD_DIR="build"
OUTPUT_DIR="$BUILD_DIR/output"

# Set up the build environment by creating clean directories
echo "Setting up build environment..."
rm -rf "$BUILD_DIR" || error "Failed to remove build directory"
mkdir -p "$OUTPUT_DIR" || error "Failed to create output directory"

# Copy HTML files while preserving sub-directory structure
echo "Copying HTML files..."
rsync -a --include '*/' --include '*.html' --exclude '*' "$CONTENT_DIR/" "$BUILD_DIR/" || error "Failed to copy HTML files"

# Perform dynamic template substitutions for HTML files
echo "Performing template substitutions..."
find "$BUILD_DIR" -type f -name "*.html" | while IFS= read -r file; do
    # Substitute template placeholders like {{TEMPLATE:header.html}}
    grep -o "{{TEMPLATE:[^}]*}}" "$file" | while IFS= read -r template; do
        template_name=$(echo "$template" | sed -e 's/{{TEMPLATE:\(.*\)}}/\1/')
        template_file="$TEMPLATES_DIR/$template_name"
        [ -f "$template_file" ] || error "Template file not found: $template_file"
        sed -i "/$template/{
        r $template_file
        d
    }" "$file" || error "Failed to substitute template $template in $file"
    done

    # Substitute CSS styles like {{STYLE:main.css}}
    grep -o "{{STYLE:[^}]*}}" "$file" | while IFS= read -r style; do
        style_name=$(echo "$style" | sed -e 's/{{STYLE:\(.*\)}}/\1/')
        style_file="$STYLES_DIR/$style_name"
        [ -f "$style_file" ] || error "Style file not found: $style_file"

        # Read the entire CSS file content into a variable
        css_content=$(cat "$style_file")

        # Replace the placeholder with <style> tags and the CSS content
        {
            while IFS= read -r line; do
                case "$line" in
                *"$style"*)
                    # Replace the placeholder with the <style> tag
                    echo "<style>$css_content</style>"
                    ;;
                *)
                    echo "$line"
                    ;;
                esac
            done <"$file"
        } >"$file.tmp" || error "Failed to substitute style $style in $file"

        # Replace the original file with the updated file
        mv "$file.tmp" "$file"
    done

    # Substitute image placeholders like <!-- {{IMAGE:logos/logo.webp}} --> with base64-encoded data
    grep -o "<!-- {{IMAGE:[^}]*}} -->" "$file" | while IFS= read -r img_comment; do
        img_path=$(echo "$img_comment" | sed -e 's/<!-- {{IMAGE:\(.*\)}} -->/\1/')
        full_img_path="$IMAGES_DIR/$img_path"
        [ -f "$full_img_path" ] || error "Image not found: $full_img_path"
        base64_data=$(base64 -w 0 "$full_img_path") || error "Failed to encode $img_path"
        img_mime_type=$(basename "$img_path" | awk -F. '{print $NF}')
        sed -i "s~$img_comment~data:image/$img_mime_type;base64,$base64_data~g" "$file" || error "Failed to substitute image $img_comment in $file"
    done

    # echo "Performing code block substitutions..."
    # find "$BUILD_DIR" -type f -name "*.html" >/tmp/html_files
    # while IFS= read -r file; do
    #     grep -o "{{CODE:[^}]*}}" "$file" >/tmp/code_placeholders
    #     while IFS= read -r code_placeholder; do
    #         # Extract file path from the placeholder
    #         code_file=$(echo "$code_placeholder" | sed -e 's/{{CODE:\(.*\)}}/\1/')
    #         full_code_path="$CODE_DIR/$code_file"

    #         # Check if the code file exists
    #         if [ ! -f "$full_code_path" ]; then
    #             error "Code file not found: $full_code_path"
    #         fi

    #         # Generate a valid filename for the debug output
    #         pygment_output_path="$BUILD_DIR/debug_$(echo "$code_file" | tr '/' '_').txt"

    #         # Generate highlighted code using Pygments (snippet only)
    #         highlighted_code=$(pygmentize -g -f html -O nowrap "$full_code_path") || error "Failed to highlight code $code_file"

    #         # Save the raw Pygments output for debugging
    #         echo "$highlighted_code" >"$pygment_output_path"

    #         # Escape special characters in the highlighted code for substitution
    #         escaped_code=$(printf '%s' "$highlighted_code" | sed 's/[&/\]/\\&/g')

    #         # Replace the placeholder with the highlighted code
    #         awk -v placeholder="$code_placeholder" -v replacement="$escaped_code" '
    #     BEGIN { RS=""; ORS="\n\n" } # Treat the file as a single record
    #     { gsub(placeholder, replacement) } 1' "$file" >"$file.tmp" || error "Failed to substitute code $code_placeholder in $file"

    #         # Replace the original file with the updated file
    #         mv "$file.tmp" "$file"
    #     done </tmp/code_placeholders
    # done </tmp/html_files

done

# Minify HTML files to reduce file size
echo "Minifying HTML..."
find "$BUILD_DIR" -type f -name "*.html" -exec html-minifier --collapse-whitespace --remove-comments --minify-css true --minify-js true -o {} {} \; || error "HTML minification failed"

# Placeholder for PurgeCSS integration to remove unused styles from CSS
# TODO: Integrate PurgeCSS here if needed.

# Compress HTML files using Brotli and preserve sub-directory structure
echo "Compressing HTML files with Brotli..."
find "$BUILD_DIR" -type f -name "*.html" | while IFS= read -r file; do
    output_path="$OUTPUT_DIR/$(dirname "${file#"$BUILD_DIR"/}")"
    mkdir -p "$output_path"

    # Copy the original minified HTML file to the output directory
    cp "$file" "$output_path/$(basename "$file")" || error "Failed to copy HTML file for fallback"

    # Create the Brotli-compressed version in the same output directory
    brotli --best -o "$output_path/$(basename "$file").br" "$file" || error "Brotli compression failed for $file"
done

# Copy non-HTML files from content/ to the output directory while preserving structure
echo "Copying non-HTML files to output..."
rsync -a --exclude '*.html' "$CONTENT_DIR/" "$OUTPUT_DIR/" || error "Failed to copy non-HTML files"

# Final cleanup and success message
echo "Build complete! Output available in $OUTPUT_DIR"
