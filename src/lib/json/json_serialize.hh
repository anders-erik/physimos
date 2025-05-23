#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip> // setprecision
#include <climits> // LLONG_MAX

#include "physon_types.hh"
#include "json_store.hh"
#include "json_variant.hh"


/** They type of whitespace output of th serializer */
enum class serial_ws : int {
    minimized,
    oneline,
    new_lines,
};

/** Floating point representation format. */
enum class float_representation {
    fixed,          // 0.0 -> 0.0000000     (precision == 7)
    fixed_trimmed,  // 0.0 -> 0.0           (precision == 7)
    scientific,     // 0.0 -> 0.0000000e+00 (precision == 7)
};

struct FloatFormat {
    float_representation rep = float_representation::scientific; // representation
    size_t precision = 8; // Number of digits past decimal point
    FloatFormat() {};
    FloatFormat(float_representation rep, size_t precision): rep{rep}, precision {precision} {};
};

/** Json Serialization confguration object.  */
struct JSerialConfig {
    serial_ws whitespace = serial_ws::new_lines;
    size_t indent_space_count = 4;
    FloatFormat float_format;

    JSerialConfig() {};
    JSerialConfig(serial_ws _whitespace) : whitespace {_whitespace}{};
    JSerialConfig(size_t _indent_space_count) : indent_space_count {_indent_space_count} {};
    JSerialConfig(serial_ws _whitespace, size_t _indent_space_count) : whitespace {_whitespace}, indent_space_count {_indent_space_count} {};
    JSerialConfig(serial_ws _whitespace, size_t _indent_space_count, FloatFormat float_format) : whitespace {_whitespace}, indent_space_count {_indent_space_count}, float_format {float_format} {};
};


class JsonSerializer {

    JSerialConfig config;
    int recursive_depth = 0;

    /** Supports different precision and scientific notation */
    static std::string float_to_json_representation(json_float float_, FloatFormat float_format);
    /** Converts a std::string to is JSON equivelence. e.g. <I "mean" it..> --> <"I \"mean\" it.."> */
    static std::string string_to_json_representation(std::string cpp_string);

    std::string result_string = "";
    void build_string(JsonWrapper value, json_store& store);

    std::string build_string(JsonVar json_var);

    static void add_spaces(std::string& str, int space_count);
    void try_add_new_line(std::string& str);
    void try_add_indent(std::string& str);
    void try_remove_trailing_comma_object(std::string& str);
    void try_remove_trailing_comma_array(std::string& str);
    void add_trailing_comma_to_object(std::string& str);
    void add_trailing_comma_to_array(std::string& str);


public:

    JsonSerializer() {};
    JsonSerializer(JSerialConfig config): config { config } {};

    /** returns full json structure as string. Creates a copy of wrapper and store store for now.  */
    std::string serialize(JsonWrapper root_wrapper, json_store& store);
    /** Returns full json structure as string.  */
    std::string serialize(JsonVar& json_root);



    void set_config(JSerialConfig config) {this->config = config;};
    
};

void JsonSerializer::add_spaces(std::string& str, int count){
    for(int i = 0; i < count; i++)
        str += " ";
}

void JsonSerializer::try_add_new_line(std::string& str){
    if(config.whitespace == serial_ws::new_lines)
        str += "\n";
}
void JsonSerializer::try_add_indent(std::string& str){
    if(config.whitespace == serial_ws::new_lines)
        add_spaces(str, recursive_depth*config.indent_space_count);
}
void JsonSerializer::add_trailing_comma_to_object(std::string& str){
    
    if(config.whitespace == serial_ws::minimized){
        str += ",";
    }
    else if (config.whitespace == serial_ws::oneline){
        str += ", ";
    }
    else if (config.whitespace == serial_ws::new_lines){
        str += ",\n";
    }
}
void JsonSerializer::try_remove_trailing_comma_object(std::string& str){
    
    if(config.whitespace == serial_ws::minimized){
        if(str.back() == ',')
            str.pop_back();
    }
    else if (config.whitespace == serial_ws::oneline){
        if(str.size() > 2)
            str = str.substr(0, str.length()-2);
    }
    else if (config.whitespace == serial_ws::new_lines){
        if(str.size() > 2)
            str = str.substr(0, str.length()-2);
    }
}


void JsonSerializer::add_trailing_comma_to_array(std::string& str){
    
    if(config.whitespace == serial_ws::minimized){
        str += ",";
    }
    else if (config.whitespace == serial_ws::oneline){
        str += ", ";
    }
    else if (config.whitespace == serial_ws::new_lines){
        str += ",\n";
    }
}
void JsonSerializer::try_remove_trailing_comma_array(std::string& str){
    
    if(config.whitespace == serial_ws::minimized){
        if(str.back() == ',')
            str.pop_back();
    }
    else if (config.whitespace == serial_ws::oneline){
        if(str.size() > 2)
            str = str.substr(0, str.length()-2);
    }
    else if (config.whitespace == serial_ws::new_lines){
        if(str.size() > 2)
            str = str.substr(0, str.length()-2);
    }
}


std::string JsonSerializer::serialize(JsonVar& json_root){

    return build_string(json_root);
}

std::string JsonSerializer::build_string(JsonVar json_var){

    if(is_literal(json_var.type)){

        switch (json_var.type)
        {
        case json_type::null :
            return "null";
            break;
        case json_type::bool_true:
            return "true";
            break;
        case json_type::bool_false:
            return "false";
            break;
        case json_type::number_float:
            return float_to_json_representation(json_var.get_float(), config.float_format);
            break;
        case json_type::number_int:
            return std::to_string( json_var.get_int() );
            break;
        case json_type::string:
            return string_to_json_representation(json_var.get_string());
            break;
        default:
            throw std::runtime_error("Unable to identify build string type.");
            break;
        }

    }


    if(json_var.type == json_type::array){
        std::string array_str = "[";
        json_array_variants arr = json_var.get_array();

        try_add_new_line(array_str);
        
        ++recursive_depth;
        for(JsonVar var : arr){

            try_add_indent(array_str);
        
            array_str += build_string(var);

            add_trailing_comma_to_array(array_str);
        }
        --recursive_depth;
        
        try_remove_trailing_comma_array(array_str);
        
        try_add_new_line(array_str);
        try_add_indent(array_str);
        
        array_str += "]";
        return array_str;

    }
    else if(json_var.type == json_type::object){

        json_object_variants obj = json_var.get_object();
        std::string obj_string = "{";

        try_add_new_line(obj_string);

        ++recursive_depth;
        for (json_kv_variant kv : obj){
            try_add_indent(obj_string);

            obj_string += string_to_json_representation(kv.first);
            
            if(config.whitespace == serial_ws::minimized)
                obj_string += ":";
            else 
                obj_string += ": ";
            
            obj_string += build_string(kv.second);

            add_trailing_comma_to_object(obj_string);

        }
        --recursive_depth;

        try_remove_trailing_comma_object(obj_string);
        
        try_add_new_line(obj_string);

        try_add_indent(obj_string);
        
        obj_string += "}";


        return obj_string;

    }
    
    throw std::runtime_error("Trying to build string of unknown type.");

}


std::string JsonSerializer::serialize(JsonWrapper root_wrapper, json_store& store){
    result_string = "";

    build_string(root_wrapper, store);

    return result_string;
}



std::string JsonSerializer::float_to_json_representation(json_float float_, FloatFormat float_format){

    if(float_format.precision == 0)
        throw std::runtime_error("Tried serializing json float with zero precision.");

    std::string str_return = "";

    std::ostringstream ss;

    switch (float_format.rep){

        case float_representation::fixed:
        case float_representation::fixed_trimmed:
            ss << std::fixed << std::setprecision(float_format.precision) << float_;
            break;

        case float_representation::scientific:
            ss << std::scientific << std::setprecision(float_format.precision) << float_;
            break;

    }

    // Done, unless trimming is set
    str_return = ss.str();


    // Trim Fixed represnetation sting below

    // we only trim fixed. Scientific string should always displaying precision!
    if(float_format.rep != float_representation::fixed_trimmed)
        return str_return;

    // at least one digit is necessary for valid json. Zero is check above.
    if(float_format.precision == 1)
        return str_return;

    // Count trailing zeros
    size_t trailing_zero_count = 0;
    size_t str_last_index = str_return.length() - 1;
    while( str_return[str_last_index - trailing_zero_count] == '0')
        ++trailing_zero_count;
    
    // if only trailing zeros, keep one (required for valid json!)
    if(str_return[str_last_index - trailing_zero_count] == '.')
        --trailing_zero_count;

    return str_return.substr(0, str_return.size()-trailing_zero_count);
}

std::string JsonSerializer::string_to_json_representation(std::string cpp_string){

    std::string json_representation = "";

    json_representation += "\"";

    for(const char ch : cpp_string){
        
        if(ch == QUOTATION_MARK){
            json_representation += "\\";
            json_representation += "\"";
        }
        else if (ch == SOLLIDUS){
            // I print the regular sollidus as-is. Parsing DOES handle escaped sollidus!
            json_representation += "/";
        }
        else if(ch == SOLLIDUS_REVERSE){
            json_representation += "\\";
            json_representation += "\\";
        }
        else if(ch == '\u0008'){
            json_representation += "\\b";
        }
        else if(ch == '\u0009'){
            json_representation += "\\t";
        }
        else if(ch == '\u000A'){
            json_representation += "\\n";
        }
        else if(ch == '\u000C'){
            json_representation += "\\f";
        }
        else if(ch == '\u000D'){
            json_representation += "\\r";
        }
        else if( (ch >= '\u0000' && ch < '\u0020') ){
            json_representation += "\\u00XX";
            // TODO: CONVERT ch to hex-string and append
            json_representation += std::to_string(ch);
        }
        else {
            json_representation += ch;
        }

    }

    json_representation += "\"";

    return json_representation;
}

void JsonSerializer::build_string(JsonWrapper value, json_store& store){
    
    if(is_literal(value.type)){

        switch (value.type)
        {
        case JSON_TYPE::NULL_:
            result_string.append("null");
            break;
        case JSON_TYPE::TRUE:
            result_string.append("true");
            break;
        case JSON_TYPE::FALSE:
            result_string.append("false");
            break;
        case JSON_TYPE::FLOAT:
            {
                json_float float_ = store.get_float(value.store_id);
                std::string float_str = float_to_json_representation(float_, config.float_format);

                result_string.append( float_str );
            }
            break;
        case JSON_TYPE::INTEGER:
            result_string.append( std::to_string(store.get_integer(value.store_id) ) );
            break;

        case JSON_TYPE::STRING:
            // 1) Grab value from store
            // 2) convert to json representation
            // 3) append to stringify-string
            result_string.append(
                string_to_json_representation(
                    store.get_string(value.store_id)
                )
            );
            break;
        default:
            break;
        }

        return;
    }

    // Print array
    if(value.type == JSON_TYPE::ARRAY){
        result_string.append("[");

        json_array_wrap array =  store.get_array(value.store_id);

        for(JsonWrapper array_entry : array){
            build_string(array_entry, store);
            result_string.append(", ");
        }

        // no comma after last entry
        if(array.size() > 0)
            result_string.erase(result_string.size()-2);

        result_string.append("]");
    }
    else if(value.type == JSON_TYPE::KV){

        json_kv_wrap& kv = store.get_kv(value.store_id);

        result_string.append("\"");
        result_string.append(kv.first);
        result_string.append("\": ");

        build_string(kv.second, store);
    }
    else if(value.type == JSON_TYPE::OBJECT){
        result_string.append("{");

        json_object_wrap object =  store.get_object(value.store_id);

        for(JsonWrapper kv : object){
            
            build_string(kv, store);

            result_string.append(", ");
        }

        // no comma after last entry
        if(object.size() > 0)
            result_string.erase(result_string.size()-2);

        result_string.append("}");
    }
}

