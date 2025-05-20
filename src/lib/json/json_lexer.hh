#pragma once
#include <vector>
#include <iomanip>

#include "physon_types.hh"

#include "json_token.hh"


struct JsonLexer {

    JsonLexer() {};
    // JsonLexer(const std::string& _json_source) : json_source{_json_source} {};

    static void print_tokens(Tokens& tokens);

    Tokens lex(std::string& _json_source);


private:

    std::string json_source;
    size_t index = 0;
    Tokens tokens;
    std::vector<Token> _tokens;

    void reset_lexer();


    // STATIC
    static bool is_whitespace(char ch);
    static bool is_digit(char ch);
    static bool is_non_zero_digit(char ch);
    /**Only checks for existence of '.' */
    static bool is_fractional_number_string(std::string number_string);


    // QUERY
    /** Get current char without affecting index. */
    char current_char();


    // CONSUME
    void increment_index();
    void increment_index(int count);
    /** Returns current char at time of call. Increment index by one. */
    char consume_char();
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
    void tokenize_literal_string();
    void tokenize_literal_number();
    /** Increment index and create ws token if ws present. */
    void tokenize_ws();

    // ERROR HANDLING
    void throw_error(std::string error_msg);

};


void JsonLexer::reset_lexer(){
    index = 0;
    _tokens.clear();
    tokens.clear();
    json_source.clear();
}

char JsonLexer::current_char(){
    return json_source[index];
}


void JsonLexer::increment_index(){
    ++index;
}
void JsonLexer::increment_index(int count){
    index += count;
}

char JsonLexer::consume_char(){
    return json_source[index++];
}

size_t JsonLexer::consume_ws(){
    size_t ws_chars_consumed = 0;

    while(is_whitespace(json_source[index])){
        consume_char();
        ++ws_chars_consumed;
    }

    return ws_chars_consumed;
}

size_t JsonLexer::consume_string_literal(){
    size_t start_i = index;
    // Skip quotation mark, but no gobbling in string literal
    index++;

    json_string new_string = "";

    bool end_of_string = false;


    while( !end_of_string ){

        // Current char
        char ch = json_source[index];
        
        if( ch >= '\u0000' && ch < '\u0020'){
            throw_error("Error: unescaped control character in string. Found at index " + std::to_string(json_source[index]) );
        }
        else if(ch == SOLLIDUS_REVERSE){

            // skip backwards sollidus
            index++;
            ch = json_source[index];

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
                // Parse unicode : '\uXXXX'
                // Currently only supports ASCII
                {
                    std::string unicode_digits = json_source.substr(index+1, 4);

                    unsigned int unicode_value_decimal;
                    std::stringstream _stringstream;
                    _stringstream << std::hex << unicode_digits;
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
                // move to last unicode digit
                index += 4;
                break;
            
            default:
                break;
            }

        }
        else if(ch == QUOTATION_MARK){
            end_of_string = true;
        }
        else {
            new_string += json_source[index];
        }
    

        // Next char
        index++;

        if(index >= json_source.size())
            throw_error("Error: Unclosed string literal. Expected closing quotation mark before end of content string.");
    }

    // Move past closing quotation mark
    increment_index();

    // Add string to store
    // JsonWrapper new_value;
    // new_value.type = JSON_TYPE::STRING;
    // new_value.store_id = store.add_string(new_string);


    return index - start_i;
    // return new_string;
}


size_t JsonLexer::consume_null_literal(){
    bool is_null_literal =  json_source[index+0] == 'n' &&
                            json_source[index+1] == 'u' &&
                            json_source[index+2] == 'l' &&
                            json_source[index+3] == 'l';
    
    if(!is_null_literal)
        throw_error("Expected null literal. Invalid chars.");
    
    increment_index(4);
    
    return 4;
}
size_t JsonLexer::consume_true_literal(){
    bool consume_true_literal =  json_source[index+0] == 't' &&
                            json_source[index+1] == 'r' &&
                            json_source[index+2] == 'u' &&
                            json_source[index+3] == 'e';

    if(!consume_true_literal)
        throw_error("Expected true literal. Invalid chars.");
    
    increment_index(4);
    
    return 4;
}
size_t JsonLexer::consume_false_literal(){
    bool consume_false_literal = json_source[index+0] == 'f' &&
                            json_source[index+1] == 'a' &&
                            json_source[index+2] == 'l' &&
                            json_source[index+3] == 's' &&
                            json_source[index+4] == 'e';

    if(!consume_false_literal)
        throw_error("Expected false literal. Invalid chars.");
    
    increment_index(5);
    
    return 5;
}




bool JsonLexer::is_whitespace(char ch){
    return ch == SPACE || ch == TAB || ch == NEW_LINE || ch == CARRIAGE_RETURN;
}
bool JsonLexer::is_digit(char c){
    return (c >= '0' && c <= '9') ? true : false;
};
bool JsonLexer::is_non_zero_digit(char c){
    return (c >= '1' && c <= '9') ? true : false;
};

size_t JsonLexer::consume_digits(){
    size_t digit_count = 0;

    while(is_digit(current_char())){
        increment_index();
        digit_count++;
    }

    return digit_count;
}


size_t JsonLexer::consume_number_literal(){
    enum class number_state {
        negative_check,
        integral_digits,
        fraction,
        exponent_detect,
        exponent_consume,
        exponent_digits,
        end
    } number_state = number_state::negative_check;

    size_t start_index = index;

    // bool is_fractional = false;

    std::string string_to_parse = "";


    while (number_state != number_state::end)
    {

        switch (number_state){
        
        case number_state::negative_check:
            if(current_char() == '-'){
                string_to_parse += "-";
                index++;
            }
            number_state = number_state::integral_digits;
            break;


        case number_state::integral_digits:
            if (current_char() == '0'){
                string_to_parse += "0";
                index++;
            }
            else if (is_non_zero_digit(current_char())) {
                consume_digits();
            }
            else {
                throw_error("First digit in number trailing leading '-' not valid.");
            }
            number_state = number_state::fraction;
            break;


        case number_state::fraction:
            if(current_char() == '.'){
                // is_fractional = true;
                string_to_parse += consume_char();

                if(! is_digit(current_char()))
                    throw_error("Fraction delimiter must be followed by digit.");

                consume_digits();
            }
            number_state = number_state::exponent_detect;
            break;


        case number_state::exponent_detect:
            if(current_char() == 'e' || current_char() == 'E'){
                string_to_parse += consume_char();
                number_state = number_state::exponent_consume;
            }
            else {
                number_state = number_state::end;
            }
            break;
        

        case number_state::exponent_consume:
        case number_state::exponent_digits:
            {
                bool first_exponent_char_valid = is_digit(current_char()) || current_char() == '+' || current_char() == '-';
                if(! first_exponent_char_valid)
                    throw_error("Exponent 'e'/'E' not trailed by sign nor digit.");
            }
            
            if(current_char() == '+' || current_char() == '-')
                consume_char();
            
            number_state = number_state::exponent_digits;

            if(! is_digit(current_char()))
                throw_error("No exponent digits detected during number verification.");

            consume_digits();

            number_state = number_state::end;
            break;

        
        default:
            throw_error("Unknown number verification state. ");
            break;
        }

    }

    // Post number checks for better error messages

    // if a "-0" or '0' is detected, a number such as "09" will be treated as two separate numbers. But since that input most likely is an attempt at writing the number '9', this error message provides this information. 
    if(is_digit(current_char()))
        throw_error("Superfluous leading zero not allowed.");
    

    // return string_to_parse;
    return index - start_index;
}


bool JsonLexer::is_fractional_number_string(std::string number_string){

    for(char ch : number_string){
        if(ch == '.')
            return true;
    }

    return false;
}



void JsonLexer::tokenize_array_open(){
    tokens.emplace( { token_t::array_open, index++, 1 } );
}
void JsonLexer::tokenize_array_close(){
    tokens.emplace( { token_t::array_close, index++, 1 } );
}
void JsonLexer::tokenize_object_open(){
    tokens.emplace( { token_t::object_open, index++, 1 } );
}
void JsonLexer::tokenize_object_close(){
    tokens.emplace( { token_t::object_close, index++, 1 } );
}
void JsonLexer::tokenize_comma(){
    tokens.emplace( { token_t::comma, index++, 1 } );
}
void JsonLexer::tokenize_colon(){
    tokens.emplace( { token_t::colon, index++, 1 } );
}
void JsonLexer::tokenize_literal_null(){
    size_t start_i = index;
    tokens.emplace( { token_t::null_, start_i, consume_null_literal() } );
}
void JsonLexer::tokenize_literal_true(){
    size_t start_i = index;
    tokens.emplace( { token_t::true_, start_i, consume_true_literal() } );
}
void JsonLexer::tokenize_literal_false(){
    size_t start_i = index;
    tokens.emplace( { token_t::false_, start_i, consume_false_literal() } );
}
void JsonLexer::tokenize_literal_string(){
    size_t start_i = index;
    tokens.emplace( { token_t::string_, start_i, consume_string_literal() } );
}
void JsonLexer::tokenize_literal_number(){
    size_t start_index = index;
    size_t len = consume_number_literal();
    Token& new_token = tokens.emplace( { token_t::int_, start_index, len } );

    if(is_fractional_number_string(json_source.substr(start_index, len)))
        new_token.type = token_t::float_;
}
void JsonLexer::tokenize_ws(){
    size_t index_start = index;
    tokens.emplace( { token_t::whitespace, index_start, consume_ws() } );
}


void JsonLexer::throw_error(std::string error_msg){

    std::string state_string = "\n    Current char  = " + json_source.substr(index, 1) + "\n    Current index = " + std::to_string(index);

    std::string full_error_str = "Lexer: " + error_msg + state_string;

    throw std::runtime_error(full_error_str);

}


void JsonLexer::print_tokens(Tokens& tokens) {

    std::cout << "Tokens: " << std::endl << std::endl;

    std::cout << "   Type       |" << std::endl;
    std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
    
    
    for (const auto& token : tokens.get_vector()) {
        std::cout << std::setw(12) << token_type_to_string(token.type) << "  |" << std::endl;
    }

    std::cout << "____________________________________" << std::endl << std::endl;
}




Tokens JsonLexer::lex(std::string& _json_source){

    reset_lexer();

    json_source = _json_source;


    while(index < json_source.size()){
    
        // NEW VALUE CHECKS
        switch(current_char())
        {
        
        // Single char literals
        case '[':
            tokenize_array_open();
            break;
        case ']':
            tokenize_array_close();
            break;
        case '{':
            tokenize_object_open();
            break;
        case '}':
            tokenize_object_close();
            break;
        case ',':
            tokenize_comma();
            break;
        case ':':
            tokenize_colon();
            break;

        // Multi-char literals
        case 'n':
            tokenize_literal_null();
            break;
        case 't':
            tokenize_literal_true();
            break;
        case 'f':
            tokenize_literal_false();
            break;
        case '\"':
            tokenize_literal_string();
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '-':
            tokenize_literal_number();
            break;
        
        // Whitespace
        case SPACE:
        case TAB:
        case NEW_LINE:
        case CARRIAGE_RETURN:
            tokenize_ws();
            break;

        default:
            throw_error("Invalid char encountered in main lexing loop.");
            break;
        
        }
    
    }

    // return _tokens;
    return tokens;
}

