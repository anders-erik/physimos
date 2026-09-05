
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include "lib/str.hh"
#include "lib/print.hh"





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


int main()
{
    Print::ln("Hello from main embed!");

    Str pipe_ret = command::piping();
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
    
    if(download)
    {
        Str arduino_update_index = command::run("arduino-cli core update-index");
        Print::ln(arduino_update_index);

        Str arduino_install_arduino_avr = command::run("arduino-cli core install arduino:avr");
        Print::ln(arduino_update_index);
    }

    Str arduino_board_list = command::run("arduino-cli board list");
    Print::ln(arduino_board_list);

    chdir("./src/embed/arduino");

    
    Str compile_1= "arduino-cli compile --fqbn arduino:avr:uno arduino.ino";
    Str arduino_1 = command::run(compile_1);


    
    Str upload_1= "arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno arduino.ino";
    Str upload_1_str = command::run(upload_1);

    // Str compile_command = "arduino-cli compile -b arduino:avr:uno ./src/embed/arduino/arduino.ino";
    // Str arduino_compile = command::run(compile_command);

    // # Upload
    // arduino-cli upload -p /dev/ttyACM0 -b arduino:avr:uno ./MySketch

    return 0;
}