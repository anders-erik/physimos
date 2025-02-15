#!/usr/bin/env bash

CURRENT_FILE_DIR=$(dirname $0)
cd $CURRENT_FILE_DIR


source ${CURRENT_FILE_DIR}/.venv/bin/activate

if [ $1 == "ui_curses" ]; then
    # python3 ${CURRENT_FILE_DIR}/curses_test.py
    gnome-terminal -- python3 ${CURRENT_FILE_DIR}/curses_test.py
else
    python3 ${CURRENT_FILE_DIR}/ptest_main.py $@
fi


