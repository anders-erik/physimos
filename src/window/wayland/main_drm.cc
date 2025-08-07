#include <fcntl.h>
#include <unistd.h>
#include <xf86drm.h>        // libdrm-dev package
#include <xf86drmMode.h>    // libdrm-dev package
#include <iostream>
#include <dirent.h>
#include <cstring>


void list_dri_cards(){

    const char* dri_path = "/dev/dri/";
    DIR* dir = opendir(dri_path);
    if (!dir) {
        perror("Failed to open /dev/dri");
    }
    else {
    
        struct dirent* entry;
        while ((entry = readdir(dir))) {
            if (strncmp(entry->d_name, "card", 4) == 0) {
                std::string full_path = std::string(dri_path) + entry->d_name;
                std::cout << full_path << std::endl;
                
            }
        }

    }

}

void open_card(const char* path){

    int fd = open(path, O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        // const char* error_msg = "Failed to open DRM device";
        // perror(error_msg);
        printf("ERROR: Failed to open %s.\n", path);
    }
    else {
        printf("Opened DRM device %s.\n", path);
    }

    close(fd);
}

int get_card(std::string card_name){
    // char* dir_str;
    std::string dir_str = "/dev/dri/";
    std::string path = dir_str + card_name;
    const char* path_c = path.c_str();
    // const char* dir_str_const = "/dev/dri";
    // dir_str = strcpy(dir_str, dir_str_const);

    // char* path = strcat(card_name, card_name);
    int fd = open(path_c, O_RDWR | O_CLOEXEC);

    return fd;
}

int main(){

    std::cout << "DRM" << std::endl;


    list_dri_cards();


    open_card("/dev/dri/card0");
    open_card("/dev/dri/card1");
    open_card("/dev/dri/card2");
    
    int card1_fd = get_card("card1");
    if(card1_fd < 0){
        printf("card1_fd not valid file descriptor.");
        return 1;
    }



    // LIBDRM

    // Get DRM resources
    drmModeRes* res = drmModeGetResources(card1_fd);
    if (!res) {
        perror("drmModeGetResources failed");
        close(card1_fd);
        return 1;
    }

    std::cout << "Connectors found: " << res->count_connectors << std::endl;
    
    for (int i = 0; i < res->count_connectors; ++i) {
        drmModeConnector* conn = drmModeGetConnector(card1_fd, res->connectors[i]);
        if (!conn) 
            continue;

        std::cout << "Connector " << i
                  << ": ID=" << conn->connector_id
                  << ", Type_name=" << drmModeGetConnectorTypeName(conn->connector_type)
                  << ", Connected=" << (conn->connection == DRM_MODE_CONNECTED)
                  << ", w(mm), h(mm)=" << conn->mmWidth << "x" << conn->mmHeight
                  << std::endl;

        drmModeFreeConnector(conn);
    }

    drmDevicePtr devices[8];
    int num_devices = drmGetDevices2(0, devices, 8);
    for (int i = 0; i < num_devices; i++) {
        printf("Vendor: %04x, Device: %04x\n",
            devices[i]->deviceinfo.pci->vendor_id,
            devices[i]->deviceinfo.pci->device_id);
        // printf("DRM Driver: %s\n", devices[i]->driver);
    }

    drmModeFreeResources(res);
    close(card1_fd);
    

    return 0;
}