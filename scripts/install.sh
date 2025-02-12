#!/bin/bash

echo "Physimos installation!"

# VERIFY SUDO
if [ $(id -u) -ne 0 ]
  then echo "Sudo privileges are required for installation. exiting."
  exit 1;
fi

# IF IN REPO, CHECK BUILD DIR
CURRENT_FILE_DIR=$(dirname $0)
REPO_ROOT=$( cd $CURRENT_FILE_DIR && cd .. && pwd)

# GRAB CURRENT USER
CURRENT_USER=
# Currently installs will be configured on a user basis.
# Thus the current user will be set to the user that ran as root.
if [ $USER == "root" ]; then
  CURRENT_USER=$SUDO_USER
else
  CURRENT_USER=$USER
fi
echo 'Installing as : $USER =' $CURRENT_USER



# Get Users Home Directory
CURRENT_USER_PASSWD_ROW=$(getent passwd "$CURRENT_USER")
CURRENT_USER_HOME_DIR=$(echo "$CURRENT_USER_PASSWD_ROW" | cut -d: -f6)


# Installed Physimos root directory
CONFIG_DIR="${CURRENT_USER_HOME_DIR}/.config/physimos"
mkdir -p "$CONFIG_DIR"
echo 'Config Directory =' $CONFIG_DIR



# FIND BUILT BINARY

BINARY_SOURCE=""
BINARY_TARGET="/usr/bin/physimos"

find_binary(){

    if [ -f ${REPO_ROOT}/build_cmake/physimos ]; then
        BINARY_SOURCE=${REPO_ROOT}/build_cmake/physimos
        echo "Binary found @ $BINARY_SOURCE"
        return 0
    fi

    # BUILD
    echo "Binary not found. Building."
    ${REPO_ROOT}/scripts/clean-build.sh
    echo "Build done."

    if [ ! -f ${REPO_ROOT}/build_cmake/physimos ]; then
        echo "Unable to location binary after build. Exiting."
        exit 1;
    fi

    BINARY_SOURCE=${REPO_ROOT}/build_cmake/physimos
}

find_binary


# SET BINARY
echo "Copying binary to $BINARY_TARGET"
cp $BINARY_SOURCE $BINARY_TARGET

