#!/bin/sh

# Enable strict error handling for better script durability
set -eu

# Run shellcheck on the current script if available
if command -v shellcheck >/dev/null 2>&1; then
    shellcheck "$0" || echo "Shellcheck found issues."
else
    echo "Shellcheck not found, skipping..."
fi

# Determine the root of the Git repository
REPO_ROOT=$(git rev-parse --show-toplevel)

# Set paths
SITE_DIR="${REPO_ROOT}/site"
OUTPUT_DIR="${REPO_ROOT}/scripts/build"
PARTITIONS_FILE="${REPO_ROOT}/micro/partitions.csv"
OUTPUT_IMAGE="${OUTPUT_DIR}/littlefs_image.bin"

# Function to print an error message and exit
error() {
    echo "Error: $1" >&2
    exit 1
}

# Extract LittleFS offset and size from partitions.csv
if [ ! -f "$PARTITIONS_FILE" ]; then
    error "Partitions file not found at $PARTITIONS_FILE"
fi

LITTLEFS_LINE=$(grep -i 'littlefs' "$PARTITIONS_FILE" | head -n 1)
if [ -z "$LITTLEFS_LINE" ]; then
    error "No LittleFS partition found in $PARTITIONS_FILE"
fi

OFFSET=$(echo "$LITTLEFS_LINE" | awk -F',' '{print $4}' | xargs)
SIZE=$(echo "$LITTLEFS_LINE" | awk -F',' '{print $5}' | xargs)

if [ -z "$OFFSET" ] || [ -z "$SIZE" ]; then
    error "Failed to extract offset or size for LittleFS from $PARTITIONS_FILE"
fi

echo "Extracted LittleFS offset: $OFFSET, size: $SIZE from $PARTITIONS_FILE"

# Check for mklittlefs
if ! command -v mklittlefs >/dev/null 2>&1; then
    error "mklittlefs is not installed or not in PATH. Install it from: https://github.com/earlephilhower/mklittlefs"
fi

# Check if site/ directory exists
if [ ! -d "$SITE_DIR" ]; then
    error "$SITE_DIR does not exist. Create it and add your site files."
fi

# Create and clean the output directory
echo "Preparing output directory at $OUTPUT_DIR..."
rm -rf "$OUTPUT_DIR" || true
mkdir -p "$OUTPUT_DIR"

# Package the site directory into a LittleFS image
OUTPUT_IMAGE="$OUTPUT_DIR/littlefs_image.bin"
echo "Packaging $SITE_DIR into $OUTPUT_IMAGE..."
mklittlefs -c "$SITE_DIR" -p 256 -b 4096 -s "$SIZE" "$OUTPUT_IMAGE"
echo "LittleFS image created at $OUTPUT_IMAGE"

# Erase the LittleFS partition before flashing to avoid leftover data
echo "Erasing LittleFS partition at offset $OFFSET..."
esptool.py --port /dev/ttyUSB0 erase_region "$OFFSET" "$SIZE" || error "Failed to erase LittleFS partition."

# Flash the LittleFS image to the appropriate partition
echo "Flashing $OUTPUT_IMAGE to LittleFS partition at offset $OFFSET..."
esptool.py --port /dev/ttyUSB0 --baud 115200 write_flash "$OFFSET" "$OUTPUT_IMAGE" || error "Failed to flash LittleFS image to ESP32."

echo "LittleFS image successfully flashed!"
