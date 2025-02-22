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

# Log directory
LOG_DIR="$HOME/microsite_logs"
mkdir -p "$LOG_DIR"

# UART Device
UART_DEVICE="/dev/ttyACM0"
RETRY_INTERVAL=5 # Seconds to wait before retrying on failure

# Function to generate log filename based on the current hour
get_log_filename() {
    date "+$LOG_DIR/%Y-%m-%d_%H.log"
}

# Function to start UART logging
start_logging() {
    # Initialize log file
    LOG_FILE="$(get_log_filename)"
    echo "Starting UART logging to $LOG_FILE"

    # Loop to handle device disconnections and log rollover
    while true; do
        if [ ! -e "$UART_DEVICE" ]; then
            error "UART device $UART_DEVICE not found. Retrying in $RETRY_INTERVAL seconds..."
            sleep "$RETRY_INTERVAL"
            continue
        fi

        # Ensure the UART device is configured properly
        if ! stty -F "$UART_DEVICE" 115200 raw -echo; then
            error "Failed to configure UART device. Retrying in $RETRY_INTERVAL seconds..."
            sleep "$RETRY_INTERVAL"
            continue
        fi

        # Read UART and prepend timestamp, with automatic log rollover
        stdbuf -oL cat "$UART_DEVICE" | while IFS= read -r line; do
            NEW_LOG_FILE="$(get_log_filename)"

            # Check if we need to roll over to a new file
            if [ "$NEW_LOG_FILE" != "$LOG_FILE" ]; then
                echo "Rolling over to new log file: $NEW_LOG_FILE"
                LOG_FILE="$NEW_LOG_FILE"
            fi

            printf "%s %s\n" "$(date '+%Y-%m-%d %H:%M:%S')" "$line" | tee -a "$LOG_FILE"
        done

        # If we exit the inner loop, the device likely disappeared or stopped outputting
        error "UART device $UART_DEVICE disconnected or stopped outputting. Retrying in $RETRY_INTERVAL seconds..."
        sleep "$RETRY_INTERVAL"
    done
}

# Start logging process
start_logging
