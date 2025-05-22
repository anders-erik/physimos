#!/bin/bash

# WORKING PACKAGE EXAMPLE
# ./scripts/package_create.sh
# sudo ./scripts/package_install.sh build/package_physimos.tar.gz /tmp/phys_install

echo ""
echo "Physimos Uninstall!"
echo ""


verify_sudo() {

    if [ $(id -u) -ne 0 ]; then
        printf "    Error: sudo privileges are required for uninstalling. exiting. \n\n"
        exit 1;
    fi
    
}

verify_uninstall_dir(){

    # Check if binary exists
    if [ ! -f ${BIN_PATH} ]; then
        printf "    Error: Physimos startup script not found at ${BIN_PATH}. \n\n"
        exit 1
    fi

    # Grab install directory from startup script
    INSTALL_DIR=$(cat ${BIN_PATH} | grep -P '^PHYSIMOS_ROOT_DIR=' | cut -d'=' -f2 )

    if [ ! -d "$INSTALL_DIR" ]; then
        printf "    Error: Install directory not found in startup script. \n\n"
        exit 1
    fi

    printf "Current installation at: '${INSTALL_DIR}' \n\n"
}



confirm_uninstall(){

    printf "Do you want to uninstall physiomos? [yes/no] " 1>&2
	read -r YN
	case "$YN" in
        # single letter or full word
		[Yy] | [Yy][Ee][Ss] )
            printf "Uninstalling.\n\n"
        ;;
		* )
            printf "Physimos not uninstalled. \n\n"
			exit 1
		;;
	esac

}

delete_startup_script_from_bin(){

    printf "Deleting startup script from ${BIN_PATH}.\n\n"
    rm -f ${BIN_PATH}
    if [ $? -ne 0 ]; then
        printf "    Error: Unable to delete startup script from ${BIN_PATH}. \n\n"
        exit 1
    fi
    
}

remove_install_dir(){

    printf "Deleting install directory ${INSTALL_DIR}.\n\n"

    rm -rf ${INSTALL_DIR}
    if [ $? -ne 0 ]; then
        printf "    Error: Unable to delete install directory ${INSTALL_DIR}. \n\n"
        exit 1
    fi
}



BIN_PATH="/usr/bin/physimos"
INSTALL_DIR=


# VERIFY CLI
verify_sudo
verify_uninstall_dir
confirm_uninstall
delete_startup_script_from_bin
remove_install_dir

printf "Physimos Uninstalled. \n\n"

exit 0

