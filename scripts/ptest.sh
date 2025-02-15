#!/bin/bash

# NOT CURRENTLY IN USE
# "REPO_ROOT/ptest/ptest.sh" is the new entrypoint

# get repo root -- requires that this file is located in REPO_ROOT/scripts
CURRENT_FILE_DIR=$(dirname $0)
REPO_ROOT=$( cd $CURRENT_FILE_DIR && cd .. && pwd)

echo "PTEST SCRIPT:"

PTEST_MAIN_PYTHON_FILE="${REPO_ROOT}/ptest/ptest_curses.sh"

gnome-terminal -- python3 "$PTEST_MAIN_PYTHON_FILE"
