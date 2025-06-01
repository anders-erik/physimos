#include <vector>
#include <iostream>
#include <chrono>

#include "file.hh"
#include "print.hh"



void file_str(){

    Str path_core = "src/lib/main_file.cc";

    // Object
    File file;
    file.set_path_core(path_core);
    Str this_file_contents_object = file.cat_as_str_core_xplat();

    // Static
    Str this_file_contents_static = File::cat_as_str_core_xplat(path_core);

    // println(this_file_contents_object);
    println(this_file_contents_static);

}

int main(){
    print("Starting: ");
    println(__FILE__);
    println();

    
    file_str();

    println("\nmain_file end.");
    return 0;
}