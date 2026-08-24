
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>

#include <stdlib.h>

#include <linux/input.h>
#include <linux/input-event-codes.h>

#include <iostream>

#include "lib/print.hh"

#include "hiddev.hh"


void add_current_user_to_input_group()
{
    // system("echo 123");
    system("sudo usermod -a -G input $USER");
}

void remove_current_user_to_input_group()
{
    // system("echo 123");
    system("sudo usermod -r -G input $USER");
}

void user_info()
{
    // system("echo 123");
    system("id");
}

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

    // listen_mouse_hiddev("/dev/usb/hiddev2");

    // Keyboard devices: "/dev/hidraw7", "/dev/input/event9" 
    // Problem: permission are 0XX0, thus a regular user cannot read the files..
    // Also, I don't know what format the data from /dev/input/eventX output


    // add_current_user_to_input_group();
    // remove_current_user_to_input_group();
    // user_info();


    // Trying to read /dev/input/event9
    // Source: https://stackoverflow.com/questions/15949163/read-from-dev-input
    if(true)
    {
        const char* file_path = "/dev/input/event9";
        int fd = open(file_path, O_RDONLY);
        if(fd < 0)
        {
            printf("Error: failed to open file %c", file_path);
            return 1;
        }
        
        struct input_event
        {
            struct timeval time;
            unsigned short type;
            unsigned short code;
            unsigned int value;
        };
        // sizeof(input_event);
        input_event ev;



        while(1)
        {
            int ret = read(fd, &ev, sizeof(input_event));
            if(ret <= 0)
            {
                print("failed to read \n");
                continue;
            }


            // print whole event object
            if(true)
            {
                // std::cout << "ev.code = " << ev.code << ", ev.type = " << ev.type << ", ev.value = " << ev.value << std::endl;
                // printf("read size = %d \n", ret);
            }

            // printf("read size = %d \n", ret);
            // printf("read ev.time = %d \n", ev.time);
            
            // printf("ev.type = %d \n", ev.type);
            // printf("ev.value = %d \n", ev.value);
            // printf("Code: %d \n", ev.code);
            
            if(ev.type == EV_KEY)
            {
                // print(" ");

                // printf("%d \n", ev.code);

                switch (ev.value)
                {
                    case 0:
                        printf("Key %d released \n", ev.code);
                        // print("released ");
                        break;
                    case 1:
                        // print("pressed ");
                        printf("Key %d pressed \n", ev.code);
                        break;
                    case 2:
                        // print("held ");
                        printf("Key %d held \n", ev.code);
                        break;
                    
                    default:
                        break;
                }
            }
            // printf("read ev.value = %d ", ev.value);
            // print("\n");

            std::cout << std::flush;

            // printf("%d", ev.value);
        }
    }

    print("~~ end main input ~~ \n");

    return 0;
}