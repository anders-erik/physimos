
#include <iostream>
#include <string>

#include "log.hh"

#include "file.hh"
#include "print.hh"



void cat(){

    Str path_core = "src/lib/main_file.cc";

    // Object
    File file;
    file.set_path_core(path_core);
    ResMove<Str> this_file_contents_object = file.cat_as_str_core_xplat();
    // println(this_file_contents_object);

    // Static
    ResMove<Str> this_file_contents_static = File::cat_as_str_core_xplat(path_core);
    if(this_file_contents_static.has_value())
        println(this_file_contents_static.consume_value());

}

void echo(){

    Str path_core = "logs/test.txt";
    // Str path_core = "logs/main.log";

    File file;
    // file.set_path_core(path_core);
    file.set_rel_path(path_core);

    println(path_core);
    
    if(file.core_exists())
        println("EXISTS");
    else
        println("NOT EXISTS");


    file.file_echo_overwrite_first_strsize_chars("HolaHola");

}


int main(){
    print("Starting: ");
    println(__FILE__);
    println();

    
    cat();
    // echo();

    println("\nmain_file end.");

    // LOG
    

    // Test std::string
    // std::string substr_check = "abcd";
    // std::cout << substr_check.size() << std::endl;
    // std::string broken_substr = substr_check.substr(5, 50);
    // std::cout << broken_substr << std::endl;
    // std::cout << broken_substr.size() << std::endl;
    

    return 0;
}