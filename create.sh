#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Check if the project name is provided
if [ -z "$1" ]; then
    echo "Usage: bash create.sh <projectname>"
    exit 1
fi

# Define variables
REPO_URL="https://github.com/DavidPerezContreras/Vulkan_ENGINE_HELLO_WORLD_V1.git"
PROJECT_NAME="$1"
CLONE_DIR="Vulkan_ENGINE_HELLO_WORLD_V1"

# Clone the repository and rename it in one line
echo "Cloning repository from $REPO_URL..."
git clone "$REPO_URL" "$PROJECT_NAME"

# Change to the new project directory
cd "$PROJECT_NAME"

# Optional: Perform additional setup tasks here
# For example, if you have a setup script in the new repository
# echo "Running setup script..."
# bash setup.sh

echo "Project $PROJECT_NAME has been created successfully!"
