#pragma once

#ifdef  PH_WINDOWS
    #include <windows.h>
#elif   PH_LINUX
    #include <fcntl.h>     // open
    #include <unistd.h>    // read, close
    #include <iostream>    // std::cerr
    #include <cstring>     // strerror
    #include <errno.h>     // errno
#endif

#include <iostream>

#include <string>

#include "process.hh"
#include "log.hh"
#include "str.hh"

#include <fstream>
#include <filesystem>


struct File {


    std::string physimos_core_path_;
    std::string relative_path_;
    std::string file_contents_;
    File(std::string relative_path) : relative_path_ {relative_path} {
        std::string physiomos_dir = physimos_root_dir_or_die();
        physimos_core_path_ = physiomos_dir + "/" + relative_path;
    };



    bool use_core_path = false;
    bool successful_read = false;

    Str absolute_core_path;

    Str physimos_root_dir;
    Str relative_path_str;

    Str file_contents_str;

    File() = default;
    File(Str relative_path_str) : relative_path_str {relative_path_str} {

        Str physimos_root_dir = physimos_root_dir_or_die_str();
        absolute_core_path = physimos_root_dir + "/" + relative_path_str;
        
    };
    Str copy_as_str_core(){

        use_core_path = true;
        successful_read = false;

        Str str = cat_file_as_str();
        // Str str = cat_core_file_xplat();

        return str;
    }
    void make_core(){
        use_core_path = true;
    }
    void make_cwd(){
        use_core_path = false;
    }
    void update_core_path(){

    }
    void set_path_core(Str& path_str){

        // Str
        relative_path_str = path_str;
        physimos_root_dir = physimos_root_dir_or_die_str();
        absolute_core_path = physimos_root_dir + "/" + relative_path_str;

        // std::string
        std::string physimos_root_dir = physimos_root_dir_or_die();
        relative_path_ = relative_path_str.to_std_string();
        physimos_core_path_ = physimos_root_dir + "/" + relative_path_;
    }



    std::string copy_as_string_core(){
        use_core_path = true;
        successful_read = false;
        return cat_file_as_string();
    }
    std::string copy_as_string_cwd(){
        successful_read = false;
        return cat_file_as_string();
    }
    std::string& ref_as_string_cwd(){
        successful_read = false;
        file_contents_ = cat_file_as_string();
        return file_contents_;
    }


    Str cat_core_file_xplat();


private:

#ifdef  PH_WINDOWS
    Str cat_core_file_windows();
#elif   PH_LINUX
    Str cat_core_file_linux();
#endif


    std::string cat_file_as_string();
    Str cat_file_as_str();

};

