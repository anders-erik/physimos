#pragma once
#include <vector>
#include <iomanip>

#include "json_types.hh"

#include "json_token.hh"


struct JsonLexer {

    JsonLexer() {};
    // JsonLexer(const std::string& _json_source) : json_source{_json_source} {};

    static void print_tokens(Tokens& tokens);

    Tokens& lex(const std::string& _json_source);


    Tokens tokens;
private:

    std::string json_source;
    size_t index = 0;

    void reset_lexer();


    // STATIC
    static bool is_whitespace(char c);
    static bool is_digit(char c);
    static bool is_non_zero_digit(char c);
    static bool is_plus_minus(char c);
    static bool is_hex_digit(char c);
    /**Only checks for existence of '.' */
    static bool is_fractional_number_string(std::string number_string);
    bool is_unicode_quad(const std::string& four_unicode_digits);

    // QUERY
    /** Get current char without affecting index. */
    char current_char();


    // CONSUME
    void increment_index();
    void increment_index(int count);
    /** Returns current char at time of call. Increment index by one. */
    char consume_char();
    /** Increments first, then returns the current char. */
    char next_char();
    size_t consume_null_literal();
    size_t consume_true_literal();
    size_t consume_false_literal();
    /** Throws on invalid string format.  */
    size_t consume_string_literal();
    /** Consumes digits until non-digit encountered. */
    size_t consume_digits();
    /** Throws on invalid number format. */
    size_t consume_number_literal();
    size_t consume_ws();
    

    // TOKENIZE 
    void tokenize_array_open();
    void tokenize_array_close();
    void tokenize_object_open();
    void tokenize_object_close();
    void tokenize_comma();
    void tokenize_colon();

    void tokenize_literal_null();
    void tokenize_literal_true();
    void tokenize_literal_false();
    constexpr void tokenize_literal_string();
    void tokenize_literal_number();
    /** Increment index and create ws token if ws present. */
    void tokenize_ws();

    // ERROR HANDLING
    void throw_error(std::string error_msg);

};


