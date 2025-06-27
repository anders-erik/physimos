
#pragma once

#include "lib/str.hh"
#include "lib/print.hh"

#include <unistd.h> // Segfault handler
#include <csignal>
#include <cstring>

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
        print("✅  ");
    else
        print("❌  ");
}

void print_indent(int indent_count){
    for(auto i = indent_count; i >= 0; i--)
    {
        print(" ");
    }
}



bool str_equal_c_str_chars(Str str, const char* c_str){

    if(str.size() != strlen(c_str))
        return false;
    
    for(size_t i = 0; i < str.size(); i++)
        if(str[i] != c_str[i])
            return false;

    return true;
}
