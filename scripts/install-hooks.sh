#!/bin/sh

# Enable strict error handling for better script durability
set -eu

# Run shellcheck on the current script
command -v shellcheck >/dev/null 2>&1 && shellcheck "$0" || echo "Shellcheck not found, skipping..."

# Determine the root of the Git repository
REPO_ROOT=$(git rev-parse --show-toplevel)

# Remove any existing pre-commit hook
rm -f "$REPO_ROOT/.git/hooks/pre-commit"

# Create a symlink to the pre-commit hook script
ln -s "$REPO_ROOT/scripts/pre-commit" "$REPO_ROOT/.git/hooks/pre-commit"

echo "Pre-commit hook installed as a symlink."
