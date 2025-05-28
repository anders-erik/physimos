#pragma once

#include <string>
#include <vector>

#include "lib/process.hh"
#include "lib/log.hh"
#include "lib/file.hh"

#include "../physon.hh"
#include "../physon_types.hh"

#include "../json.hh"

#include <filesystem>


/** 
  The primary json config tool. Intended to be used exclusively as a base class.
  A new Derived class is created for each confuration type.
*/
class JsonFile {


protected:
    File file;
    Json json;

    bool verify_type_array(JsonVar& array, std::string error_msg);
    bool verify_type_object(JsonVar& object, std::string error_msg);
    bool json_equals_string(JsonVar& json_var_str, std::string target, std::string error_msg);


public:
    JsonFile(std::string& str_path) : file {File(str_path)}, json {file.copy_as_string_cwd()} {
        json.lex_parse();
        // std::cout << json.serialize() << std::endl;
        
    };

    JsonFile(File& _file) : file {_file}, json {file.copy_as_string_cwd()} {
        json.lex_parse();
        // std::cout << json.serialize() << std::endl;
        
    };

};


bool JsonFile::verify_type_array(JsonVar& array, std::string error_msg){
    if(!array.is_array()){
        std::string msg = error_msg + " File: " + file.physimos_core_path;
        plib::plog_error("JPHILE", "json_format", msg);
        return false;
    }
    return true;
}

bool JsonFile::verify_type_object(JsonVar& object, std::string error_msg){
    if(!object.is_object()){
        std::string msg = error_msg + " File: " + file.physimos_core_path;
        plib::plog_error("JPHILE", "json_format", msg);
        return false;
    }
    return true;
}


bool JsonFile::json_equals_string(JsonVar& json_var_str, std::string target, std::string error_msg){
    if(json_var_str.get_string() != target){
        std::string msg = "JFileShape: array entry is not of type shape. File: " +  file.physimos_core_path;
        plib::plog_error("JPHILE", "json_format", msg);
        return false;
    }
    return true;
}