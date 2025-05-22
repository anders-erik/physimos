#!/bin/bash

# WORKING PACKAGE EXAMPLE
# ./scripts/package_create.sh
# sudo ./scripts/package_install.sh build/package_physimos.tar.gz /tmp/phys_install

echo ""
echo "Physimos package creation for installation."
echo ""

# PATHS
REPO_ROOT_DIR=$(pwd)
TMP_DIR="/tmp/physimos_package/"
PACKAGE_PATH=${REPO_ROOT_DIR}/"build/package_physimos.tar.gz"
PACKAGE_LOG="${TMP_DIR}/package.log"

# What to include in package
BINARY_MAIN_PATH="build/debug_linux/physimos"
DOCS_DIR="docs"
RESOURCES_DIR="resources"
SCRIPTS_DIR="scripts"
SHADERS_DIR="src/opengl/shaders"
UI_DIR="src/ui"
DRAW_DIR="src/draw"


create_tmp_dir(){
    if [ -d ${TMP_DIR} ]; then
        rm -rf ${TMP_DIR}
    fi

    mkdir -p ${TMP_DIR}
    echo "1/4: New tmp dir: ${TMP_DIR}."
    # echo ""
}

build(){
    cmake --build --preset=debug_linux > $PACKAGE_LOG 2>&1
    if [ $? -eq 1 ]; then
        echo ""
        echo "Package creation failed. Unable to build physiomos. Exiting."
        exit 1;
    fi
    echo "2/4: Build successful."
    # echo ""
}


copy_files(){
    cp ${BINARY_MAIN_PATH} ${TMP_DIR}/physimos_main
    chmod +x ${TMP_DIR}/physimos_main
    
    # Make parent dirs
    mkdir -p ${TMP_DIR}/${DOCS_DIR}/../
    mkdir -p ${TMP_DIR}/${RESOURCES_DIR}/../
    mkdir -p ${TMP_DIR}/${SCRIPTS_DIR}/../
    mkdir -p ${TMP_DIR}/${SHADERS_DIR}/../
    mkdir -p ${TMP_DIR}/${UI_DIR}/../
    mkdir -p ${TMP_DIR}/${DRAW_DIR}/../

    # Copy into parent dirs
    cp -r ${DOCS_DIR} ${TMP_DIR}/${DOCS_DIR}/..
    cp -r ${RESOURCES_DIR} ${TMP_DIR}/${RESOURCES_DIR}/..
    cp -r ${SCRIPTS_DIR} ${TMP_DIR}/${SCRIPTS_DIR}/..
    cp -r ${SHADERS_DIR} ${TMP_DIR}/${SHADERS_DIR}/..
    cp -r ${UI_DIR} ${TMP_DIR}/${UI_DIR}/..
    cp -r ${DRAW_DIR} ${TMP_DIR}/${DRAW_DIR}/..

    echo "3/4: Files Copied."
    # echo ""
}

create_package(){

    cd ${TMP_DIR}
    tar czf ${PACKAGE_PATH} . > $PACKAGE_LOG 2>&1
    cd - > $PACKAGE_LOG 2>&1

    echo "4/4: Package created."
    echo ""
}

create_tmp_dir
build
copy_files
create_package




list_tmp_dirs(){

    echo "Listing dir:"

    ls -l ${TMP_DIR}
    ls -l ${TMP_DIR}/${DOCS_DIR}
    ls -l ${TMP_DIR}/${RESOURCES_DIR}
    # ls -l ${TMP_DIR}/${RESOURCES_DIR}/"models"
    ls -l ${TMP_DIR}/${SCRIPTS_DIR}
    ls -l ${TMP_DIR}/${SHADERS_DIR}
    ls -l ${TMP_DIR}/${UI_DIR}

    echo ""
}
# list_tmp_dirs




