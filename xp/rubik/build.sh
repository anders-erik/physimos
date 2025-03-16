#!/bin/bash

# NOTE: Run from this directory!

CURR_DIR=$(pwd)
BIN_DIR=$CURR_DIR/bin/

mkdir -p $BIN_DIR

# NOTE: This should probably be copied once as an archive?
cp ../../extern/glad/src/glad.c ./bin/glad.c

SOURCES="main.cc window.cc gpu.cc model.cc fs.cc math.cc scene.cc bin/glad.c"
INCLUDE_DIRS="-I. -I../../extern/glad/include -I../../extern/GLFW/include"
LIB_DIRS="-L../../extern/GLFW/lib"

time g++ -Wall -g $INCLUDE_DIRS $SOURCES -o $BIN_DIR/rubik.bin $LIB_DIRS -lglfw3


if [ ! $? -eq 0 ]; then
    echo ""
    echo "ERROR BUILDING XP-CUBE. EXITING."
    exit 1
fi

exit 0
