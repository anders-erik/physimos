#pragma once

#include <vector>
#include <string>
#include <iomanip>

#include "json_types.hh"

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

    /** Unquoted json source string as input.  */
    j_string parse_string_literal(std::string string_literal);
    char unicode_sequence_to_ASCII(std::string unicode_sequence);
    char json_escape_char_to_value(char escape_char);
    JsonVar parse_integer_str(std::string number_str);
    JsonVar parse_float_str(std::string number_str);

    JsonVar parse_value(Token& token);

    void throw_error(std::string error_msg);
};

char JsonParser::unicode_sequence_to_ASCII(std::string unicode_sequence){

    unsigned int unicode_value_decimal;
    std::stringstream _stringstream;
    _stringstream << std::hex << unicode_sequence;
    _stringstream >> unicode_value_decimal;
    // log(unicode_value_decimal);

    // Enforce ASCII
    if(unicode_value_decimal > 0x7F)
        throw_error("ERROR: non-ASCII unicode values in string are not yet supported.");

    return static_cast<char>(unicode_value_decimal);
}
char JsonParser::json_escape_char_to_value(char escape_char){

    switch (escape_char)
    {

    case QUOTATION_MARK:
        return QUOTATION_MARK;
        break;
    case SOLLIDUS:
        return SOLLIDUS;
        break;
    case SOLLIDUS_REVERSE:
        return SOLLIDUS_REVERSE;
        break;
    case 'b':
        return '\u0008';
        break;
    case 'f':
        return '\u000C';
        break;
    case 'n':
        return '\u000A';
        break;
    case 'r':
        return '\u000D';
        break;
    case 't':
        return '\u0009';
        break;
    case 'u':
        throw_error("String escape char: 'u' does not correspond to char.");
        break;
    default:
        throw_error("String escape char: invalid char.");
        break;

    }

    return 0;
}

j_string JsonParser::parse_string_literal(std::string string_literal){

    // Literal value to return
    j_string new_string = "";

    // Parse loop
    for(size_t i = 0; i < string_literal.length(); i++){

        /** Current char in the json-source string literal.  */
        char ch = string_literal[i];

        // Relies on lexer to have caught invalid quotes and control chars.
        if(ch != SOLLIDUS_REVERSE){
            new_string += ch;
            continue;
        }

        // Skip escape char
        ch = string_literal[++i];

        // Single-char escape values
        if(ch != 'u'){
            new_string += json_escape_char_to_value(ch);
            continue;
        }
        
        // Unicode sequence - only ascii
        std::string unicode_chars = string_literal.substr(i+1, 4);
        new_string += unicode_sequence_to_ASCII(unicode_chars);
        i += 4;
    }

    return new_string;
};

JsonVar JsonParser::parse_integer_str(std::string number_str){

    j_int parsed_int;

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

    j_float parsed_float;

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

    JsonVar array = json_array_variants();

    // Try to close right away
    if(Token::is_array_close(tokens.next_w_bounds_check()))
            return array;

    // Reset index as next token was not 'array close'
    tokens.decrement_index();


    // Value-parsing
    while(true){

        Token value_token = tokens.next_w_bounds_check();

        if(Token::is_new_value(value_token))
            array.push_to_array(parse_value(value_token));
        else
            throw_error("Value expected in array.");
        
        // End of Value check
        Token end_of_value_token = tokens.next_w_bounds_check();
        if(Token::is_comma(end_of_value_token))
            continue;
        else if (Token::is_array_close(end_of_value_token))
            return array;
        else
            throw_error("Comma or closing-bracket expected after value in array.");

    }
    
}


JsonVar JsonParser::parse_object(){

    JsonVar object = json_object_variants();

    // Try to close right away
    if(Token::is_object_close(tokens.next_w_bounds_check()))
            return object;

    // Reset index as next token was not 'object close
    tokens.decrement_index();


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
        j_string key = parse_string_literal(raw_json_str);

        json_kv_variant kv (key, parse_value(value_token));
        object.push_to_object(kv);
        
        // End of value checks
        Token end_of_value_token = tokens.next_w_bounds_check();
        if(Token::is_comma(end_of_value_token))
            continue;
        else if (Token::is_object_close(end_of_value_token))
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