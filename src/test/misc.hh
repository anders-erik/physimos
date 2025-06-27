
#pragma once



#include <unistd.h> // Segfault handler
#include <csignal>

#include <iostream>


#define trycatch(_x_) println("trycatch_x_");

#define try_    try{
#define catch_  }catch(const std::exception& e){
#define end_    }


#ifdef PH_LINUX
/** Linux */
void segfault_handler(int signal) {
    const char msg[] = "Segmentation fault!.\n";
    write(STDERR_FILENO, msg, sizeof(msg) - 1); // write() for async-signal-safety
    _exit(1);
}
#endif

void print_check(bool passed)
{
    if(passed)
        std::cout << "✅  ";
    else
        std::cout << "❌  ";
}

void print_indent(int indent_count){
    for(auto i = indent_count; i >= 0; i--)
    {
        std::cout << " ";
    }
}


