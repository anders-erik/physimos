#pragma once
#include <vector>
#include <iomanip>

#include "physon_types.hh"

#include "json_token.hh"


struct JsonLexer {

    JsonLexer() {};
    // JsonLexer(const std::string& _json_source) : json_source{_json_source} {};

    std::vector<Token>& lex(std::string& _json_source);
    void print_tokens();


private:

    std::string json_source;
    size_t index = 0;
    std::vector<Token> tokens;

    void reset_lexer();

    void increment_index();
    void increment_index(int count);

    char current_char();

    bool is_whitespace();
    bool is_whitespace(char ch);
    bool is_null_literal();
    bool is_true_literal();
    bool is_false_literal();

    /** Grab current char and increment index. */
    char consume_char();
    /** Increment index and create token. */
    void consume_ws();
    void consume_array_open();
    void consume_array_close();
    void consume_literal_null();
    void consume_literal_true();
    void consume_literal_false();
    void consume_comma();

    void throw_error(std::string error_msg);
};

void JsonLexer::reset_lexer(){
    index = 0;
    tokens.clear();
    json_source.clear();
}

void JsonLexer::increment_index(){
    ++index;
}
void JsonLexer::increment_index(int count){
    index += count;
}

char JsonLexer::current_char(){
    return json_source[index];
}




bool JsonLexer::is_whitespace(){
    char ch = json_source[index];
    return ch == SPACE || ch == TAB || ch == NEW_LINE || ch == CARRIAGE_RETURN;
}
bool JsonLexer::is_whitespace(char ch){
    return ch == SPACE || ch == TAB || ch == NEW_LINE || ch == CARRIAGE_RETURN;
}

bool JsonLexer::is_null_literal(){
    return  json_source[index+0] == 'n' &&
            json_source[index+1] == 'u' &&
            json_source[index+2] == 'l' &&
            json_source[index+3] == 'l';
}
bool JsonLexer::is_true_literal(){
    return  json_source[index+0] == 't' &&
            json_source[index+1] == 'r' &&
            json_source[index+2] == 'u' &&
            json_source[index+3] == 'e';
}
bool JsonLexer::is_false_literal(){
    return  json_source[index+0] == 'f' &&
            json_source[index+1] == 'a' &&
            json_source[index+2] == 'l' &&
            json_source[index+3] == 's' &&
            json_source[index+4] == 'e';
}

char JsonLexer::consume_char(){
    return json_source[index++];
}
void JsonLexer::consume_ws(){
    std::string ws_str = "";

    while(is_whitespace()){
        ws_str += consume_char();
    }

    if(ws_str.length() > 0){
        Token token (token_t::whitespace);
        tokens.push_back(token);
    }
}

void JsonLexer::consume_array_open(){

    Token new_token (token_t::array_open);
    increment_index();

    tokens.push_back(new_token);

}
void JsonLexer::consume_array_close(){

    Token new_token (token_t::array_close);
    increment_index();

    tokens.push_back(new_token);

}

void JsonLexer::consume_literal_null(){

    if(!is_null_literal())
        throw_error("Expected null literal. Invalid chars.");

    increment_index(4);

    Token new_token (token_t::null_);

    tokens.push_back(new_token);

}
void JsonLexer::consume_literal_true(){

    if(!is_true_literal())
        throw_error("Expected true literal. Invalid chars.");

    increment_index(4);

    Token new_token (token_t::true_);

    tokens.push_back(new_token);

}
void JsonLexer::consume_literal_false(){

    if(!is_false_literal())
        throw_error("Expected false literal. Invalid chars.");
    
    increment_index(5);

    Token new_token (token_t::false_);

    tokens.push_back(new_token);

}

void JsonLexer::consume_comma(){

    Token new_token (token_t::comma);
    increment_index();

    tokens.push_back(new_token);

}






void JsonLexer::print_tokens() {

    std::cout << "Tokens: " << std::endl << std::endl;

    std::cout << "   Type       |" << std::endl;
    std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
    
    
    for (const auto& token : tokens) {
        std::cout << std::setw(12) << token_type_to_string(token.type) << "  |" << std::endl;
    }

    std::cout << "____________________________________" << std::endl << std::endl;
}

void JsonLexer::throw_error(std::string error_msg){

    std::string state_string = "\n    Current char  = " + json_source.substr(index, 1) + "\n    Current index = " + std::to_string(index);

    std::string full_error_str = "Lexer: " + error_msg + state_string;

    throw std::runtime_error(full_error_str);

}



std::vector<Token>& JsonLexer::lex(std::string& _json_source){

    reset_lexer();

    json_source = _json_source;

    consume_ws();

    while(index < json_source.size()){
    
        // NEW VALUE CHECKS
        switch(current_char())
        {
        
        case '[':
            consume_array_open();
            break;
        case ']':
            consume_array_close();
            break;

        case 'n':
            consume_literal_null();
            break;
        case 't':
            consume_literal_true();
            break;
        case 'f':
            consume_literal_false();
            break;

        case ',':
            consume_comma();
            break;
        
        default:
            throw_error("Unknown char. ");
            break;
        
        }

        consume_ws();
    
    }

    return tokens;
}

