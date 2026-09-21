#!/bin/bash


BUILD_DIR="build/debug_linux"

# Check if src/build/debug_linux exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "Directory src/build/debug_linux does not exist."
    exit 1
fi


# Proceed with installation steps
echo "Directory $BUILD_DIR exists. Proceeding with installation."

cd $BUILD_DIR

sudo cp ./physimos /usr/local/bin/
sudo cp ./src/apps/piano/main_piano /usr/local/bin/physimos_piano
