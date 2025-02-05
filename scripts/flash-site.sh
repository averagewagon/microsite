#!/bin/sh

# Enable strict error handling for better script durability
set -eu

# Run shellcheck on the current script if available
if command -v shellcheck >/dev/null 2>&1; then
    shellcheck "$0" || echo "Shellcheck found issues."
else
    echo "Shellcheck not found, skipping..."
fi

# Function to print an error message in red and exit
error() {
    printf "\033[31mError: %s\033[0m\n" "$1" >&2
    exit 1
}

# Function to calculate directory size in bytes
calculate_dir_size() {
    du -sb "$1" | awk '{print $1}'
}

# Pre-check if the directory size exceeds the partition size
check_directory_size() {
    DIR_SIZE=$(calculate_dir_size "$OUTPUT_RESOURCES")
    echo "Directory size: $DIR_SIZE bytes"
    echo "Partition size: $PARTITION_SIZE_DEC bytes"

    if [ "$DIR_SIZE" -gt "$PARTITION_SIZE_DEC" ]; then
        echo "Warning: The resources directory is larger than the partition size."
        PERCENTAGE_OVER=$((DIR_SIZE * 100 / PARTITION_SIZE_DEC))
        echo "It is $PERCENTAGE_OVER% of the partition size."
        echo "Please reduce the size of the site directory or increase the partition size."

        echo "Largest files in the site directory:"
        find "$OUTPUT_RESOURCES" -type f -exec du -h {} + | sort -rh | head -10
        exit 1
    fi
}

# Function to compress files with Brotli
compress_with_brotli() {
    if ! command -v brotli >/dev/null 2>&1; then
        error "Brotli (brotli) is not installed or not in PATH. Install it with: sudo apt install brotli"
    fi

    echo "Compressing .html files with Brotli..."
    find "$OUTPUT_RESOURCES" -type f -name "*.html" | while IFS= read -r file; do
        # Compress file and save as .br
        brotli -f --best "$file" -o "$file.br"
        echo "Compressed: $file -> $file.br"
    done
}

# Paths and variables
REPO_ROOT=$(git rev-parse --show-toplevel)
SITE_DIR="${REPO_ROOT}/site/public"
OUTPUT_DIR="${REPO_ROOT}/scripts/build"
OUTPUT_RESOURCES="${OUTPUT_DIR}/resources"
PARTITIONS_FILE="${REPO_ROOT}/micro/partitions.csv"
OUTPUT_IMAGE="${OUTPUT_DIR}/littlefs_image.bin"

if [ ! -f "$PARTITIONS_FILE" ]; then
    error "Partitions file not found at $PARTITIONS_FILE"
fi

LITTLEFS_LINE=$(grep -i 'littlefs' "$PARTITIONS_FILE" | head -n 1)
if [ -z "$LITTLEFS_LINE" ]; then
    error "No LittleFS partition found in $PARTITIONS_FILE"
fi

OFFSET=$(echo "$LITTLEFS_LINE" | awk -F',' '{print $4}' | xargs)
SIZE_HEX=$(echo "$LITTLEFS_LINE" | awk -F',' '{print $5}' | xargs)

if [ -z "$OFFSET" ] || [ -z "$SIZE_HEX" ]; then
    error "Failed to extract offset or size for LittleFS from $PARTITIONS_FILE"
fi

# Convert hexadecimal size to decimal
PARTITION_SIZE_DEC=$(printf "%d" "$SIZE_HEX")
echo "Extracted LittleFS offset: $OFFSET, size: $SIZE_HEX bytes (${PARTITION_SIZE_DEC} bytes) from $PARTITIONS_FILE"

# Check for mklittlefs
if ! command -v mklittlefs >/dev/null 2>&1; then
    error "mklittlefs is not installed or not in PATH. Install it from: https://github.com/earlephilhower/mklittlefs"
fi

if [ ! -d "$SITE_DIR" ]; then
    error "$SITE_DIR does not exist. Create it and add your site files."
fi

# Create and clean the output directory
rm -rf "$OUTPUT_DIR" || true
mkdir -p "$OUTPUT_RESOURCES"

# Copy all files from SITE_DIR to OUTPUT_RESOURCES
echo "Copying files from $SITE_DIR to $OUTPUT_RESOURCES..."
cp -r "$SITE_DIR/"* "$OUTPUT_RESOURCES/"

# Compress files with Brotli
compress_with_brotli

# Pre-check directory size
check_directory_size

# Package the resources directory into a LittleFS image
echo "Packaging $OUTPUT_RESOURCES into $OUTPUT_IMAGE..."
if ! mklittlefs -c "$OUTPUT_RESOURCES" -p 256 -b 4096 -s "$PARTITION_SIZE_DEC" "$OUTPUT_IMAGE"; then
    error "Failed to create LittleFS image. The directory size likely exceeds the partition size."
fi

echo "LittleFS image created at $OUTPUT_IMAGE"

# Flashing process
echo "Erasing LittleFS partition at offset $OFFSET..."
esptool.py --port /dev/ttyACM0 erase_region "$OFFSET" "$PARTITION_SIZE_DEC" || error "Failed to erase LittleFS partition."

echo "Flashing $OUTPUT_IMAGE to LittleFS partition at offset $OFFSET..."
esptool.py --port /dev/ttyACM0 --baud 115200 write_flash "$OFFSET" "$OUTPUT_IMAGE" || error "Failed to flash LittleFS image to ESP32."

echo "LittleFS image successfully flashed!"
