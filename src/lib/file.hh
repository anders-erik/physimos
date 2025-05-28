#pragma once

#include <string>

#include "lib/process.hh"
#include "lib/log.hh"

#include <filesystem>


struct File {

    std::string physimos_core_path;
    std::string relative_path;

    bool use_core_path = false;

    std::string file_contents;
    bool successful_read = false;

    File(std::string relative_path) : relative_path {relative_path} {

        std::string physiomos_dir = physimos_root_dir_or_die();
        physimos_core_path = physiomos_dir + "/" + relative_path;

        // file_contents = cat_file_as_string();
        // std::cout << file_contents << std::endl;
        
    };

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
        file_contents = cat_file_as_string();
        return file_contents;
    }

private:

    std::string cat_file_as_string(){

        std::string string;
        std::ifstream _ifstream;

        

        if(use_core_path)
            _ifstream.open(physimos_core_path);
        else
            _ifstream.open(relative_path);
        
        if(!_ifstream.is_open()){
            std::string error_msg = "File: cat_file_as_string. Path: " + physimos_core_path;
            plib::plog_error("CONFIG", "OPEN_FILE", error_msg);
            return string;
        }

        try
        {
            std::stringstream buffer;
            buffer << _ifstream.rdbuf();
            string = buffer.str();
        }
        catch(const std::exception& e)
        {
            std::string error_msg = "File: cat_file_as_string. Path: " + physimos_core_path;
            plib::plog_error("CONFIG", "READ_FILE", error_msg);
        }
        
        successful_read = true;
        return string;
    }
};

