
#pragma once

#include <string>
#include <iostream>

#include "physon_types.hh"

#include "json_variant.hh"
#include "json_lexer.hh"
#include "json_parser.hh"
#include "json_serialize.hh"

class Json {
        
            std::string json_source;
            Tokens tokens;
            JsonVar root_var;
            
            bool is_lexed = false;
            bool is_parsed = false;

        public:

            Json(std::string _json_source) : json_source {_json_source} {};

            void set_json_source(std::string _json_source){
                json_source = _json_source;
                is_lexed = false;
                is_parsed = false;
            }

            void lex(){
                JsonLexer lexer;
                tokens = lexer.lex(json_source);
                is_lexed = true;
            }

            JsonVar& parse(){

                if(tokens.get_count() < 1)
                    std::cout << "Warning: No tokens to parse. " << std::endl;
                    
                JsonParser parser (json_source, tokens); 
                
                // try
                // {
                root_var = parser.parse();
                // }
                // catch(const std::exception& e)
                // {
                //     std::cout << "Json Parsing error." << std::endl;
                //     std::cerr << e.what() << '\n';
                // }

                is_parsed = true;
                return root_var;
            }

            JsonVar& lex_parse(){
                lex();
                return parse();
            };

            void print_tokens(){
                if(is_lexed)
                    JsonLexer::print_tokens(tokens);
                else 
                    std::cout << "Warning: can't print tokens. Source has not been lexed. " << std::endl;
            }

            std::string serialize() {
                if(!is_parsed){
                    std::cout << "Warning: can't serialize json. Tokens have not been parsed. " << std::endl;
                    return "";
                }
                
                JsonSerializer serializer;
                serializer.set_config( {serial_ws::new_lines, 4} );
                return serializer.serialize(root_var);
            }

        };

