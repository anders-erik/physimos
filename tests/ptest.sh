#!/usr/bin/env bash

CURRENT_FILE_DIR=$(dirname $0)
cd $CURRENT_FILE_DIR


source .venv/bin/activate

python3 ptest_pimage.py -r

