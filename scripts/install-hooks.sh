#!/bin/sh

# Determine the root of the Git repository
REPO_ROOT=$(git rev-parse --show-toplevel)

# Remove any existing pre-commit hook
rm -f "$REPO_ROOT/.git/hooks/pre-commit"

# Create a symlink to the pre-commit hook script
ln -s "$REPO_ROOT/scripts/pre-commit" "$REPO_ROOT/.git/hooks/pre-commit"

echo "Pre-commit hook installed as a symlink."
