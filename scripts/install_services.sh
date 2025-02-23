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

# Determine the root of the Git repository
REPO_ROOT=$(git rev-parse --show-toplevel)
SERVICE_DIR="$REPO_ROOT/scripts/services"

# Ensure the service directory exists
if [ ! -d "$SERVICE_DIR" ]; then
    error "Service directory $SERVICE_DIR not found."
    exit 1
fi

BIN_DIR="/usr/local/bin"
SYSTEMD_DIR="/etc/systemd/system"

echo "Installing scripts and services from $SERVICE_DIR..."

# Symlink all .sh scripts into /usr/local/bin
for script in "$SERVICE_DIR"/*.sh; do
    if [ -f "$script" ]; then
        SCRIPT_NAME="$(basename "$script")"
        TARGET_SCRIPT="$BIN_DIR/$SCRIPT_NAME"

        echo "Symlinking script: $SCRIPT_NAME -> $TARGET_SCRIPT"
        sudo ln -sf "$script" "$TARGET_SCRIPT"
        sudo chmod +x "$TARGET_SCRIPT"
    fi
done

# Symlink and enable all .service files
for service in "$SERVICE_DIR"/*.service; do
    if [ -f "$service" ]; then
        SERVICE_NAME="$(basename "$service")"
        TARGET_SERVICE="$SYSTEMD_DIR/$SERVICE_NAME"

        echo "Symlinking systemd service: $SERVICE_NAME -> $TARGET_SERVICE"
        sudo ln -sf "$service" "$TARGET_SERVICE"

        echo "Reloading systemd..."
        sudo systemctl daemon-reload

        SERVICE_UNIT="${SERVICE_NAME%.service}"
        echo "Enabling and starting service: $SERVICE_UNIT"
        sudo systemctl enable "$SERVICE_UNIT"
        sudo systemctl start "$SERVICE_UNIT"
    fi
done

echo "All scripts and services installed successfully."
