#pragma once

#include <string>
#include <stdexcept>


enum class token_t {

    string_ =0,
    int_,
    uint_,
    float_,
    true_,
    false_,
    null_,

    array_open,
    array_close,
    object_open,
    object_close,
    kv_separator,
    comma,
    colon,

    whitespace,
};

struct Token {
    token_t type;
    /** First char of token in source string.  */
    int str_start_i;
    /** Length of token in source string. */
    int str_length;

    Token(token_t type) 
        : type(type) {};
    Token(token_t type, int str_start_i, int str_length) 
        : type(type), str_start_i(str_start_i), str_length(str_length) {};

};



std::string token_type_to_string(token_t type){

    switch (type)
    {
    
    case token_t::string_:
        return "string";
        break;
    case token_t::int_:
        return "int";
        break;
    case token_t::uint_:
        return "uint";
        break;
    case token_t::float_:
        return "float";
        break;
    case token_t::true_:
        return "true";
        break;
    case token_t::false_:
        return "false";
        break;
    case token_t::null_:
        return "null";
        break;

    
    case token_t::array_open:
        return "array_open";
        break;
    case token_t::array_close:
        return "array_close";
        break;
    case token_t::object_open:
        return "object_open";
        break;
    case token_t::object_close:
        return "object_close";
        break;
    case token_t::kv_separator:
        return "kv_separator";
        break;
    case token_t::comma:
        return "comma";
        break;
    case token_t::colon:
        return "colon";
        break;

    case token_t::whitespace:
        return "ws";
        break;
    }

    throw std::runtime_error("Failed to match a json token type.");
}
