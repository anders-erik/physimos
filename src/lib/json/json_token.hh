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
    size_t str_start_i;
    /** Length of token in source string. */
    size_t str_length;

    Token(token_t type) 
        : type(type) {};
    Token(token_t type, size_t str_start_i, size_t str_length) 
        : type(type), str_start_i(str_start_i), str_length(str_length) {};

    static bool is_literal(token_t type) {
        return  type == token_t::string_    ||
                type == token_t::int_       ||
                type == token_t::uint_      ||
                type == token_t::float_     ||
                type == token_t::true_      ||
                type == token_t::false_     ||
                type == token_t::null_;
    }
    static bool is_literal(Token token) {
        return  is_literal(token.type);
    }
    static bool is_new_value(token_t type) {
        return  type == token_t::array_open     ||
                type == token_t::object_open    ||
                type == token_t::string_        ||
                type == token_t::int_           ||
                type == token_t::uint_          ||
                type == token_t::float_         ||
                type == token_t::true_          ||
                type == token_t::false_         ||
                type == token_t::null_;
    }
    static bool is_new_value(Token token) {
        return  is_new_value(token.type);
    }
};

class Tokens {
    std::vector<Token> vec;
    size_t current_index = 0;
public:

    void push(Token& token) {vec.push_back(token);};
    Token& emplace(Token&& token) {return vec.emplace_back(token);};
    size_t get_index() {return current_index;}
    void reset_index() {current_index = 0;}
    void clear() {reset_index(); vec.clear();};
    Token& operator[](size_t index) {return vec[current_index];}
    Token& current() {return vec[current_index];}
    Token& next() {return vec[++current_index];}
    void remove_ws() {
        std::vector<Token> vec_no_ws;
        for(Token& token : vec){
            if(token.type == token_t::whitespace)
                continue;
            vec_no_ws.push_back(token);
        }
        vec = vec_no_ws;
    }
    bool current_is_in_bounds() {return (current_index < vec.size()) ? true:false;};
    bool next_is_in_bounds() {return (current_index < vec.size()) ? true:false;};
    std::vector<Token>& get_vector() {return vec;};
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
