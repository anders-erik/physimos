#pragma once

#include <vector>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>

#include <variant>
#include <memory>



#define log(x) std::cout << x << std::endl;



#define QUOTATION_MARK      '\u0022'
#define SOLLIDUS            '\u002F'
#define SOLLIDUS_BACKWARDS  '\u005C'

#define SPACE               '\u0020'
#define TAB                 '\u0009'
#define NEW_LINE            '\u000A'
#define CARRIAGE_RETURN     '\u000D'




typedef std::string     json_string;
typedef bool            json_bool;
typedef std::nullptr_t  json_null;
typedef double          json_float;
typedef long long int   json_int;




/*

    STORE

*/

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
bool is_literal(JSON_TYPE type){
    return  type == JSON_TYPE::NULL_    ||
            type == JSON_TYPE::TRUE     ||
            type == JSON_TYPE::FALSE    ||
            type == JSON_TYPE::NUMBER   ||
            type == JSON_TYPE::FLOAT    ||
            type == JSON_TYPE::INTEGER  ||
            type == JSON_TYPE::STRING;
}
bool is_container(JSON_TYPE type){
    return  type == JSON_TYPE::ARRAY    ||
            type == JSON_TYPE::OBJECT;
}




/*
    PARSER
*/

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




