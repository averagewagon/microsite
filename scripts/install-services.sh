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
SERVICE_DIR="$REPO_ROOT/services"

# Ensure the service directory exists
if [ ! -d "$SERVICE_DIR" ]; then
    error "Service directory $SERVICE_DIR not found."
    exit 1
fi

BIN_DIR="/usr/local/bin"
SYSTEMD_DIR="/etc/systemd/system"
SUDOERS_FILE="/etc/sudoers.d/microsite_uhubctl"

echo "Resetting system user for microsite services..."

# Remove the microsite user if it exists (clean state)
if id -u microsite >/dev/null 2>&1; then
    echo "Removing existing user: microsite"
    sudo systemctl stop monitor_microsite || true
    sudo systemctl stop watchdog_microsite || true
    sudo systemctl disable monitor_microsite || true
    sudo systemctl disable watchdog_microsite || true
    sudo userdel -r microsite || true
fi

# Create a new system user
echo "Creating new system user: microsite"
sudo useradd --system --create-home --shell /usr/sbin/nologin --groups dialout microsite

# Grant microsite permission to run uhubctl as root without a password
echo "Configuring sudo access for uhubctl..."
echo "microsite ALL=(ALL) NOPASSWD: /usr/sbin/uhubctl" | sudo tee "$SUDOERS_FILE" >/dev/null
sudo chmod 440 "$SUDOERS_FILE"

echo "Installing scripts and services from $SERVICE_DIR..."

# Copy all .sh scripts into /usr/local/bin
for script in "$SERVICE_DIR"/*.sh; do
    if [ -f "$script" ]; then
        SCRIPT_NAME="$(basename "$script")"
        TARGET_SCRIPT="$BIN_DIR/$SCRIPT_NAME"

        # Remove existing file to avoid conflicts
        if [ -f "$TARGET_SCRIPT" ]; then
            echo "Removing existing script: $TARGET_SCRIPT"
            sudo rm -f "$TARGET_SCRIPT"
        fi

        echo "Copying script: $SCRIPT_NAME -> $TARGET_SCRIPT"
        sudo cp "$script" "$TARGET_SCRIPT"
        sudo chmod +x "$TARGET_SCRIPT"
        sudo chown microsite:microsite "$TARGET_SCRIPT"
    fi
done

# Copy and enable all .service files
for service in "$SERVICE_DIR"/*.service; do
    if [ -f "$service" ]; then
        SERVICE_NAME="$(basename "$service")"
        TARGET_SERVICE="$SYSTEMD_DIR/$SERVICE_NAME"

        # Remove existing service file to avoid conflicts
        if [ -f "$TARGET_SERVICE" ]; then
            echo "Removing existing systemd service: $TARGET_SERVICE"
            sudo rm -f "$TARGET_SERVICE"
        fi

        echo "Copying systemd service: $SERVICE_NAME -> $TARGET_SERVICE"
        sudo cp "$service" "$TARGET_SERVICE"

        echo "Setting correct permissions..."
        sudo chmod 644 "$TARGET_SERVICE"

        echo "Reloading systemd..."
        sudo systemctl daemon-reload

        SERVICE_UNIT="${SERVICE_NAME%.service}"
        echo "Enabling and restarting service: $SERVICE_UNIT"
        sudo systemctl enable "$SERVICE_UNIT"
        sudo systemctl restart "$SERVICE_UNIT"
    fi
done

echo "All scripts and services installed successfully."
