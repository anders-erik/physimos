#include <string>
#include <vector>

#include "lib/process.hh"
#include "lib/log.hh"

#include "../physon.hh"
#include "../physon_types.hh"

#include "../json.hh"

#include <filesystem>


struct Phile {

    std::string file_path_abs;

    std::string file_contents;

    Phile(std::string physimos_file_path) {

        std::string physiomos_dir = physimos_root_dir_or_die();
        file_path_abs = physiomos_dir + "/" + physimos_file_path;

        // file_contents = cat_file_as_string();
        // std::cout << file_contents << std::endl;
        
    };

    std::string copy_as_string(){
        return cat_file_as_string();
    }
    std::string& ref_as_string(){
        file_contents = cat_file_as_string();
        return file_contents;
    }

private:

    std::string cat_file_as_string(){

        std::string string;

        std::ifstream _ifstream(file_path_abs);
        if(!_ifstream.is_open()){
            std::string error_msg = "Phile: cat_file_as_string. Path: " + file_path_abs;
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
            std::string error_msg = "Phile: cat_file_as_string. Path: " + file_path_abs;
            plib::plog_error("CONFIG", "READ_FILE", error_msg);
        }
        
        return string;
    }
};

/** 
  The primary json config tool. Intended to be used exclusively as a base class.
  A new Derived class is created for each confuration type.
*/
struct JPhile {
    Phile file;
    Json json;

    JPhile(Phile& _file) : file {_file}, json {file.copy_as_string()} {
        json.lex_parse();
        // std::cout << json.serialize() << std::endl;
        
    };
};

