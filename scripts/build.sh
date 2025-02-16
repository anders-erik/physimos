#!/bin/bash

# get repo root -- requires that this file is located in REPO_ROOT/scripts
CURRENT_FILE_DIR=$(dirname $0)
REPO_ROOT=$( cd $CURRENT_FILE_DIR && cd .. && pwd)

SOURCE_DIR=${REPO_ROOT}
BUILD_DIR=

BUILD_TYPE="$1"

set_build_dir(){

    if   [ "$1" == "debug" ]; then
        BUILD_DIR="$REPO_ROOT/build/debug"

    elif [ "$1" == "gprof" ]; then
        BUILD_DIR="$REPO_ROOT/build/gprof"

    else
        BUILD_DIR="$REPO_ROOT/build/release-linux"

    fi
}

run_cmake(){

    if   [ "$1" == "debug" ]; then
        cmake -B"$BUILD_DIR" -S"$SOURCE_DIR" -DBUILD_TYPE_FROM_CLI:STRING=DEBUG

    elif [ "$1" == "gprof" ]; then
        cmake -B"$BUILD_DIR" -S"$SOURCE_DIR" -DBUILD_TYPE_FROM_CLI:STRING=GPROF

    else
        cmake -B"$BUILD_DIR" -S"$SOURCE_DIR" -DBUILD_TYPE_FROM_CLI:STRING=RELEASE_LINUX

    fi
}


# MAIN SCRIPT

set_build_dir "$BUILD_TYPE"

if [ "$2" == "clean" ]; then
    rm -r "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"

run_cmake "$BUILD_TYPE"

cd "$BUILD_DIR"
make

