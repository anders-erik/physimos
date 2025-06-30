#pragma once

#include <vector>
#include <string>
#include <iomanip>

#include "json_types.hh"

#include "json_token.hh"


struct JsonParser {

    JsonParser(const std::string& _json_source, Tokens& _tokens) 
        :   json_source {_json_source},
            tokens {_tokens} {};

    JsonVar parse();
    
private:

    const std::string& json_source;
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
