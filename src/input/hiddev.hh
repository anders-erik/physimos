

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/hiddev.h>



void listen_mouse_hiddev(const char* file_name)
{
    
        int fd = open(file_name, O_RDONLY);
        if (fd < 0) {
            perror("Failed to open hiddev device");
            return;
        }

        struct hiddev_devinfo dinfo;
        if (ioctl(fd, HIDIOCGDEVINFO, &dinfo) < 0) {
            perror("HIDIOCGDEVINFO failed");
            close(fd);
            return;
        }

        printf("Vendor ID:  0x%04X\n", dinfo.vendor);
        printf("Product ID: 0x%04X\n", dinfo.product);
        printf("Version:    0x%04X\n", dinfo.version);
        printf("Num Applications: %d\n", dinfo.num_applications);

        // Read incoming raw HID events
        struct hiddev_event ev;
        printf("Waiting for HID events (press Ctrl+C to exit)...\n");


        int up_down = 0;
        int left_right = 0;
        int scroll = 0;

        while (read(fd, &ev, sizeof(ev)) == sizeof(ev))
        {
            // printf("Event: HID 0x%08X, Value: %d\n", ev.hid, ev.value);

            if(ev.hid == 0x00010030)
            {
                if(ev.value == 1)
                    printf("Move moved right\n");
                else if(ev.value == -1)
                    printf("Move moved left\n");

                left_right += ev.value;
                printf("left_right = %i \n", left_right);
            }
            if(ev.hid == 0x00010031)
            {
                if(ev.value == 1)
                    printf("Move moved down\n");
                else if(ev.value == -1)
                    printf("Move moved up\n");
                
                up_down += ev.value;
                printf("up_down = %i \n", up_down);
            }
            if(ev.hid == 0x00010038)
            {
                // if(ev.value == 1)
                //     printf("scolled up\n");
                // else if(ev.value == -1)
                //     printf("scolled down\n");

                // printf("scroll event value = %d \n", ev.value);
                
                scroll += ev.value;

                printf("scroll = %i \n", scroll);
            }
        }

        close(fd);

}