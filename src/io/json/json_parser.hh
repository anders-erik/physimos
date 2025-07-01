#pragma once

#include <vector>
#include <string>
#include <iomanip>

#include "json_types.hh"

#include "json_token.hh"


struct JsonParser {

    JsonParser(const Str& _json_source, Tokens& _tokens) 
        :   json_source {_json_source},
            tokens {_tokens} {};

    Json parse();
    
private:

    Str json_source;
    Tokens& tokens;

    Json parse_array();
    Json parse_object();

    /** Unquoted json source string as input.  */
    j_string parse_string_literal(Str string_literal);
    char unicode_sequence_to_ASCII(Str unicode_sequence);
    char json_escape_char_to_value(char escape_char);
    Json parse_integer_str(Str number_str);
    Json parse_float_str(Str number_str);

    Json parse_value(Token& token);

    void throw_error(Str error_msg);
};
