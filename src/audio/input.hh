
#pragma once

#include <unistd.h>
#include <stdio.h>

// #include <stdlib.h>

#include <linux/input.h>
#include <linux/input-event-codes.h>

#include <iostream>

#include "lib/print.hh"


enum class Key
{
    A,
    S,
    D,
    F,
    G,
    H,
    J,
    K,
    UNKNOWN,
};

Key get_key_from_key_code(unsigned short key_code)
{
    Key key;

    switch (key_code)
    {
    case 30:
        key = Key::A;
        break;
    case 31:
        key = Key::S;
        break;
    case 32:
        key = Key::D;
        break;
    case 33:
        key = Key::F;
        break;
    case 34:
        key = Key::G;
        break;
    case 35:
        key = Key::H;
        break;
    case 36:
        key = Key::J;
        break;
    case 37:
        key = Key::K;
        break;
    
    default:
        key = Key::UNKNOWN;
        break;
    }

    return key;
}


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


    Arr<Key> get_key_presses()
    {
        Arr<Key> keys;

        bool ret_value = false;
        int read_byte_count = 1; // Make sure we always enter loop at least once (do-while-esqe)

        while(1)
        {
            read_byte_count = read(fd, &ev, sizeof(input_event));

            // End of available input data
            if(read_byte_count == -1)
            {
                break;
            }

            // Only return true on key*PRESS* (ev.value == 1)
            if(ev.type == EV_KEY && ev.value == 1)
            {
                // ret_value = true;
                // printf("Key %d pressed \n", ev.code);
                keys.push_back(get_key_from_key_code(ev.code));
            }
        }

        return keys;
    }


    bool keypress_detected()
    {
        bool ret_value = false;
        int read_byte_count = 1; // Make sure we always enter loop at least once (do-while-esqe)

        while(1)
        {
            read_byte_count = read(fd, &ev, sizeof(input_event));

            // End of available input data
            if(read_byte_count == -1)
            {
                break;
            }

            // Only return true on key*PRESS* (ev.value == 1)
            if(ev.type == EV_KEY && ev.value == 1)
            {
                ret_value = true;
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
