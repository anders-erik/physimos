#!/bin/bash


function download_picotool()
{
    TAR_FILE="tmp/picotool-2.3.1.tar.gz"
    OUTPUT_DIR="./tmp"

    wget https://github.com/raspberrypi/picotool/releases/download/2.3.1/picotool-2.3.1.tar.gz -O $TAR_FILE
    tar -xzf $TAR_FILE -C $OUTPUT_DIR
}

function get_pico_sdk()
{
    git clone https://github.com/raspberrypi/pico-sdk.git ./src/pico/pico-sdk
}

function install_pico_stuff()
{
    sudo apt update
    sudo apt install gcc-arm-none-eabi libnewlib-arm-none-eabi
}

function download_pico_sdk_submodules()
{
    cd ./src/pico/pico_sdk/pico-sdk
    git submodule update --init --recursive
    cd -
}

function build_pico_main()
{
    cd ./src/pico/pico_sdk
    cmake -S . -B build
    cmake --build build
    echo "Pico Build complete."
    cd -
}

function upload_pico_main()
{
    cd ./src/pico/pico_sdk
    cp ./build/main_pico_sdk.uf2 /media/$USER/RPI-RP2/
}



# download_picotool
# get_pico_sdk
# install_pico_stuff
build_pico_main
upload_pico_main
