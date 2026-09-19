#pragma once

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include "lib/str.hh"

namespace Unix
{

Str Popen(Str _command_str)
{
    Str return_str;

    // FILE * file_FILE = popen(command_str.to_c_str(), "r");
    FILE *stream = popen(_command_str.to_c_str(), "r");

    char buf[5000];

    while (fgets(buf, sizeof(buf), stream))
    {
        return_str += buf;
    }

    // char* read_popen = fgets(buf, sizeof(buf), file_FILE); // reads until EOL

    int pclose(FILE *stream);
    // return {};
    return return_str;
};



struct ErrorNo
{
    static Str str(int _errno)
    {
        return Str(strerror(_errno));
    }
};

struct Pipe
{
    // i2 fd;
    int fd[2];

    // int write_fd() { return fd[1]; }
    // int read_fd() { return fd.x; }

    // int read_fd() {return fd.x;}
    // int write_fd() {return fd.y;}

    Pipe()
    {
        // int fd_array[2];
        int pipe_ret = pipe(fd);

        // fd = {fd_array[0], fd_array[1]};

        if(pipe_ret == 0)
        {
            Print::ln("Pipe command succesful!");
        }
        else
        {
            Print::ln("Pipe command NOT succesful!");
        }


        // char buf_write[10] = "Jello!";
        // write(fd[1], buf_write, 7);

        // // Read on recieving end of buffer!
        // char buf_read[10];
        // read(fd[0], buf_read, 7);

        // Print::ln(buf_read);

    }

    void Write(Str _str)
    {
        // Str str = "Jello!\n";

        const char* str_c = _str.to_c_str();
        // size_t str_c_size = str.size();
        size_t str_c_size = strlen(str_c);

        // ssize_t bytes_writen = write(fd[1], _str.to_c_str(), _str.size());
        // Print::buf("Bytes written = ");
        // Print::ln(Str::SI(bytes_writen));

        // char buf_write[10] = "Jello!";
        write(fd[1], str_c, str_c_size);
    }

    Str Read()
    {
        // Str return_str;

        // int BUF_SIZE = 101;
        // char buf[BUF_SIZE] {0x00};
        // // buf[BUF_SIZE-1] = 0x00;

        // ssize_t bytesread = 0;

        // while ( (bytesread = read( fd[0], buf, BUF_SIZE-1)) > 0)
        // {
        //     return_str += buf;
        // }

        // // get errorno
        // if (bytesread < 0)
        // {
        //     Print::buf("Error: ");
        //     Print::ln(ErrorNo::str(bytesread));
        // }
        // else
        // {
        //     Print::buf("Bytes read: ");
        //     Print::ln(Str::SI(bytesread));
        // }


        // return return_str;

        close(fd[1]);

        Str return_string = "";


        size_t BUF_SIZE = 100;
        char buf[BUF_SIZE];
        
        ssize_t bytes_read = 0;

        while((bytes_read = read(fd[0], buf, BUF_SIZE)) > 0)
        {
            return_string += Str{(const char*)buf, (uint)bytes_read};

            Print::buf("Bytes read: " );
            Print::ln(Str::SI(bytes_read));

        }
        
        // Str return_string { (const char*)buf, 
        //                     (uint)bytes_read    };

        return return_string;
    }

    ~Pipe()
    {
        close(fd[0]);
        close(fd[1]);
    }
};



struct Process
{
    
};

}