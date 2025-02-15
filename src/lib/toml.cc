
#include <string>

#include <iostream>

#include <fstream>
#include <streambuf>
#include <sstream>

#include "fs.hh"
#include "string.hh"

#include "toml.hh"

namespace plib {




    void TOML::load(std::string file_path) {

        // std::cout << "111" << std::endl;
        root_properties.clear();

        TOML_Property current_table;

        std::string file_contents = plib::fs_cat(file_path);

        std::vector <std::string> file_lines = plib::std_string::split(file_contents, '\n');

        for(std::string line : file_lines){
            std::cout << line << std::endl;

            TOML_Property new_prop = parse_line(line);

            if(new_prop.type == TOML_Type::TABLE){
                current_table = new_prop;
            }
        }

    }

    TOML_Property TOML::find_root_property(std::string property_key) {

    }



    TOML_Property TOML::parse_line(std::string line){

        TOML_Property prop;
        prop.type = TOML_Type::EMPTY;
        prop.name = "";

        // NOTE: this is a general split, meaning that for example any equal-sign in the line that is not the KV-separator will trigger an unintended split!
        std::vector <std::string> line_kv_strings = plib::std_string::split(line, '=');

        std::string first_substr;
        std::string second_substr;

        // Empty line
        if (line_kv_strings.size() == 0){
            return prop;
        }
        else if (line_kv_strings.size() == 1){
            first_substr = line_kv_strings[0];
            prop.type == TOML_Type::TABLE;
        }
        else if (line_kv_strings.size() == 2) {
            first_substr = line_kv_strings[0];
            second_substr = line_kv_strings[1];
        }

        // since line_kv_strings.size() > 0, this is ok
        // std::string first_substr = line_kv_strings[0];


        plib::std_string::trim(first_substr);

        // Only whitespace in line
        if (first_substr.size() == 0)
            return prop;

        // Verify root property
        if (prop.type == TOML_Type::TABLE) {

            bool table_format_ok = first_substr[0] == '[' && first_substr[first_substr.size() - 1] == ']';

            if (!table_format_ok){
                prop.type = TOML_Type::EMPTY;
                return prop;
            }

        }
            

        // Root property
        if (first_substr[0] == '[') {
            if (first_substr[first_substr.size() - 1] == ']') {

                // NEW ROOT PROP

                // Always an array of properties
                prop.type = TOML_Type::ARRAY;
                prop.name = first_substr.substr(1, first_substr.size() - 2);
                prop.is_table = true;

                return prop;
            }
        }
        
        // NOT ROOT PROPERTY

        

        // No matches so this should be an empty property
        return prop;

    }


    TOML_Property parse_array(std::string array_str){

    }

}
