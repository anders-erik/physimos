
#include "io/json/json.hh"

#include "io/json/json_serialize.hh"


void JsonSerializer::add_spaces(Str& str, int count){
    for(int i = 0; i < count; i++)
        str += " ";
}

void JsonSerializer::try_add_new_line(Str& str){
    if(config.whitespace == serial_ws::new_lines)
        str += "\n";
}
void JsonSerializer::try_add_indent(Str& str){
    if(config.whitespace == serial_ws::new_lines)
        add_spaces(str, recursive_depth*config.indent_space_count);
}
void JsonSerializer::add_trailing_comma_to_object(Str& str){
    
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
void JsonSerializer::try_remove_trailing_comma_object(Str& str){
    
    if(config.whitespace == serial_ws::minimized){
        if(str[str.size()-1] == ',')
            str.pop_back();
    }
    else if (config.whitespace == serial_ws::oneline){
        if(str.size() > 2)
            str = str.substr(0, str.size()-2);
    }
    else if (config.whitespace == serial_ws::new_lines){
        if(str.size() > 2)
            str = str.substr(0, str.size()-2);
    }
}


void JsonSerializer::add_trailing_comma_to_array(Str& str){
    
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
void JsonSerializer::try_remove_trailing_comma_array(Str& str){
    
    if(config.whitespace == serial_ws::minimized){
        if(str[str.size()-1] == ',')
            str.pop_back();
    }
    else if (config.whitespace == serial_ws::oneline){
        if(str.size() > 2)
            str = str.substr(0, str.size()-2);
    }
    else if (config.whitespace == serial_ws::new_lines){
        if(str.size() > 2)
            str = str.substr(0, str.size()-2);
    }
}


Str JsonSerializer::serialize(const Json& json_root){

    return build_string(json_root);
}

Str JsonSerializer::build_string(Json json_var){

    if(is_literal(json_var.type)){

        switch (json_var.type)
        {
        case json_type::null :
            return "null";
            break;
        case json_type::boolean:
            if(json_var.get_bool())
                return "true";
            else 
                return "false";
            break;
        case json_type::number_float:
            return float_to_json_representation(json_var.get_float(), config.float_format);
            break;
        case json_type::number_int:
            return Str::SI( json_var.get_int() );
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
        Str array_str = "[";
        j_array arr = json_var.get_array();

        try_add_new_line(array_str);
        
        ++recursive_depth;
        for(Json var : arr){

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

        j_object obj = json_var.get_object();
        Str obj_string = "{";

        try_add_new_line(obj_string);

        ++recursive_depth;
        for (auto& kv : obj){
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




Str JsonSerializer::float_to_json_representation(j_float float_, FloatFormat float_format){

    if(float_format.precision == 0)
        throw std::runtime_error("Tried serializing json float with zero precision.");

    Str str_return = "";

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
    str_return = ss.str().c_str();


    // Trim Fixed represnetation sting below

    // we only trim fixed. Scientific string should always displaying precision!
    if(float_format.rep != float_representation::fixed_trimmed)
        return str_return;

    // at least one digit is necessary for valid json. Zero is check above.
    if(float_format.precision == 1)
        return str_return;

    // Count trailing zeros
    size_t trailing_zero_count = 0;
    size_t str_last_index = str_return.size() - 1;
    while( str_return[str_last_index - trailing_zero_count] == '0')
        ++trailing_zero_count;
    
    // if only trailing zeros, keep one (required for valid json!)
    if(str_return[str_last_index - trailing_zero_count] == '.')
        --trailing_zero_count;

    return str_return.substr(0, str_return.size()-trailing_zero_count);
}

Str JsonSerializer::string_to_json_representation(Str cpp_string){

    Str json_representation = "";

    json_representation += "\"";

    // for(const char ch : cpp_string){
    for(auto i=0; i<cpp_string.size(); i++)
    {
        char ch = cpp_string[i];
   
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
            json_representation += Str::CH(ch);
        }
        else {
            json_representation += Str::CH(ch);
        }

    }

    json_representation += "\"";

    return json_representation;
}
