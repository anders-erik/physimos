
#include <string>

#include <iostream>

#include <fstream>
#include <streambuf>
#include <sstream>

#include "fs.hh"
#include "string.hh"

#include "toml.hh"

namespace plib {


    // START TOML

    void TOML::load(std::string file_path) {
        tables.clear();

        TOML_Table* new_table;
        TOML_String root_table_name = "ROOT_TABLE";
        std::vector <std::string>   current_table_lines = {};


        std::string                 file_contents = plib::fs_cat(file_path);
        std::vector <std::string>   file_lines = plib::std_string::split(file_contents, '\n');


        for (std::string line : file_lines) {

            bool is_table_label = is_table_label_line(line);

            if (is_table_label) {

                new_table = new TOML_Table(current_table_lines);

                tables.push_back(new_table);
            }

            current_table_lines.push_back(line);
        }


        // Make sure the last table is parsed
        if (current_table_lines.size() > 0) {

            TOML_Table* new_table = new TOML_Table(current_table_lines);

            tables.push_back(new_table);

            // All KVs before first table label is referred to as the root table per the specs
            tables[0]->name = root_table_name;
        }


    }


    TOML_Table* TOML::find_table(TOML_String table_name) {

        for( TOML_Table* table : tables){
            if (table_name == table->name)
                return table;
        }

        return nullptr;
    }

    TOML_Table& TOML::operator[](TOML_String table_name) {
        for (TOML_Table* table : tables) {
            if (table_name == table->name)
                return *table;
        }

        return *(tables[0]);
    }


    // END TOML

    // START TOML TABLE

    TOML_Table::TOML_Table(std::vector<std::string> lines) {

        // Will be overwritten if label-line found
        name = "_";

        for (std::string line : lines) {

            parse_line_and_push_kv(line);

        }
    }


    TOML_KV TOML_Table::operator[](TOML_String key_name) {

        for (TOML_KV& kv : kvs){
            if (key_name == kv.key)
                return kv;
        }
        return TOML_KV();
    }


    bool     TOML_Table::parse_line_and_push_kv(std::string line) {

        // TOML_Value new_value;
        TOML_String key;

        if (line.empty())
            return true;

        // No '=' found --> Table Label
        bool is_table_label = line.find('=') == std::string::npos;
        if (is_table_label) {

            pstring label_str = line;
            plib::std_string::trim(label_str);

            // bool table_format_ok = line[0] == '[' && line[line.size() - 1] == ']';
            // if (!table_format_ok)
            //     return false;

            TOML_String table_name_no_brackets = label_str.substr(1, label_str.size() - 2);

            this->name = table_name_no_brackets;
            return true;

        }



        // VERIFIED EQUAL SIGN FOR KV SPLIT AT THIS POINT

        // NOTE: this is a generic string splitting function
        // For example any equal-sign in the line that is not the KV-separator will trigger a split!
        std::vector <std::string> line_kv_strings = plib::std_string::split(line, '=');

        // First substring is the key
        std::string first_substr = line_kv_strings[0];
        plib::std_string::trim(first_substr);
        key = first_substr;


        // VALUE
        std::string second_substr = line_kv_strings[1];
        plib::std_string::trim(second_substr);

        // TOML_Value parsed_value = parse_toml_value(second_substr);
        TOML_Value toml_value = TOML_Value(second_substr);


        kvs.push_back(TOML_KV(key, toml_value));

        return true;
    }


    // END TOML TABLE

    
    
    // START TOML ARRAY

    TOML_Array::TOML_Array(pstring array_str_single_line) {

        // TOML_Array toml_array = {};

        pstring array_str_no_brackets = array_str_single_line.substr(1, array_str_single_line.size() - 2);

        std::vector<pstring> array_str_entries = plib::std_string::split(array_str_no_brackets, ',');

        for (pstring string_entry : array_str_entries) {
            plib::std_string::trim(string_entry);
            TOML_Value parsed_value = TOML_Value(string_entry);
            // toml_array.push_back(parsed_value);
            vector.push_back(parsed_value);
        }

        // return toml_array;

    }

    TOML_Value  TOML_Array::operator[](pstring value_string){

        for(TOML_Value vector_value : vector){
            TOML_Value new_value = TOML_Value(value_string);
            if (new_value == vector_value)
                return new_value;
        }

        return TOML_Value();
    }

    
    bool  TOML_Array::operator==(TOML_Array& rhs){

        if(this->vector.size() != rhs.vector.size())
            return false;
        
        for (size_t i = 0; i < this->vector.size(); i++)
        {
            if (!(this->vector[i] == rhs.vector[i]))
                return false;
        }
        
        return true;
    }

    std::vector<TOML_Value>& TOML_Array::get_vector() {
        return vector;
    }


    // END TOML ARRAY


    // START TOML VALUE


    TOML_Value::TOML_Value(pstring value_string) {

        // Type detection
        char first_char = value_string[0];
        switch (first_char)
        {
        case '"':
        {
            std::string str_without_quotes = value_string.substr(1, value_string.size() - 2);
            string = str_without_quotes;
            type = TOML_ValueType::STRING;
        }
        break;

        case '[':
        {
            // array = parse_array_single_line(value_string);
            array = TOML_Array(value_string);
            type = TOML_ValueType::ARRAY;
        }
        break;

        default:
            break;
        }

    }


    bool TOML_Value::operator==( TOML_Value& rhs) {
        bool same_type;
        bool same_value;

        switch (rhs.type){

            case TOML_ValueType::STRING :
                same_type = this->type == TOML_ValueType::STRING;
                same_value = this->string == rhs.string;
                break;

            case TOML_ValueType::ARRAY:
                same_type = this->type == TOML_ValueType::ARRAY;
                same_value = this->array == rhs.array;
                break;
            
            default:
                break;
        }

        return (same_type && same_value) ? true : false;
    }


    void TOML_Value::set_value(TOML_String string) {
        type = TOML_ValueType::STRING;
        this->string = string;
    }
    void TOML_Value::set_value(TOML_Array array) {
        type = TOML_ValueType::ARRAY;
        this->array = array;
    }


    // END TOML VALUE

    

    // START TOML KV

    TOML_KV::TOML_KV(TOML_String _key, TOML_Value _value) {
        key     = _key;
        value   = _value;
    }

    // Currently 3 types of lines: empty, table label, or KV containing a '='
    bool TOML::is_table_label_line(std::string line) {

        if (line.empty())
            return false;

        // No '=' found --> table
        bool is_table_label = line.find('=') == std::string::npos;

        return is_table_label;

    }

    // END TOML KV

}
