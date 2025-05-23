#pragma once

#include <string>
#include <vector>

#include "lib/process.hh"
#include "lib/log.hh"
#include "lib/phile.hh"

#include "../physon.hh"
#include "../physon_types.hh"

#include "../json.hh"

#include <filesystem>


/** 
  The primary json config tool. Intended to be used exclusively as a base class.
  A new Derived class is created for each confuration type.
*/
class JPhile {


protected:
    Phile file;
    Json json;

    bool verify_type_array(JsonVar& array, std::string error_msg);
    bool verify_type_object(JsonVar& object, std::string error_msg);
    bool json_equals_string(JsonVar& json_var_str, std::string target, std::string error_msg);


public:
    JPhile(std::string& str_path) : file {Phile(str_path)}, json {file.copy_as_string()} {
        json.lex_parse();
        // std::cout << json.serialize() << std::endl;
        
    };

    JPhile(Phile& _file) : file {_file}, json {file.copy_as_string()} {
        json.lex_parse();
        // std::cout << json.serialize() << std::endl;
        
    };

};


bool JPhile::verify_type_array(JsonVar& array, std::string error_msg){
    if(!array.is_array()){
        std::string msg = error_msg + " File: " + file.file_path_abs;
        plib::plog_error("JPHILE", "json_format", msg);
        return false;
    }
    return true;
}

bool JPhile::verify_type_object(JsonVar& object, std::string error_msg){
    if(!object.is_object()){
        std::string msg = error_msg + " File: " + file.file_path_abs;
        plib::plog_error("JPHILE", "json_format", msg);
        return false;
    }
    return true;
}


bool JPhile::json_equals_string(JsonVar& json_var_str, std::string target, std::string error_msg){
    if(json_var_str.get_string() != target){
        std::string msg = "JPhileShape: array entry is not of type shape. File: " +  file.file_path_abs;
        plib::plog_error("JPHILE", "json_format", msg);
        return false;
    }
    return true;
}