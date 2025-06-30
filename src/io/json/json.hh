
#pragma once

#include <string>
#include <iostream>

#include "lib/res.hh"

#include "json_types.hh"
#include "json_lexer.hh"
#include "json_parser.hh"
#include "json_serialize.hh"


class Json {
        
            Str json_source;    // Original json source to parse
            Tokens      tokens;         // Lexed Json -- including whitespace
            JsonVar     root_var;       // Parsed Json - whitespace removed included
            
            bool is_lexed = false;
            bool is_parsed = false;

        public:

            Json(Str _json_source) 
                : json_source {_json_source} 
            {};

            void set_json_source(Str _json_source){
                json_source = _json_source;
                is_lexed = false;
                is_parsed = false;
            }

            void lex(){
                if(is_lexed)
                    std::cout << "Warning: Lexing already lexed json source. " << std::endl;

                JsonLexer lexer;
                tokens = lexer.lex(json_source);
                is_lexed = true;
            }
            void print_tokens(){
                if(is_lexed)
                    JsonLexer::print_tokens(tokens);
                else 
                    std::cout << "Warning: can't print tokens. Source has not been lexed. " << std::endl;
            }

            JsonVar& parse(){

                if(tokens.get_count() < 1)
                    std::cout << "Warning: No tokens to parse. " << std::endl;
                if(is_parsed)
                    std::cout << "Warning: parsing already parsed tokens. " << std::endl;
                    
                JsonParser parser (json_source, tokens); 
                root_var = parser.parse();

                is_parsed = true;
                return root_var;
            }

            JsonVar& get_root(){
                if(!is_parsed)
                    throw_error("Tried to access root value before successful parse.");
                return root_var;
            }

            Str serialize() {
                if(!is_parsed){
                    std::cout << "Warning: can't serialize json. Tokens have not been parsed. " << std::endl;
                    return "";
                }
                
                JsonSerializer serializer;
                serializer.set_config( {serial_ws::new_lines, 4} );
                return serializer.serialize(root_var);
            }

            JsonVar& lex_parse(){
                lex();
                return parse();
            };
            JsonVar& lex_parse(Str _json_source){
                set_json_source(_json_source);
                lex();
                return parse();
            };

            static void throw_error(Str error_msg){
                // std::string base_msg = "Json Error: \n";
                // std::string complete_msg = base_msg + error_msg + "\n";
                // throw std::runtime_error(complete_msg);
                
            }

            static ResMove<JsonVar> parse(Str json_source)
            {
                Json json {json_source};
                try
                {
                    json.lex();
                    json.parse();
                    return {std::move(json.get_root())};
                }
                catch(const std::exception& e)
                {
                    Err err;
                    err.message = e.what();
                    return {MV(err)};
                }
            }

            static Str serialize(const JsonVar& json_var)
            {    
                JsonSerializer serializer;
                serializer.set_config( {serial_ws::new_lines, 4} );
                // serializer.serialize()
                Str serialized_string = serializer.serialize(json_var);
                return serialized_string;
            }

        };

