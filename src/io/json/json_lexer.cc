
#include "io/json/json_lexer.hh"



void JsonLexer::reset_lexer(){
    index = 0;
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
char JsonLexer::next_char(){
    return json_source[++index];
}

size_t JsonLexer::consume_ws(){
    size_t ws_chars_consumed = 0;

    while(is_whitespace(json_source[index])){
        consume_char();
        ++ws_chars_consumed;
    }

    return ws_chars_consumed;
}

bool JsonLexer::is_unicode_quad(const Str& four_unicode_digits)
{
    if(four_unicode_digits.size() != 4)
        return false;

    for(auto i = 0; i < 4; i++)
    {
        if(!is_hex_digit(four_unicode_digits[i]))
            return false;
    }
    return true;
}

size_t JsonLexer::consume_string_literal(){
    
    size_t start_i = index;
    char ch;

    while( true ){

        // Current char
        ch = current_char();
        
        if( ch >= '\u0000' && ch < '\u0020')
        {
            throw_error((Str)"Error: unescaped control character in string. Found at index " + Str::CH(json_source[index]) );
        }
        else if(ch == SOLLIDUS_REVERSE){

            // skip reverse sollidus
            ch = next_char();

            switch (ch)
            {

                case QUOTATION_MARK:
                case SOLLIDUS:
                case SOLLIDUS_REVERSE:
                case 'b':
                case 'f':
                case 'n':
                case 'r':
                case 't':
                    break;

                case 'u':
                    if(!is_unicode_quad(json_source.substr(index+1, 4)))
                        throw_error("Non valid unicode escape characters.");
                    increment_index(4);
                    break;
                
                default:
                    throw_error("Invalid escape character.");
                    break;
     
            }

        }
        else if(ch == QUOTATION_MARK){
            break;
        }

        // Next char
        increment_index();

        if(index >= json_source.size())
            throw_error("Error: Unclosed string literal. Expected closing quotation mark before end of source string.");
    }

    // Move past closing quotation mark
    increment_index();

    // Minus 1 because the quotation mark is not part of string 
    return index - start_i - 1;
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




bool JsonLexer::is_whitespace(char c){
    return c == SPACE || c == TAB || c == NEW_LINE || c == CARRIAGE_RETURN;
}
bool JsonLexer::is_digit(char c){
    return (c >= '0' && c <= '9') ? true : false;
};
bool JsonLexer::is_non_zero_digit(char c){
    return (c >= '1' && c <= '9') ? true : false;
};
bool JsonLexer::is_plus_minus(char c){
    return (c == '+' || c == '-') ? true : false;
}
bool JsonLexer::is_hex_digit(char c){
    return (c >= '0' && c <= '9') || 
            (c >= 'A' && c <= 'F') || 
            (c >= 'a' && c <= 'f') ? true : false;
}

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


    while (number_state != number_state::end)
    {

        /** The 'current_char()' in number lexing loop.  */
        char ch = current_char();

        switch (number_state){
        
        case number_state::negative_check:
            if(ch == '-')
                increment_index();

            number_state = number_state::integral_digits;
            break;


        case number_state::integral_digits:
            if (ch == '0')
                increment_index();
            else if (is_non_zero_digit(ch))
                consume_digits();
            else
                throw_error("First digit in number trailing leading '-' not valid.");

            number_state = number_state::fraction;
            break;


        case number_state::fraction:
            if(ch == '.'){
                ch = next_char();

                if(! is_digit(ch))
                    throw_error("Fraction delimiter must be followed by digit.");

                consume_digits();
            }
            number_state = number_state::exponent_detect;
            break;


        case number_state::exponent_detect:
            if(ch == 'e' || ch == 'E'){
                increment_index();
                number_state = number_state::exponent_consume;
            }
            else {
                number_state = number_state::end;
            }
            break;
        

        case number_state::exponent_consume:
        case number_state::exponent_digits:
            
            if(! (is_digit(ch) || is_plus_minus(ch)))
                throw_error("Exponent 'e'/'E' not trailed by sign nor digit.");
            
            if(is_plus_minus(ch))
                ch = next_char();
            
            number_state = number_state::exponent_digits;

            if(! is_digit(ch))
                throw_error("No exponent digits detected during number verification.");

            consume_digits();

            number_state = number_state::end;
            break;

        
        default:
            throw_error("Unexpected number verification state.");
            break;

        }

    }

    // Beyond formal checks for better error messages

    // if a "-0" or '0' is detected, a number such as "09" will be treated as two separate numbers, per the formal grammar. But since that input most likely is an attempt at writing the number '9', this error message provides this information. 
    if(is_digit(current_char()))
        throw_error("Superfluous leading zero not allowed.");
    

    return index - start_index;
}


bool JsonLexer::is_fractional_number_string(Str number_string)
{
    for(auto i=0; i < number_string.size(); i++)
    {
        if(number_string[i] == '.')
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
constexpr void JsonLexer::tokenize_literal_string(){
    size_t start_i = ++index; // skip quotation
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


void JsonLexer::throw_error(Str error_msg){

    Str state_string = (Str)"\n    Current char  = " + json_source.substr(index, 1) + "\n    Current index = " + Str::UI(index);

    Str full_error_str = (Str)"Lexer: " + error_msg + state_string;

    throw std::runtime_error(full_error_str.to_c_str());

}


void JsonLexer::print_tokens(Tokens& tokens) {

    std::cout << "Tokens: " << std::endl << std::endl;

    std::cout << "   Type       |" << std::endl;
    std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
    
    
    for (const auto& token : tokens.get_vector())
    {
        std::cout << std::setw(12) << token_type_to_string(token.type).to_c_str() << "  |" << std::endl;
    }

    std::cout << "____________________________________" << std::endl << std::endl;
}




Tokens& JsonLexer::lex(const Str& _json_source){

    reset_lexer();

    if(_json_source.size() == 0)
        throw_error("Tried to lex an empty json source string. ");

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

    return tokens;
}
