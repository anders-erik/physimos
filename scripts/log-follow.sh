#!/bin/bash

# get repo root -- requires that this file is located in REPO_ROOT/scripts
CURRENT_FILE_DIR=$(dirname $0)
REPO_ROOT=$( cd $CURRENT_FILE_DIR && cd .. && pwd)

tail -f ${REPO_ROOT}/logs/main.log 

