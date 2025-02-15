
#include <string>

#include <iostream>

#include <fstream>
#include <streambuf>
#include <sstream>

#include "fs.hh"
#include "string.hh"

#include "toml.hh"

namespace plib {


    TOML_Table* TOML::find_table(TOML_String table_name) {

        for( TOML_Table* table : tables){
            if (table_name == table->name)
                return table;
        }

        return nullptr;
    }


    void TOML_Value::set_value(TOML_String string) {
        type = TOML_ValueType::STRING;
        this->string = string;
    }
    void TOML_Value::set_value(TOML_Array array) {
        type = TOML_ValueType::ARRAY;
        this->array = array;
    }


    void TOML::load(std::string file_path) {

        tables.clear();


        std::string                 file_contents = plib::fs_cat(file_path);
        std::vector <std::string>   file_lines = plib::std_string::split(file_contents, '\n');

        // TOML_Table                  current_table = root_table;
        std::vector <std::string>   current_table_lines = {};


        TOML_KV parsed_line;
        TOML_Table* new_table;

        TOML_String current_table_name;
        TOML_String root_table_name = "ROOT_TABLE";

        for(std::string line : file_lines){
            // std::cout << line << std::endl;
            
            current_table_lines.push_back(line);

            parsed_line = parse_line(line);

            // Parse table if a new table indentifier is encountered
            if(parsed_line.value.type == TOML_ValueType::TABLE){

                // Do not include the next table label
                current_table_lines.pop_back();

                new_table = parse_table(current_table_lines);

                if (root_table == nullptr) {
                    root_table = new_table;
                }
                else {
                    new_table->name = parsed_line.key;
                }

                tables.push_back(new_table);
                
                // Reset table lines
                current_table_lines.clear();
                current_table_lines.push_back(line);
                // set name of new table that triggered parsing of preivous table
                current_table_name = parsed_line.key;
            }


        }

        // Make sure the last table is parsed
        if (true) {
            TOML_Table* new_table = parse_table(current_table_lines);

            if (root_table == nullptr) {
                root_table = new_table;
                root_table->name = root_table_name;
            }
            else {
                new_table->name = current_table_name;
            }

            tables.push_back(new_table);
        }



        std::cout << "TOML LOADING DONE" << std::endl;

    }

    // Parse KV lines of table, including the table label itself ([table_name])
    // Returns nullptr on empty table
    TOML_Table* TOML::parse_table(std::vector<std::string> table_lines) {
        
        TOML_Table* return_table = new TOML_Table();

        // Usually its the root table that contains no lines
        if (table_lines.size() == 0){
            return return_table;
        }

        for (std::string line : table_lines){

            TOML_KV parsed_prop = parse_line(line);
            
            // Table name
            if (parsed_prop.value.type == TOML_ValueType::TABLE){
                return_table->name = parsed_prop.key;
                continue;
            }

            // Empty line within table is skipped
            if (parsed_prop.key == ""){
                continue;
            }
            
            return_table->kvs.push_back(parsed_prop);

        }

        return return_table;

    }

    // Currently 3 types of lines: empty, table label, or KV containing a '='
    TOML_KV TOML::parse_line(std::string line) {

        TOML_KV toml_kv;
        toml_kv.value.type = TOML_ValueType::EMPTY;
        toml_kv.key = "";

        if (line.empty())
            return toml_kv;

        // No '=' found --> table
        bool is_table_label = line.find('=') == std::string::npos;
        if (is_table_label) {

            toml_kv.value.type = TOML_ValueType::TABLE;
            pstring label_str = line;
            plib::std_string::trim(label_str);
            toml_kv.key = label_str.substr(1, label_str.size() - 2);

            bool table_format_ok = line[0] == '[' && line[line.size() - 1] == ']';

            // if (!table_format_ok) {
            //     prop.type = TOML_Type::EMPTY;
            //     prop.name = "";
            // }

            return toml_kv;
        }



        // VERIFIED EQUAL SIGN FOR KV SPLIT AT THIS POINT

        // NOTE: this is a generic string splitting function
        // For example any equal-sign in the line that is not the KV-separator will trigger a split!
        std::vector <std::string> line_kv_strings = plib::std_string::split(line, '=');

        // First substring is the key
        std::string first_substr = line_kv_strings[0];
        plib::std_string::trim(first_substr);
        toml_kv.key = first_substr;


        // VALUE
        std::string second_substr = line_kv_strings[1];
        plib::std_string::trim(second_substr);

        TOML_Value parsed_value = parse_toml_value(second_substr);
        toml_kv.value = parsed_value;


        return toml_kv;

    }



    TOML_Array TOML::parse_single_line_array(pstring array_str) {

        TOML_Array toml_array = {};

        pstring array_str_no_brackets = array_str.substr(1, array_str.size() - 2);

        std::vector<pstring> array_str_entries = plib::std_string::split(array_str_no_brackets, ',');

        for (pstring string_entry : array_str_entries){
            plib::std_string::trim(string_entry);
            TOML_Value parsed_value = parse_toml_value(string_entry);
            toml_array.push_back(parsed_value);
        }

        return toml_array;
    }


    TOML_Value TOML::parse_toml_value(std::string value_str) {

        
        TOML_Value toml_value;

        // TYPE DETECTION
        char first_char = value_str[0];

        switch (first_char)
        {
        case '"':
            {
                std::string str_without_quotes = value_str.substr(1, value_str.size() - 2);
                toml_value.set_value(str_without_quotes);
            }
            break;

        case '[':
            {
                TOML_Array toml_array = parse_single_line_array(value_str);
                toml_value.set_value(toml_array);
            }
            break;

        default:
            break;
        }

        return toml_value;

    }


}
