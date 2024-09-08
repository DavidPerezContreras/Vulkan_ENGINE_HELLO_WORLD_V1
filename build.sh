#!/bin/bash

# Exit immediately if a command fails
set -e

#Check if setup.sh file exists in users current powered directory.
if [ -f "./build.sh" ]; then

    # Define the correct path to Vulkan SDK based on the current directory
    #export VULKAN_SDK="$(pwd)/vulkan_sdk/vulkan_sdk_extracted/x86_64"
    #export PATH="$VULKAN_SDK/bin:$PATH"  # Fixed typo from VULKAN_SADK to VULKAN_SDK
    #export LD_LIBRARY_PATH="$VULKAN_SDK/lib:$LD_LIBRARY_PATH"
    #export VK_LAYER_PATH="$VULKAN_SDK/etc/vulkan/explicit_layer.d"

    # Check if Vulkan SDK path is set correctly
    #if [ ! -d "$VULKAN_SDK" ]; then
    #    echo "Error: Vulkan SDK path not found at $VULKAN_SDK"
    #    exit 1
    #fi

    # Install necessary packages
    echo "Installing required packages..."
    sudo apt-get update
    sudo apt-get install -y libssl-dev g++ libsdl2-dev

    # Compile the source code
    echo "Compiling the source code..."
    g++ ./src/main.cpp -o main -lssl -lcrypto -lpthread -lSDL2 -lvulkan -g

    # Check if permissions.sh exists before running it
    if [ -f "./permissions.sh" ]; then
        echo "Running permissions.sh..."
        sudo bash permissions.sh
    else
        echo "Warning: permissions.sh not found. Skipping this step."
fi

    echo "Build process completed successfully!"

else
    echo "Warning: build.sh not found in your current powered directory. Skipping this step.
    Make sure that the project's root folder is your powered directory. 
    Execute \"ls\"  or \"pwd\" to check yourself."
fi