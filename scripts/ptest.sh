#!/bin/bash

# get repo root -- requires that this file is located in REPO_ROOT/scripts
CURRENT_FILE_DIR=$(dirname $0)
REPO_ROOT=$( cd $CURRENT_FILE_DIR && cd .. && pwd)

echo "PTEST:"

PTEST_MAIN_PYTHON_FILE=${REPO_ROOT}/tests/ptest_pimage.py

gnome-terminal -- python3 "$PTEST_MAIN_PYTHON_FILE"
