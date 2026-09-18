
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include "lib/str.hh"
#include "lib/print.hh"

#include "math/vecmat.hh"



struct command
{
    static Str run(Str command_str)
    {
        Str return_str;

        // FILE * file_FILE = popen(command_str.to_c_str(), "r");
        FILE *stream = popen(command_str.to_c_str(), "r");

        char buf[5000];

        while (fgets(buf, sizeof(buf), stream))
        {
            return_str += buf;
        }

        // char* read_popen = fgets(buf, sizeof(buf), file_FILE); // reads until EOL

        int pclose(FILE *stream);
        // return {};
        return return_str;
    }


    static Str piping()
    {
        int     fd[2];
        pid_t   childpid;

        int pipe_int = pipe(fd);

        if(pipe_int == 0)
        {
            Print::ln("Pipe command succesful!");
        }


        // Write to write-end of pipe
        char buf_write[10] = "Jello!";
        write(fd[1], buf_write, 7);

        // Read on recieving end of buffer!
        char buf_read[10];
        read(fd[0], buf_read, 7);

        Print::ln(buf_read);

        if((childpid = fork()) == -1)
        {
                perror("fork");
                return {};
        }

        if(childpid == 0)
        {
                /* Child process closes up input side of pipe */
                Print::ln("Child");
                close(fd[0]);
        }
        else
        {
                /* Parent process closes up output side of pipe */
                Print::ln("Parent");
                close(fd[1]);
        }

        return {};
    }
};


// i2 Fork()
// {

// }


struct ErrorNo
{
    static Str str(int _errno)
    {
        return Str(strerror(errno));
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
        Str strr = "Jello!";
        // ssize_t bytes_writen = write(fd[1], _str.to_c_str(), _str.size());
        // Print::buf("Bytes written = ");
        // Print::ln(Str::SI(bytes_writen));

        // char buf_write[10] = "Jello!";
        write(fd[1], _str.to_c_str(), 7);
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




        char buf_read[10];

        size_t read_size = 7;
        
        read(fd[0], buf_read, read_size);

        return Str{buf_read};
    }

    ~Pipe()
    {
        close(fd[0]);
        close(fd[1]);
    }
};

struct Fork
{
    enum SplitType
    {
        Parent,
        Child,
    } split_type;

    

    Fork()
    {
        
    }    
};

int main()
{
    Print::ln("Hello from main pico!");

    // Str pipe_ret = command::piping();

    // Print::buf(pipe_ret);
    // Print::buf("pipe_ret: \n");
    // Print::ln(pipe_ret);

    // Str stdout = command::run("echo hello");
    // Print::ln(stdout);

    // Str arduino_cli = command::run("arduino-cli");
    // Print::ln("");
    // Print::ln(arduino_cli);


    // ARDUINO::
    // https://docs.arduino.cc/arduino-cli/getting-started/

    bool download = false;


    Str echo_output = command::run("echo 'asdf'");
    // Print::ln(echo_output);


    Pipe pipe;

    pipe.Write("Looser!");
    Str pipe_read = pipe.Read();
    Print::ln(pipe_read);

    // Str pipe_read = pipe.Read();
    // printf("%s", pipe_read.to_c_str());
    
    // if(download)
    // {
    //     Str arduino_update_index = command::run("arduino-cli core update-index");
    //     Print::ln(arduino_update_index);

    //     Str arduino_install_arduino_avr = command::run("arduino-cli core install arduino:avr");
    //     Print::ln(arduino_update_index);
    // }

    // Str arduino_board_list = command::run("arduino-cli board list");
    // Print::ln(arduino_board_list);

    // chdir("./src/embed/arduino");

    
    // Str compile_1= "arduino-cli compile --fqbn arduino:avr:uno arduino.ino";
    // Str arduino_1 = command::run(compile_1);


    
    // Str upload_1= "arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno arduino.ino";
    // Str upload_1_str = command::run(upload_1);

    // Str compile_command = "arduino-cli compile -b arduino:avr:uno ./src/embed/arduino/arduino.ino";
    // Str arduino_compile = command::run(compile_command);

    // # Upload
    // arduino-cli upload -p /dev/ttyACM0 -b arduino:avr:uno ./MySketch

    return 0;
}