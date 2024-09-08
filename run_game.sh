#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Check if setup.sh exists and is executable
if [ ! -x "./setup.sh" ]; then
    echo "Error: setup.sh not found or not executable. Please check the file."
    exit 1
fi

# Check if build.sh exists and is executable
if [ ! -x "./build.sh" ]; then
    echo "Error: build.sh not found or not executable. Please check the file."
    exit 1
fi

# Run setup.sh
echo "Running setup.sh..."
sudo bash setup.sh

# Run build.sh
echo "Running build.sh..."
sudo bash build.sh

# Check if the binary exists before attempting to run it
if [ ! -x "./main" ]; then
    echo "Error: Binary 'main' not found or not executable. Ensure build.sh completed successfully."
    exit 1
fi

# Execute the binary
echo "Executing './main'..."
./main
