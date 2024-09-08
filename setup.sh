#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Check if setup.sh file exists in the user's current directory.
if [ -f "./setup.sh" ]; then

    sudo apt install g++ -y

    # Check for root privileges
    if [ "$EUID" -ne 0 ]; then 
        echo "Please run as root or use sudo"
        exit
    fi

    # Define project directories
    PROJECT_DIR=$(pwd)
    #VULKAN_SDK_DIR="$PROJECT_DIR/vulkan_sdk"
    #VULKAN_SDK_TAR="$VULKAN_SDK_DIR/vulkan-sdk.tar.gz"
    #EXTRACTED_SDK_DIR="$VULKAN_SDK_DIR/vulkan_sdk_extracted"  # Standard directory for the extracted SDK

    # Function to install AMD GPU drivers
    install_amd_drivers() {
        echo "Installing AMD GPU drivers..."
        sudo apt-get update
        sudo apt-get install -y firmware-amd-graphics xserver-xorg-video-amdgpu
        sudo apt-get install vulkan-tools vulkan-validationlayers
        sudo apt install libvulkan1 mesa-vulkan-drivers vulkan-utils
    }

    # Function to install Vulkan packages for AMD GPUs (Mesa drivers)
    install_mesa_vulkan() {
        echo "Installing Vulkan packages for AMD..."
        sudo apt-get install -y mesa-vulkan-drivers vulkan-tools libvulkan-dev
    }

    # Function to handle generic drivers (llvmpipe, software rendering)
    handle_generic_drivers() {
        echo "It looks like you're using generic drivers or software rendering."
        echo "Installing AMD GPU drivers..."
        install_amd_drivers
        echo "Proceeding with Vulkan setup..."
        install_mesa_vulkan
    }

    # Detect the GPU vendor
    GPU_VENDOR=$(lspci | grep -i "VGA" | grep -i "amd" || echo "unknown")
    if [ "$GPU_VENDOR" != "unknown" ]; then
        echo "Detected AMD GPU"
        install_mesa_vulkan
    else
        # Check for software rendering (e.g., llvmpipe)
        RENDERER=$(glxinfo | grep "renderer string")
        if echo "$RENDERER" | grep -qi "llvmpipe"; then
            handle_generic_drivers
        else
            echo "No AMD GPU detected and no software rendering detected. Exiting."
            exit 1
        fi
    fi

    # Ask the user for window management library choice (GLFW or SDL2)
    echo "Choose a window management library:"
    echo "1) GLFW"
    echo "2) SDL2"
    read -p "Enter the number of your choice (1 or 2): " wm_choice

    # Install either GLFW or SDL2 based on user choice
    if [ "$wm_choice" = "1" ]; then
        echo "Installing GLFW..."
        sudo apt-get install -y libglfw3-dev
    elif [ "$wm_choice" = "2" ]; then
        echo "Installing SDL2..."
        sudo apt-get install -y libsdl2-dev
    else
        echo "Invalid choice. Exiting."
        exit 1
    fi

    

    # Set up environment variables in the current shell
    #export VULKAN_SDK="$EXTRACTED_SDK_DIR/x86_64"
    #export PATH="$VULKAN_SDK/bin:$PATH"
    #export LD_LIBRARY_PATH="$VULKAN_SDK/lib:$LD_LIBRARY_PATH"
    #export VK_LAYER_PATH="$VULKAN_SDK/etc/vulkan/explicit_layer.d"

    # Add to ~/.bashrc for future sessions
    #echo "export VULKAN_SDK=$EXTRACTED_SDK_DIR/x86_64" >> ~/.bashrc
    #echo "export PATH=\$VULKAN_SDK/bin:\$PATH" >> ~/.bashrc
    #echo "export LD_LIBRARY_PATH=\$VULKAN_SDK/lib:\$LD_LIBRARY_PATH" >> ~/.bashrc
    #echo "export VK_LAYER_PATH=\$VULKAN_SDK/etc/vulkan/explicit_layer.d" >> ~/.bashrc

    # Check if permissions.sh exists before running it
    if [ -f "./permissions.sh" ]; then
        echo "Running permissions.sh..."
        sudo bash permissions.sh
    else
        echo "Warning: permissions.sh not found. Skipping this step."
    fi

    echo "Development environment setup complete!"

else
    echo "Warning: setup.sh not found in your current directory. Skipping this step."
    echo "Make sure that the project's root folder is your current directory."
    echo "Execute \"ls\" or \"pwd\" to check your current directory."
fi
