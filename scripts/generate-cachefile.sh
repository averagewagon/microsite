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

# Determine repository root and necessary directories
REPO_ROOT=$(git rev-parse --show-toplevel)
OUTPUT_RESOURCES="${REPO_ROOT}/scripts/build/resources"
CACHE_PRIORITY_LIST="${REPO_ROOT}/scripts/cache-priority-list.txt"
TEMP_CACHE_FILE="/tmp/cache-temp.txt"
FINAL_CACHE_FILE="${OUTPUT_RESOURCES}/cache.txt"

# Ensure the output resources directory exists
if [ ! -d "$OUTPUT_RESOURCES" ]; then
    error "Output resources directory does not exist: $OUTPUT_RESOURCES"
fi

# Clear or create the temporary cache file
rm -f "$TEMP_CACHE_FILE"
rm -f "$FINAL_CACHE_FILE"
touch "$TEMP_CACHE_FILE"

# Step 1: Copy priority list into the cache file
if [ -f "$CACHE_PRIORITY_LIST" ]; then
    cat "$CACHE_PRIORITY_LIST" >>"$TEMP_CACHE_FILE"
fi

# Step 2: Add root-level files from OUTPUT_RESOURCES
find "$OUTPUT_RESOURCES" -maxdepth 1 -type f | while IFS= read -r file; do
    relative_path=${file#"$OUTPUT_RESOURCES/"}
    grep -qxF "$relative_path" "$TEMP_CACHE_FILE" || echo "$relative_path" >>"$TEMP_CACHE_FILE"
done

# Step 3: Add files from top-level directories
find "$OUTPUT_RESOURCES" -mindepth 2 -maxdepth 2 -type f | while IFS= read -r file; do
    relative_path=${file#"$OUTPUT_RESOURCES/"}
    grep -qxF "$relative_path" "$TEMP_CACHE_FILE" || echo "$relative_path" >>"$TEMP_CACHE_FILE"
done

# Step 4: Add Brotli files from blog subdirectories (sorted by date, newest first)
find "$OUTPUT_RESOURCES/blog/" -mindepth 2 -maxdepth 2 -type d 2>/dev/null | sort -r | while IFS= read -r dir; do
    find "$dir" -type f -name "*.br" | while IFS= read -r file; do
        relative_path=${file#"$OUTPUT_RESOURCES/"}
        grep -qxF "$relative_path" "$TEMP_CACHE_FILE" || echo "$relative_path" >>"$TEMP_CACHE_FILE"
    done
done

# Step 5: Add non-Brotli files from blog subdirectories (same order)
find "$OUTPUT_RESOURCES/blog/" -mindepth 2 -maxdepth 2 -type d 2>/dev/null | sort -r | while IFS= read -r dir; do
    find "$dir" -type f ! -name "*.br" | while IFS= read -r file; do
        relative_path=${file#"$OUTPUT_RESOURCES/"}
        grep -qxF "$relative_path" "$TEMP_CACHE_FILE" || echo "$relative_path" >>"$TEMP_CACHE_FILE"
    done
done

# Step 6: Add any remaining files, recursively
find "$OUTPUT_RESOURCES" -type f | while IFS= read -r file; do
    relative_path=${file#"$OUTPUT_RESOURCES/"}
    grep -qxF "$relative_path" "$TEMP_CACHE_FILE" || echo "$relative_path" >>"$TEMP_CACHE_FILE"
done

# Step 7: Move temporary file to final cache file
mv "$TEMP_CACHE_FILE" "$FINAL_CACHE_FILE"

echo "Cache file generated at: $FINAL_CACHE_FILE"
