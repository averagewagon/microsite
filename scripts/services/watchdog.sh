#!/bin/sh

# Enable strict error handling
set -eu

# Run shellcheck on the current script if available
if command -v shellcheck >/dev/null 2>&1; then
    shellcheck "$0" || echo "Shellcheck found issues."
else
    echo "Shellcheck not found, skipping..."
fi

# Function to print an error message
error() {
    printf "\033[31mError: %s\033[0m\n" "$1" >&2
}

# URL to monitor
URL="https://joni-on-micro.site"

# Timeout for HTTP request (in seconds)
TIMEOUT=30

# ESP32-S3 device
ESP_PORT="/dev/ttyACM0"

# Function to check website status
check_website() {
    if ! curl --max-time "$TIMEOUT" --silent --fail "$URL" >/dev/null 2>&1; then
        error "Website $URL is unreachable. Restarting ESP32-S3..."
        restart_board
    else
        echo "Website is online."
    fi
}

# Function to restart the ESP32-S3 board
restart_board() {
    if ! command -v esptool.py >/dev/null 2>&1; then
        error "esptool.py not found. Cannot reset ESP32-S3."
        exit 1
    fi

    if [ ! -e "$ESP_PORT" ]; then
        error "ESP32-S3 device not found at $ESP_PORT."
        exit 1
    fi

    esptool.py --chip esp32s3 --port "$ESP_PORT" reset
}

# Run the website check
check_website
