#pragma once

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "lib/print.hh"
#include "lib/str.hh"

struct IO
{

    static void dump(Str file_path, void* ptr, uint byte_count)
    {
        int fd, ret;

        fd = open(file_path.to_c_str(), O_WRONLY | O_CREAT);
        if(fd < 0)
        {
            println("ERROR: Failed to open bin_dump-file for writing.");
            return;
        }
        ret = fchmod(fd, 00777);


        ret = write(fd, ptr, byte_count);
        if(ret < 0)
        {
            println("ERROR: Failed to write to bin_dump-file.");
            return;
        }

        ret = close(fd);
        if(ret < 0)
        {
            println("ERROR: Failed to close bin_dump-file.");
            return;
        }
    }



    static void dump(Str file_path, Str _content)
    {
        int fd, ret;

        fd = open(file_path.to_c_str(), O_WRONLY | O_CREAT);
        if(fd < 0)
        {
            println("ERROR: Failed to open bin_dump-file for writing.");
            return;
        }
        ret = fchmod(fd, 00777);


        ret = write(fd, _content.data(), _content.size());
        if(ret < 0)
        {
            println("ERROR: Failed to write to bin_dump-file.");
            return;
        }

        ret = close(fd);
        if(ret < 0)
        {
            println("ERROR: Failed to close bin_dump-file.");
            return;
        }
    }




    static int echo(const char* _file_path_c, void* data, uint _data_length)
    {
        remove(_file_path_c);

        int fd = open(_file_path_c, O_WRONLY | O_CREAT );
        if(fd < 0)
        {
            Print::ln("Error: Failed to open file for echo function.");
            return -1;
        }
        fchmod(fd, 0777);


        int write_ret = write(fd, data, _data_length);
        if(write_ret < 0)
        {
            Print::ln("Error: Failed to write to file in echo function.");
            return -1;
        }


        fd = close(fd);
        if(fd < 0)
        {
            Print::ln("Error: Failed to close file in echo function.");
            return -1;
        }

        return 0;
    }



};