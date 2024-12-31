#!/bin/sh

# Enable strict error handling for better script durability
set -eu

# Run shellcheck on the current script if available
command -v shellcheck >/dev/null 2>&1 && shellcheck "$0" || echo "Shellcheck not found, skipping..."

# Determine the root of the Git repository
REPO_ROOT=$(git rev-parse --show-toplevel)

# Set paths
SITE_DIR="${REPO_ROOT}/site"
OUTPUT_IMAGE="${REPO_ROOT}/site/build/littlefs_image.bin"

# Function to print an error message and exit
error() {
    echo "Error: $1" >&2
    exit 1
}

# Check for mklittlefs
if ! command -v mklittlefs >/dev/null 2>&1; then
    error "mklittlefs is not installed or not in PATH. Install it from: https://github.com/earlephilhower/mklittlefs"
fi

# Check if site/ directory exists
if [ ! -d "$SITE_DIR" ]; then
    error "$SITE_DIR does not exist. Create it and add your site files."
fi

# Package the site directory into a LittleFS image
echo "Packaging $SITE_DIR into $OUTPUT_IMAGE..."
rm -rf "${REPO_ROOT}/site/build" || true
mkdir -p "${REPO_ROOT}/site/build"
mklittlefs -c "$SITE_DIR" -p 256 -b 4096 -s 327680 "$OUTPUT_IMAGE"
echo "LittleFS image created at $OUTPUT_IMAGE"

# Erase the LittleFS partition before flashing to avoid leftover data
echo "Erasing LittleFS partition at offset 0x110000..."
esptool.py --port /dev/ttyUSB0 erase_region 0x110000 0x50000 || error "Failed to erase LittleFS partition."

# Flash the LittleFS image to the appropriate partition
echo "Flashing $OUTPUT_IMAGE to LittleFS partition at offset 0x110000..."
esptool.py --port /dev/ttyUSB0 --baud 115200 write_flash 0x110000 "$OUTPUT_IMAGE" || error "Failed to flash LittleFS image to ESP32."

echo "LittleFS image successfully flashed!"
