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




struct JsonSerializer {

    JsonSerializer() {};

    /** returns full json structure as string. Creates a copy of wrapper and store store for now.  */
    std::string serialize(JsonWrapper root_wrapper, json_store& store);
    void build_string(JsonWrapper value, json_store& store);
    

    /** Supports different precision and scientific notation */
    static std::string float_to_json_representation(json_float float_);
    /** Converts a std::string to is JSON equivelence. e.g. <I "mean" it..> --> <"I \"mean\" it.."> */
    static std::string string_to_json_representation(std::string cpp_string);


private:

    std::string result_string = "";

};




std::string JsonSerializer::serialize(JsonWrapper root_wrapper, json_store& store){
    result_string = "";

    build_string(root_wrapper, store);

    return result_string;
}



std::string JsonSerializer::float_to_json_representation(json_float float_){

    int precision = 7;

    /** Examples : precision == 7 */
    enum class REPRESENTATION {
        FIXED,              // 0.0 -> 0.0000000
        FIXED_TRIMMED,      // 0.0 -> 0.0 : precision == 1
        SCIENTIFIC,         // 0.0 -> 0.0000000e+00
    } representation = REPRESENTATION::SCIENTIFIC;


    std::string str_return = "";

    std::ostringstream ss;

    switch (representation){

    case REPRESENTATION::FIXED:
    case REPRESENTATION::FIXED_TRIMMED:
        ss << std::fixed << std::setprecision(precision) << float_;
        break;

    case REPRESENTATION::SCIENTIFIC:
        ss << std::scientific << std::setprecision(precision) << float_;
        break;

    }

    str_return = ss.str();


    // TODO:  THIS WILL BE REMOVED WITH THE PRECISION SETTING
    if(representation == REPRESENTATION::FIXED_TRIMMED){

         // TRIM TRAILING ZEROS
        while( str_return[str_return.length()-2] == '0')
            str_return.pop_back();

    }

    return str_return;
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
        else if(ch == SOLLIDUS_BACKWARDS){
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
                std::string float_str = float_to_json_representation(float_);

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

