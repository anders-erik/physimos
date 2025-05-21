#pragma once

#include <vector>
#include <string>
#include <iomanip>

#include "physon_types.hh"

#include "json_variant.hh"
#include "json_token.hh"


struct JsonParser {

    JsonParser(std::string& _json_source, Tokens& _tokens) 
        :   json_source {_json_source},
            tokens {_tokens} {};

    JsonVar parse();
    
private:

    std::string& json_source;
    Tokens& tokens;

    JsonVar parse_array();
    JsonVar parse_object();

    json_string parse_string_literal(std::string string_literal);
    JsonVar parse_integer_str(std::string number_str);
    JsonVar parse_float_str(std::string number_str);

    JsonVar parse_value(Token& token);

    void throw_error(std::string error_msg);
};

json_string JsonParser::parse_string_literal(std::string string_literal){

    // Remove quotes
    std::string unquoted_str = string_literal.substr(1, string_literal.size()-2);

    // Literal value to return
    json_string new_string = "";

    enum class str_parse_state {
        normal,
        escape_check,
        escape_skip,
        u1,
        u2,
        u3,
        u4,
    } state = str_parse_state::normal;

    std::string unicode_chars = "1234";


    for(char& ch : string_literal){

        switch (state)
        {

        case str_parse_state::normal :
            if(ch == SOLLIDUS_REVERSE){
                state = str_parse_state::escape_check;
            }
            else {
                new_string += ch;
            }
            break;


        case str_parse_state::escape_check :

            switch (ch)
            {

            case QUOTATION_MARK:
                new_string += QUOTATION_MARK;
                break;
            case SOLLIDUS:
                new_string += SOLLIDUS;
                break;
            case SOLLIDUS_REVERSE:
                new_string += SOLLIDUS_REVERSE;
                break;
            
            case 'b':
                new_string += '\u0008';
                break;
            case 'f':
                new_string += '\u000C';
                break;
            case 'n':
                new_string += '\u000A';
                break;
            case 'r':
                new_string += '\u000D';
                break;
            case 't':
                new_string += '\u0009';
                break;

            case 'u':
                state = str_parse_state::u1;
                unicode_chars = "1234"; // reset
                break;
            
            default:
                break;
            }

            break;

        
        // Parse unicode : '\uXXXX'
        // Currently only supports ASCII
        case str_parse_state::u1 :
            unicode_chars[0] = ch;
            state = str_parse_state::u2;
            break;
        case str_parse_state::u2 :
            unicode_chars[1] = ch;
            state = str_parse_state::u3;
            break;
        case str_parse_state::u3 :
            unicode_chars[2] = ch;
            state = str_parse_state::u4;
            break;
        case str_parse_state::u4 :
            unicode_chars[3] = ch;
            {
                unsigned int unicode_value_decimal;
                std::stringstream _stringstream;
                _stringstream << std::hex << unicode_chars;
                _stringstream >> unicode_value_decimal;

                // log(unicode_digits);
                // log(unicode_value_decimal);

                // ASCII
                if(unicode_value_decimal < 0x7F){
                    new_string += static_cast<char>(unicode_value_decimal);
                }
                else {
                    throw_error("ERROR: non-ASCII unicode values in string are not yet supported.");
                }
            }

            state = str_parse_state::normal;
            break; 


        default:
            break;
        }
        // End state : str_parse_state


    }


    return new_string;
};

JsonVar JsonParser::parse_integer_str(std::string number_str){

    json_int parsed_int;

    try {
        parsed_int = std::stol(number_str);
    }
    catch(const std::exception& e)
    {
        throw_error("Failed to parse integer string.");

    }
    
    return JsonVar( parsed_int );
}

JsonVar JsonParser::parse_float_str(std::string number_str){

    json_float parsed_float;

    try {
        parsed_float = std::stod(number_str);
    }
    catch(const std::exception& e)
    {
        throw_error("Failed to parse float string.");
    }
    
    return JsonVar( parsed_float );
}


JsonVar JsonParser::parse_array(){

    JsonVar arr = json_array_variants();

    // Non-progressing close-check
    // {
    if(!tokens.next_is_in_bounds())
            throw_error("End of tokens inside array. ");
    size_t next_index = tokens.get_index() + 1;
    Token close_bracket_token = tokens[next_index];
    if(close_bracket_token.type == token_t::array_close){
        tokens.next_w_bounds_check(); // actual increment
        return arr;
    }
    // }

    while(true){

        Token value_token = tokens.next_w_bounds_check();

        if(Token::is_new_value(value_token))
            arr.push_to_array(parse_value(value_token));
        else
            throw_error("Value expected in array.");
        
        Token end_of_value_token = tokens.next_w_bounds_check();
        if(end_of_value_token.type == token_t::comma)
            continue;
        else if (end_of_value_token.type == token_t::array_close)
            return arr;
        else
            throw_error("Comma or closing-bracket expected after value in array.");

    }
    
}


JsonVar JsonParser::parse_object(){

    JsonVar object = json_object_variants();


    // Try to close right away without progressing index
    {
    if(!tokens.next_is_in_bounds())
            throw_error("End of tokens inside object. ");
    size_t next_index = tokens.get_index() + 1;
    Token close_brace_token = tokens[next_index];
    if(close_brace_token.type == token_t::object_close){
        tokens.next_w_bounds_check(); // actual increment
        return object;
    }
    }

    while(true){
        
        // KV check
        Token key_token = tokens.next_w_bounds_check();
        Token colon_token = tokens.next_w_bounds_check();
        Token value_token = tokens.next_w_bounds_check();
        if( !Token::is_string(key_token) || \
            !Token::is_colon(colon_token) || \
            !Token::is_new_value(value_token) )
            throw_error("Invalid kv token sequence in object.");

        // KV creation
        std::string raw_json_str = json_source.substr(key_token.str_start_i, key_token.str_length);
        json_string key = parse_string_literal(raw_json_str);

        json_kv_variant kv (key, parse_value(value_token));
        object.push_to_object(kv);
        
        // End of value checks
        Token end_of_value_token = tokens.next();
        if(end_of_value_token.type == token_t::comma)
            continue;
        else if (end_of_value_token.type == token_t::object_close)
            return object;
        else
            throw_error("Comma or closing-brace expected after value in object.");

    }
    
}

JsonVar JsonParser::parse_value(Token& token){

    JsonVar json_var;

    switch (token.type)
    {
    
    case token_t::whitespace :
        // if( tokens.next_is_in_bounds())
        //     return parse_literal(tokens.next());
        throw_error("No whitespace expected in parser.");
        // json_var = parse_value(tokens.next());
        break;

    case token_t::null_ :
        json_var = JsonVar(nullptr);
        break;

    case token_t::true_ :
        json_var = JsonVar(true);
        break;

    case token_t::false_ :
        json_var = JsonVar(false);
        break;

    case token_t::int_ :
        json_var = parse_integer_str(json_source.substr(token.str_start_i, token.str_length));
        break;

    case token_t::float_ :
        json_var = parse_float_str(json_source.substr(token.str_start_i, token.str_length));
        break;

    case token_t::string_ :
        json_var = JsonVar(parse_string_literal(json_source.substr(token.str_start_i, token.str_length)));
        break;

    case token_t::array_open :
        json_var = parse_array();
        break;

    case token_t::object_open :
        json_var = parse_object();
        break;

    default:
        throw_error("Token type not matched. ");
        break;
    }

    return json_var;
}



JsonVar JsonParser::parse(){
    
    tokens.reset_index();
    if(!tokens.current_is_in_bounds())
        throw_error("No tokens found.");
    
    if(tokens.get_count() == 1 && Token::is_ws(tokens.current()))
        throw_error("Tried to parse a whitespace-only json source string. ");

    tokens.remove_ws();
    
    Token first_token = tokens.current();

    return parse_value(first_token);

}


void JsonParser::throw_error(std::string error_msg){
    // token_type_to_string
    std::string full_error_str = "Parser: " + error_msg + tokens.get_state_string();

    throw std::runtime_error(full_error_str);
}