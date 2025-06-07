
// Segfault handler
#include <iostream>
#include <csignal>
#include <cstdlib>
#include <unistd.h>

#include <string>
#include <vector>
#include <functional>

#include "str.hh"
#include "print.hh"

#include "test/test.hh"

#include "test/test_str.hh"


// FUNCTIONAL PLAYGROUND
std::function<bool()> lol = []() { println("HAHAHA"); return true; };
std::vector<std::function<int()>> fns = {

    []() -> bool { 
        println("f1");
        return true;
    },

    []() -> bool { 
        println("f2");
        return false;
    },

    lol
};






int main(){

    // lol();
    // for(std::function<void()> f : fns)
    //     f();


    println("\nStarting Lib tests.\n");


    // Signal handler for linux
    signal(SIGSEGV, segfault_handler);

    
    unit_tests_run("Str Constructors", constructors);
    unit_tests_run("Str Equality    ", equality);
    unit_tests_run("Str concat      ", concat);
    unit_tests_run("Str substr      ", substr);
    
    
    println("\nEnd Lib tests.\n");

    return 0;
}

