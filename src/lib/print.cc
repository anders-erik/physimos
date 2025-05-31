
#ifdef PH_LINUX
    #include <fcntl.h>     // open
    #include <unistd.h>    // read, close, write
// #elif PH_WINDOWS

#else
    #error Printing for current OS not supported. Stopping.
#endif


#include "str.hh"
#include "print.hh"




void print(const Str & str){

    if(!str.is_allocated())
        throw std::runtime_error("Tried to print uninitialized Str");

#ifdef PH_LINUX

    // write to stdout
    write(
        STDOUT_FILENO, 
        str.get_pointer(), 
        str.size()
    );

#endif

}

void println(const Str & str){
    print(str);
    print("\n");
}


void print_e(const Str & str){

    if(!str.is_allocated())
        throw std::runtime_error("Tried to print uninitialized Str");

#ifdef PH_LINUX

    // write to stdout
    write(
        STDERR_FILENO, 
        str.get_pointer(), 
        str.size()
    );

#endif

}

void println_e(const Str & str){
    print_e(str);
    print_e("\n");
}

