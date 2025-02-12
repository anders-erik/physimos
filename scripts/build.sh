#!/bin/bash

# get repo root -- requires that this file is located in REPO_ROOT/scripts
CURRENT_FILE_DIR=$(dirname $0)
REPO_ROOT=$( cd $CURRENT_FILE_DIR && cd .. && pwd)

# echo $REPO_ROOT
mkdir -p "$REPO_ROOT/build_cmake"

cd $REPO_ROOT/build_cmake && cmake .. && make

# ININ=$(read -u 0)

echo $ININ