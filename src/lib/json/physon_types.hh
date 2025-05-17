#pragma once

#include <vector>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>

#include <variant>
#include <memory>


void json_error_get_variant_value(std::string error_msg){
    throw std::runtime_error("Error trying to get value from variant. " + error_msg);
}

/** Original : store type */
enum class JSON_TYPE {

    NULL_ = 0,
    TRUE,
    FALSE,

    STRING,

    NUMBER,
    FLOAT,
    INTEGER,
    
    ARRAY,
    OBJECT,
    KV,

    NONE, /** When no type is valid */
};



typedef std::string     json_string;
typedef bool            json_bool;
typedef std::nullptr_t  json_null;
typedef double          json_float;
typedef long long int   json_int;








enum class token_type {

    STRING =0,
    NUMBER,
    TRUE,
    FALSE,
    NULL_,

    LEFT_SQUARE,
    RIGHT_SQUARE,
    LEFT_CURLY,
    RIGHT_CURLY,
    COLON,
    COMMA,
};

struct Token {
    // int id;
    token_type type;
    int str_start_i;
    int str_length;

    Token(token_type type, int str_start_i, int str_length) 
        : type(type), str_start_i(str_start_i), str_length(str_length) {}
};

enum class JSON_PARSE_STATE {

    ROOT_BEFORE_VALUE,      /** Looking for the root value */
    ROOT_END_OF_VALUE,      /** Reached end of root value */
    DONE,                   /** Parsing done. Return primary parse method. */

    VALUE_AT_NEW_VALUE_CHAR,
    VALUE_PARSE_LITERAL,
    VALUE_END_OF_VALUE,

    ARRAY_ENTER,
    ARRAY_ENTERED,
    ARRAY_CLOSE,

    OBJECT_ENTER,
    OBJECT_PARSE_KEY_COMMA,
    OBJECT_CLOSE,

    NEW_KV,

    ERROR,
};




