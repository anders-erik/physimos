
#pragma once

struct Str;



void print(const Str& str);
void println(const Str& str);

void print_e(const Str& str);
void println_e(const Str& str);


// Namespacing the printxxx-functions
struct Print {

    static inline void line(const Str& str){
        println(str);
    }

    template <typename T, typename... Args>
    static inline void format(const Str& str, Args&&... args){
        
    }

};