#pragma once

#include <string>
#include <stdexcept>

#include "lib/str.hh"

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

constexpr Str token_type_to_string(token_t type){

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

struct Token 
{
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
    static bool is_string(Token token) {
        return  token.type == token_t::string_ ? true : false;
    }
    static bool is_array_close(Token token) {
        return  token.type == token_t::array_close ? true : false;
    }
    static bool is_object_close(Token token) {
        return  token.type == token_t::object_close ? true : false;
    }
    static bool is_colon(Token token) {
        return  token.type == token_t::colon ? true : false;
    }
    static bool is_comma(Token token) {
        return  token.type == token_t::comma ? true : false;
    }
    static bool is_ws(Token token) {
        return  token.type == token_t::whitespace ? true : false;
    }
    
};

class Tokens 
{
    std::vector<Token> vec;
    size_t current_index = 0;
public:

    void push(Token& token) {vec.push_back(token);};
    Token& emplace(Token&& token) {return vec.emplace_back(token);};
    size_t get_index() {return current_index;}
    void decrement_index() {
        if(--current_index > vec.size())
            throw_bounds_error("when trying to decrement index.");
    }
    void reset_index() {current_index = 0;}
    size_t get_count() {return vec.size();}
    void clear() {reset_index(); vec.clear();};
    Token& operator[](size_t index) {return vec[index];}
    Token& current() {return vec[current_index];}
    Token& next() {return vec[++current_index];}
    Token& next_w_bounds_check() {
        if(++current_index >= vec.size())
            throw_bounds_error("when trying to grab next token.");
        return vec[current_index];
    }
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
    
    bool index_is_valid(size_t i){
        return (i < vec.size()) ? true : false;
    }
    Str get_state_string(){

        Str state_string;

        size_t print_count = 3;
        size_t index_offset_neg = 1;
        size_t start_i = current_index - index_offset_neg;
        size_t end_i = start_i + print_count;

        for(size_t i = start_i; i < end_i; i++){

            if(index_is_valid(i))
                state_string += (Str)"\nToken index " + Str::UI(i) + ": " + token_type_to_string(this->operator[](i).type);
            else
                state_string += "\nToken index out of range.";

        }
        return state_string;
    }
    void throw_bounds_error(Str when_msg){
        --current_index; // move back to in bounds
        Str state_string = get_state_string();
        Str error_msg = (Str)"Out of bounds error." + when_msg;
        throw std::runtime_error((error_msg + state_string).to_c_str());
    };
};

