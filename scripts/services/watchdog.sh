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

# uhubctl target hub and port (RPi4B default values)
USB_HUB="1-1"
USB_ACTION_OFF=0
USB_ACTION_ON=1

# Function to check website status
check_website() {
    if ! curl --max-time "$TIMEOUT" --silent --fail "$URL" >/dev/null 2>&1; then
        error "Website $URL is unreachable. Restarting ESP32-S3..."
        restart_board
    else
        echo "Website is online."
    fi
}

# Function to restart the ESP32-S3 using uhubctl
restart_board() {
    if ! command -v uhubctl >/dev/null 2>&1; then
        error "uhubctl not found. Cannot reset ESP32-S3."
        exit 1
    fi

    echo "Turning off USB power..."
    sudo uhubctl -l "$USB_HUB" -a "$USB_ACTION_OFF"
    sleep 5 # Wait for power to fully discharge

    echo "Turning on USB power..."
    sudo uhubctl -l "$USB_HUB" -a "$USB_ACTION_ON"
}

# Run the website check
check_website
