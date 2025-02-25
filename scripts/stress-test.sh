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
BUILD_DIR="$REPO_ROOT/scripts/build"
STRESS_DIR="$BUILD_DIR/stress"
RESOURCES_DIR="$BUILD_DIR/resources"

# Check if the resources directory exists
if [ ! -d "$RESOURCES_DIR" ]; then
    error "The website hasn't been built yet. Run flash-site.sh first."
fi

# Clean up the stress test directory
if [ -d "$STRESS_DIR" ]; then
    rm -rf "$STRESS_DIR"
fi
mkdir -p "$STRESS_DIR"

# Scrape the contents of the resources directory and create a list of files to request
FILE_LIST="$STRESS_DIR/file_list.txt"
find "$RESOURCES_DIR" -type f ! -name '*.br' >"$FILE_LIST"

# Generate the Lua script for wrk
LUA_SCRIPT="$STRESS_DIR/pages.lua"
cat <<EOF >"$LUA_SCRIPT"
-- pages.lua

local pages = {
$(sed "s|$RESOURCES_DIR||;s|^\(.*\)$|    \"\1\",|" "$FILE_LIST")
}

function select_random_page()
    local index = math.random(1, #pages)
    return pages[index]
end

function request()
    local path = select_random_page()
    local headers = {
        ["Accept-Encoding"] = "br"
    }
    return wrk.format("GET", path, headers)
end
EOF

# Execute wrk and save the result in output.txt
OUTPUT_FILE="$STRESS_DIR/stress-test-output.txt"
wrk -t1 -c50 -d30s -s "$LUA_SCRIPT" https://joni-on-micro.site >"$OUTPUT_FILE"

echo "Stress test completed. Results saved in $OUTPUT_FILE"
