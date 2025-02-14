#!/usr/bin/env bash

CURRENT_FILE_DIR=$(dirname $0)
cd $CURRENT_FILE_DIR


python3 -m venv .venv

source .venv/bin/activate

python3 -m pip install --upgrade pip
python3 -m pip install --upgrade Pillow


deactivate

