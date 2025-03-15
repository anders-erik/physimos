#!/bin/bash

# NOTE: Run from this directory!

CURR_DIR=$(pwd)
BIN_DIR=$CURR_DIR/bin/

mkdir -p $BIN_DIR

# NOTE: This should probably be copied once as an archive?
cp ../../extern/glad/src/glad.c ./bin/glad.c

SOURCES="main.cc bin/glad.c"
INCLUDE_DIRS="-I../../extern/glad/include -I../../extern/GLFW/include"
LIB_DIRS="-L../../extern/GLFW/lib"

g++ -Wall -g $INCLUDE_DIRS $SOURCES -o $BIN_DIR/cube.bin $LIB_DIRS -lglfw3


if [ ! $? -eq 0 ]; then
    echo ""
    echo "ERROR BUILDING XP-CUBE. EXITING."
    exit 1
fi


$BIN_DIR/cube.bin