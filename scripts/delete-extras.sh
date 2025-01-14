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

# Paths and variables
REPO_ROOT=$(git rev-parse --show-toplevel)
BUILD_DIR="${REPO_ROOT}/site/build/output"
WHITELIST_FILE="${REPO_ROOT}/scripts/whitelist.txt"
TEMP_WHITELIST=$(mktemp)

# Ensure the build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    error "$BUILD_DIR does not exist. Ensure the site is built before running this script."
fi

# Ensure the whitelist file exists
if [ ! -f "$WHITELIST_FILE" ]; then
    error "Whitelist file not found at $WHITELIST_FILE"
fi

# Initialize the temporary whitelist with contents of whitelist file
cat "$WHITELIST_FILE" >"$TEMP_WHITELIST"

# Debugging: Show initial whitelist
echo "Initial whitelist from file:"
cat "$TEMP_WHITELIST"

# Automatically whitelist all HTML files
find "$BUILD_DIR" -type f -name "*.html" | while read -r html_file; do
    # Normalize path relative to build directory and add to whitelist
    relative_file=$(realpath --relative-to="$BUILD_DIR" "$html_file")
    echo "$relative_file" >>"$TEMP_WHITELIST"
    echo "HTML whitelisted: $relative_file"
done

# Grep-based whitelisting: Match filenames in HTML contents
find "$BUILD_DIR" -type f -name "*.html" | while read -r html_file; do
    # Loop through all files in the build directory
    find "$BUILD_DIR" -type f | while read -r file; do
        filename=$(basename "$file")
        # Check if the filename appears anywhere in the HTML file
        if grep -q "$filename" "$html_file"; then
            relative_file=$(realpath --relative-to="$BUILD_DIR" "$file")
            echo "$relative_file" >>"$TEMP_WHITELIST"
            echo "Automatically whitelisted: $relative_file (found in $html_file)"
        fi
    done
done

# Ensure whitelist is unique and debug final whitelist
sort -u "$TEMP_WHITELIST" -o "$TEMP_WHITELIST"
echo "Final whitelist:"
cat "$TEMP_WHITELIST"

# Remove files not in the whitelist
find "$BUILD_DIR" -type f | while read -r file; do
    # Normalize path relative to build directory
    relative_file=$(realpath --relative-to="$BUILD_DIR" "$file")

    # Debugging: Check if the file is in the whitelist
    if ! grep -qx "$relative_file" "$TEMP_WHITELIST"; then
        echo "Deleting unused file: $relative_file"
        rm -f "$file"
    else
        echo "Retained: $relative_file"
    fi
done

# Remove empty folders
find "$BUILD_DIR" -type d -empty -delete

echo "Cleanup complete. All unreferenced files and empty folders have been removed."

# Clean up temporary file
rm -f "$TEMP_WHITELIST"
