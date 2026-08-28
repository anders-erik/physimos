
#pragma once

#include <unistd.h>
#include <stdio.h>

// #include <stdlib.h>

#include <linux/input.h>
#include <linux/input-event-codes.h>

#include <iostream>

#include "lib/print.hh"



class EvdevReader
{
    Str file_path;
    int fd = -1;

public: 

    input_event ev;

    EvdevReader(Str _file_path) : file_path {_file_path}
    {
        open_file();
    };


    int open_file()
    {
        fd = open(file_path.to_c_str(), O_RDONLY | O_NONBLOCK);
        if(fd < 0)
        {
            printf("Error: failed to open file %c \n", file_path.to_c_str());
            return 1;
        }

        return 0;
    }

    bool keypress_detected()
    {
        bool ret_value = false;
        int read_byte_count = 1; // Make sure we always enter loop at least once (do-while-esqe)

        while(1)
        {
            read_byte_count = read(fd, &ev, sizeof(input_event));

            if(read_byte_count == -1)
            {
                break;
            }

            if(ev.type == EV_KEY)
            {
                

                switch (ev.value)
                {
                    case 0:
                        printf("Key %d released \n", ev.code);
                        break;
                    case 1:
                        printf("Key %d pressed \n", ev.code);
                        ret_value = true;
                        break;
                    case 2:
                        printf("Key %d held \n", ev.code);
                        break;
                    
                    default:
                        break;
                }
            }
        }

        return ret_value;
    }

    void read_and_print()
    {
        int read_byte_count = 1; // Make sure we always enter loop at least once (do-while-esqe)

        while(1)
        {
            read_byte_count = read(fd, &ev, sizeof(input_event));
            if(read_byte_count == -1)
            {
                return;
            }

            if(ev.type == EV_KEY)
            {

                switch (ev.value)
                {
                    case 0:
                        printf("Key %d released \n", ev.code);
                        break;
                    case 1:
                        printf("Key %d pressed \n", ev.code);
                        break;
                    case 2:
                        printf("Key %d held \n", ev.code);
                        break;
                    
                    default:
                        break;
                }
            }
        }
    }

    ~EvdevReader()
    {
        int ret = close(fd);
        if(ret < 0)
        {
            printf("Error: failed to close file %c \n", file_path.to_c_str());
        }
    }

};
