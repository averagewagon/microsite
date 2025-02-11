#!/bin/sh

# Enable strict error handling
set -eu

# Run shellcheck if available
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

# Get the root directory of the Git repository
REPO_ROOT=$(git rev-parse --show-toplevel) || error "Failed to determine repo root."

# Define paths
SECRETS_DIR="$HOME/secrets/secrets_microsite"
ARCHIVE_DIR="$SECRETS_DIR/archive"
CERTBOT_LIVE_DIR="/etc/letsencrypt/live/joni-on-micro.site"
DATE_PREFIX=$(date +"%Y-%m-%d")
USER=$(whoami)

# Ensure the archive directory exists
mkdir -p "$ARCHIVE_DIR"

# Move old certificates to the archive folder
if ls "$SECRETS_DIR"/*-fullchain.pem "$SECRETS_DIR"/*-privkey.pem >/dev/null 2>&1; then
    mv "$SECRETS_DIR"/*-fullchain.pem "$SECRETS_DIR"/*-privkey.pem "$ARCHIVE_DIR"/ || error "Failed to archive old certificates."
    echo "Archived previous certificates."
else
    echo "No old certificates found to archive."
fi

# Request a new ECDSA certificate using manual DNS challenge
sudo certbot certonly --manual --preferred-challenges dns \
    --key-type ecdsa --elliptic-curve secp256r1 \
    --cert-name joni-on-micro.site \
    -d joni-on-micro.site || error "Certbot certificate request failed."

# Copy new certificates with date prefix (requires sudo)
sudo cp "$CERTBOT_LIVE_DIR/fullchain.pem" "$SECRETS_DIR/$DATE_PREFIX-fullchain.pem" || error "Failed to copy fullchain.pem"
sudo cp "$CERTBOT_LIVE_DIR/privkey.pem" "$SECRETS_DIR/$DATE_PREFIX-privkey.pem" || error "Failed to copy privkey.pem"

# Change ownership back to the original user
sudo chown "$USER":"$USER" "$SECRETS_DIR/$DATE_PREFIX-fullchain.pem" "$SECRETS_DIR/$DATE_PREFIX-privkey.pem"

# Update symlinks in the ESP32 project
ln -sf "$SECRETS_DIR/$DATE_PREFIX-fullchain.pem" "$REPO_ROOT/micro/main/certs/fullchain.pem"
ln -sf "$SECRETS_DIR/$DATE_PREFIX-privkey.pem" "$REPO_ROOT/micro/main/certs/privkey.pem"

echo "Certificate renewal completed. New certificates are in $SECRETS_DIR."
echo "Symlinks updated in the ESP32 project."

exit 0
