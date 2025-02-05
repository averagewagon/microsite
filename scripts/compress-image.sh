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

# Check arguments
if [ "$#" -ne 2 ]; then
    error "Usage: $0 <input_file> <output_prefix>"
fi

input_file="$1"
output_prefix="$2"
output_file="${output_prefix}_0.webp"

# Verify input file exists
if [ ! -f "$input_file" ]; then
    error "Input file not found: $input_file"
fi

# Copy the input file to start the compression chain
cp "$input_file" "$output_file"

# Run three iterations of compression
for i in 1 2; do
    next_output_file="${output_prefix}_${i}.webp"
    cwebp -q 1 "$output_file" -o "$next_output_file" || error "Compression failed at iteration $i"
    output_file="$next_output_file"
    echo "Iteration $i completed: $output_file"
done

echo "Final file: $output_file"
