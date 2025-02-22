#!/bin/bash

# INPUT:    Character-2 bitmap. Original order. (1000x1500 pixels)
# OUTPUT:   Character map where the chars are stacked then cropped (80x15000). ASCII 30 at bottom, highest ASCII value at the top. 

#  DEPS:    Imagemagick (v6?)


ORIG_BITMAP="characters-2-thick-thick.bmp"
OUTPUT_BITMAP="characters-2-thick-thick-tall.bmp"

# -----------------------------------

SCRIPT_FILE_DIR=$(dirname $0)

TMP_DIR="/tmp/physimos/characters_2/"
mkdir -p ${TMP_DIR}


# Base crop name
CROP_NAME="cropped.bmp"
# Remove first and last of 10 vertical slices
CROP_NAME_0="cropped-0.bmp"
CROP_NAME_9="cropped-9.bmp"


# Work on copy instead of original
cp ${SCRIPT_FILE_DIR}/${ORIG_BITMAP} ${TMP_DIR}/${ORIG_BITMAP}

# Crop each char, flip each vertically.
convert ${TMP_DIR}/${ORIG_BITMAP} -crop 100x150 +repage -flip +adjoin ${TMP_DIR}/tile_%02d.bmp

# stack chars to create a very tall image
convert ${TMP_DIR}tile_*.bmp -append ${TMP_DIR}/tall-inverted.bmp

# Crop image into 10px wide slices. Remove the first and last one
convert ${TMP_DIR}/tall-inverted.bmp -crop 10x15000 ${TMP_DIR}/${CROP_NAME}
rm ${TMP_DIR}/${CROP_NAME_0}
rm ${TMP_DIR}/${CROP_NAME_9}
convert ${TMP_DIR}/cropped-*.bmp +append ${TMP_DIR}/tall-cropped-inverted.bmp

#  Final vertical flip of tall stack
convert ${TMP_DIR}/tall-cropped-inverted.bmp -flip ${TMP_DIR}/output.bmp

# Save output
cp ${TMP_DIR}/output.bmp ${SCRIPT_FILE_DIR}/${OUTPUT_BITMAP}

# Cleanup
rm ${TMP_DIR}/*
