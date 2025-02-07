#!/bin/bash

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

# Determine the root of the Git repository
REPO_ROOT=$(git rev-parse --show-toplevel)

# Define output directory
BUILD_DIR="$REPO_ROOT/scripts/build/context"

# Remove existing context directory if it exists, then recreate it
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

### Function to collect and format source files ###
collect_sources() {
    local target_file="$1"
    shift
    local files="$@"

    for file in $files; do
        if [ -f "$file" ]; then
            printf "===== FILE: %s =====\n" "$file" >>"$target_file"
            cat "$file" >>"$target_file"
            printf "\n\n" >>"$target_file"
        fi
    done
}

echo "Gathering context..."

# 1. Process micro directory
echo "Processing micro/ directory..."
MICRO_DIR="$REPO_ROOT/micro"

# Generate tree structure
(cd "$MICRO_DIR" && tree -I 'managed_components|build|.cache' >"$BUILD_DIR/micro_filetree.txt")

# Collect source files
MICRO_CODE="$BUILD_DIR/micro_code.txt"
echo "Collecting micro source files..."
collect_sources "$MICRO_CODE" "$MICRO_DIR/main/main.c" "$(find "$MICRO_DIR/components" -type f \( -name "*.h" -o -name "*.c" \))"

# 2. Process site directory
echo "Processing site/ directory..."
SITE_DIR="$REPO_ROOT/site"

# Generate tree structure
(cd "$SITE_DIR" && tree -I 'public|resources/_gen' >"$BUILD_DIR/site_filetree.txt")

# Collect site content
SITE_CODE="$BUILD_DIR/site_code.txt"
echo "Collecting site content..."
collect_sources "$SITE_CODE" "$SITE_DIR/hugo.yaml" "$(find "$SITE_DIR" -type f \( -name "*.html" -o -name "*.md" -o -name "*.css" \) ! -path "*/public/*" ! -path "*/content/blog/*" ! -path "*/resources/_gen/*")"

# 3. Process scripts directory
echo "Processing scripts/ directory..."
SCRIPTS_DIR="$REPO_ROOT/scripts"

# Generate tree structure
(cd "$SCRIPTS_DIR" && tree -I 'build' >"$BUILD_DIR/scripts_filetree.txt")

# Collect script files
SCRIPTS_CODE="$BUILD_DIR/scripts_code.txt"
echo "Collecting scripts..."
collect_sources "$SCRIPTS_CODE" "$(find "$SCRIPTS_DIR" -type f -name "*.sh")"

# 4. Copy and rename README files
echo "Copying and renaming README files..."
rename_and_copy() {
    local source_path="$1"
    local new_name="$2"
    if [ -f "$source_path" ]; then
        cp "$source_path" "$BUILD_DIR/$new_name"
    fi
}

rename_and_copy "$REPO_ROOT/README.md" "root_README.md"
rename_and_copy "$MICRO_DIR/README.md" "micro_README.md"
rename_and_copy "$SITE_DIR/README.md" "site_README.md"
rename_and_copy "$SCRIPTS_DIR/README.md" "scripts_README.md"

echo "Context gathering complete. Files saved in $BUILD_DIR."
