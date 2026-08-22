
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>


#include "lib/print.hh"

#include "hiddev.hh"

int main()
{
    print("~~ main input ~~ \n");

    // list linux devices
    DIR* dir = opendir("/dev");

    dirent* drent = readdir(dir);

    DIR* dirp_dir = opendir("/dev");
    // https://man7.org/linux/man-pages/man3/dirfd.3.html
    // >> Great concept to manually check for potential error after action.
    int dirfd_int =  dirfd(dirp_dir);
    int dirfd_close = close(dirfd_int);

    struct dirent *entry;

    while ((entry = readdir(dir))) {
        if (entry->d_name[0] == '.')
            continue;

        // printf("%s\n", entry->d_name);
    }

    int close_dir_ret = closedir(dir);

    listen_mouse_hiddev("/dev/usb/hiddev2");

    // Keyboard devices: "/dev/hidraw7", "/dev/input/event9" 
    // Problem: permission are 0XX0, thus a regular user cannot read the files..
    // Also, I don't know what format the data from /dev/input/eventX output

    print("~~ end main input ~~ \n");

    return 0;
}