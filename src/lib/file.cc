
#ifdef  PH_LINUX
    #include "fs/linux.hh"
#elif   PH_WINDOWS
    #include "fs/windows.hh"
#else
    #error lib/file is not supported on current OS.
#endif


#include <iostream>
#include <fstream>
#include <filesystem>

#include "log.hh"

#include "file.hh"



File::File(Str relative_path_str)
    : relative_path {relative_path_str} 
{
};



Str& File::get_current_path(){
    if(path_type == path_t::core){
        update_absolute_path();
        return absolute_core_path;
    }
    else{
        return relative_path;
    }
}

const char* File::get_current_path_c_str(){

    return get_current_path().to_c_str();
    
}

void File::update_absolute_path(){

    physimos_root_dir = physimos_root_dir_or_die_str();
    absolute_core_path = physimos_root_dir + "/" + relative_path;

}

void File::set_rel_path(Str & path_str){
    relative_path = path_str;
}




bool File::core_exists(){

    path_type = path_t::core;

    // const char* path = get_current_path_c_str();
    const char* path = get_current_path().to_c_str();
    std::cout << std::string(path) << std::endl;
    Str path_str = get_current_path();
    // std::cout << path_str.to_std_string() << std::endl;

    return OS::file_exists(path);

}



void File::file_echo_overwrite_first_strsize_chars(Str str){

    path_type = path_t::core;
    update_absolute_path();

    bool successful = OS::file_echo_overwrite_first_strsize_chars( 
        get_current_path().to_c_str(),
        str
    );

    if(!successful){
        error = Err();
        error.severity = err_s::Error; // default
        error.module = err_m::Lib;
        error.type = err_t::ERRNO ;
        error.file = __FILE__;
        error.message = "file_echo_overwrite_first_strsize_chars(): Failed to write. ";
        log_error(error);
    }

}

Str File::cat_as_str_core_xplat() {

    path_type = path_t::core;
    update_absolute_path();

    // return cat_file_linux();
    // return OS::file_cat_as_str(get_current_path_c_str());
    return OS::file_cat_as_str(get_current_path().to_c_str());

}


Str File::cat_as_str_core_xplat(Str & path_str){

    File file (path_str);

    return file.cat_as_str_core_xplat();

}










// LEGACY BELOW







std::string File::cat_file_as_string(){

    std::string string;
    std::ifstream _ifstream;

    

    if(use_core_path)
        _ifstream.open(physimos_core_path_);
    else
        _ifstream.open(relative_path_);
    
    if(!_ifstream.is_open()){
        std::string error_msg = "File: cat_file_as_string. Path: " + physimos_core_path_;
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
        std::string error_msg = "File: cat_file_as_string. Path: " + physimos_core_path_;
        plib::plog_error("CONFIG", "READ_FILE", error_msg);
    }
    
    successful_read = true;
    return string;
}



Str File::cat_file_as_str(){

    std::string string = cat_file_as_string();
    return Str(string.c_str());

}




