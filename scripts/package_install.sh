#!/bin/bash

# WORKING PACKAGE EXAMPLE
# ./scripts/package_create.sh
# sudo ./scripts/package_install.sh build/package_physimos.tar.gz /tmp/phys_install

echo ""
echo "Physimos Package Installation!"
echo ""


print_usage() {
    printf "Usage:  \n        sudo install.sh <package_path> <install_dir> \n"
    printf "Example:\n        sudo install.sh /build/package_physimos.tar.gz ~/.config/physimos \n\n"
}

verify_sudo() {
    
    # sudo -v
    
    if [ $(id -u) -ne 0 ]; then
        printf "Sudo privileges are required for installation. exiting. \n\n"
        exit 1;
    fi

    # Currently installs will be configured on a user basis.
    # Thus the current user will be set to the user that ran as root.
    if [ $USER == "root" ]; then
        INSTALL_USER=$SUDO_USER
    else
        INSTALL_USER=$USER
    fi
    printf "Installing as user '${INSTALL_USER}'.\n\n"
    
}

verify_arguments(){
    if [ -z "$PACKAGE_PATH" ] || [ -z "$INSTALL_DIR" ]; then
        printf "Error: Missing arguments. \n\n"
        print_usage
        exit 1
    fi
}

verify_package(){
    # VERIFY PACKAGE
    if [ ! -f $PACKAGE_PATH ]; then
        printf "Package not found. Exiting. \n\n"
        exit 1;
    fi
}

verify_install_dir(){
    if [ -d $INSTALL_DIR ]; then
        printf "Install directory already exists. Physimos-package installations can not overwrite another directory. \n\n"
        exit 1;
    fi
    mkdir -p $INSTALL_DIR
    chown -R $INSTALL_USER:$INSTALL_USER $INSTALL_DIR
}


unpack_package(){

    tar -xzf $PACKAGE_PATH -C $INSTALL_DIR
    if [ $? -ne 0 ]; then
        printf "Error unpacking package. Exiting. \n\n"
        exit 1;
    fi
    printf "Package unpacked to $INSTALL_DIR \n\n"
}

create_new_startup_script(){


    printf "#!/bin/bash\n\n" > ${STARTUP_SCRIPT}

    printf "PHYSIMOS_ROOT_DIR=${INSTALL_DIR}\n\n" >> ${STARTUP_SCRIPT}

    printf "cd ${INSTALL_DIR}\n" >> ${STARTUP_SCRIPT}
    printf "export PHYSIMOS_ROOT_DIR=${INSTALL_DIR}\n\n" >> ${STARTUP_SCRIPT}

    printf "${INSTALL_DIR}/physimos_main &\n\n" >> ${STARTUP_SCRIPT}

    chmod +x ${STARTUP_SCRIPT}

    printf "Startup script created at ${STARTUP_SCRIPT} \n\n"

}

move_startup_script_to_bin(){

    cp ${STARTUP_SCRIPT} ${BIN_PATH}
    printf "Startup script copied to ${BIN_PATH}\n\n"
    
}

create_env_file(){

    # print key paths to env file
    printf "PHYSIMOS_ROOT_DIR=${INSTALL_DIR}\n" > ${ENV_FILE}
    printf "BIN_PATH=${BIN_PATH}\n" >> ${ENV_FILE}

    printf "Environment file created at ${ENV_FILE} \n\n"
}



# CLI HELP
if [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
    print_usage
    exit 0
fi



# CLI ARGUMENTS
PACKAGE_PATH=$1
INSTALL_DIR=$2

# Set after sudo is verified
INSTALL_USER=

STARTUP_SCRIPT=${INSTALL_DIR}/"startup_script.sh"
BIN_PATH="/usr/bin/physimos"
ENV_FILE="${INSTALL_DIR}/installed.env"


# VERIFY CLI
verify_arguments
verify_sudo
verify_package
verify_install_dir

# PACKAGE
unpack_package
create_new_startup_script
move_startup_script_to_bin
create_env_file

printf "Installation complete! \n\n"

exit 0

