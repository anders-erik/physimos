
#pragma once

#include "str.hh"



void print(const Str& str);
void println(const Str& str);
// void println(const Str&& str);
void println();

void print_e(const Str& str);
void println_e(const Str& str);

template <typename... Args>
void print_format(const Str& str, Args&&... args){
    
    println(str);

    // args;

}
template <typename T>
void println_any(T t){
    
    println(t);

}
template <typename... Args>
void println_list(Args&&... args){
    
    // println(args...);
    // args...[0];
    // args[0];


}

// Namespacing the printxxx-functions
struct Print {

    static inline void line(const Str& str){
        println(str);
    }

    template <typename T, typename... Args>
    static inline void format(const Str& str, Args&&... args){
        
    }

};