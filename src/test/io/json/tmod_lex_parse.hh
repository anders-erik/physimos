#pragma once

#include "lib/file.hh"

#include "test/test.hh"
#include "test/io/json/json_test_util.hh"

#include "io/json/json.hh"



UnitTestArray json_test_setup = {

    {   "\"hello\"",
    [](UnitTest& utest) -> void
    {
        Json j_var;
        j_var.try_parse("\"hello\"");

        utest.assert_custom(   j_var.get_string(), 
                        (Str)"hello"                ); 
    }},


    {   "cat_json_source utility fn",
    [](UnitTest& utest) -> void
    {
        Str json_source = cat_json_source("primitives/string.json");

        ResMove<Json> jroot_res = Json::parse(json_source);
        if(jroot_res.has_error())
            utest.fail(jroot_res.consume_error().to_str());
        
        Json jroot = jroot_res.consume_value();

        utest.assert_custom(   jroot.get_string(), 
                        (Str) "valid_string"    ); 
    }},


    {   "parse_json_test_file utility fn",
    [](UnitTest& utest) -> void
    {
        Json jroot = parse_json_test_file("primitives/string.json");

        utest.assert_custom(   jroot.get_string(),
                        (Str) "valid_string"    ); 
    }},
};






UnitTestArray json_psps = {



    #define psps(name)                                                  \
    {   name,                                                           \
    [](UnitTest& utest) -> void                                         \
    {                                                                   \
        Json     j_var = parse_json_test_file(name);                \
        Str         s1  = j_var.try_serialize();                      \
        j_var.try_parse(s1);                                        \
        Str         s2  = j_var.try_serialize();                     \
                                                                        \
        utest.assert_custom(   s1,                                             \
                        s2  );                                          \
    }},                                                                 \


    psps("arrays/literal_names_array.json")
    psps("arrays/name_literals_nested_array.json")

    psps("floats/floats.json")

    psps("integers/integers.json")
    psps("integers/ints2.json")                                                           

    psps("misc/penpaper.json")
    psps("misc/shapes.json")
    psps("misc/widget.json")
    
    psps("primitives/array.json")
    psps("primitives/false.json")
    psps("primitives/float.json")
    psps("primitives/integer.json")
    psps("primitives/null.json")
    psps("primitives/object.json")
    psps("primitives/string.json")
    psps("primitives/true.json")

    psps("strings/string_array.json")
};




UnitTestArray json_conforms = {

    // conveniend define for repetitive unit test writing
    #define conforms(name) \
    {   name, \
        [](UnitTest& utest) -> void \
        { \
            Str jsource = cat_json_source(name); \
            utest.assert_custom(   Json::parse(jsource).has_error(), false); \
        } \
    },
    

    conforms("arrays/literal_names_array.json")
    conforms("arrays/name_literals_nested_array.json")

    conforms("floats/floats.json")

    conforms("integers/integers.json")
    conforms("integers/ints2.json")

    conforms("misc/penpaper.json")
    conforms("misc/shapes.json")
    conforms("misc/widget.json")
    
    conforms("primitives/array.json")
    conforms("primitives/false.json")
    conforms("primitives/float.json")
    conforms("primitives/integer.json")
    conforms("primitives/null.json")
    conforms("primitives/object.json")
    conforms("primitives/string.json")
    conforms("primitives/true.json")

    // conforms("strings/non_ascii_string.json") // Not suported by parser
    conforms("strings/string_array.json")

};





UnitTestArray json_conforms_not = {

    // conveniend define for repetitive unit test writing
    #define conforms_not(name)                                          \
    {   name,                                                           \
        [](UnitTest& utest) -> void                                     \
        {                                                               \
            Str jsource = cat_json_source(name);                        \
            utest.assert_custom(   Json::parse(jsource).has_error(), true);    \
        }                                                               \
    },
    

    conforms_not("non_valid_lexer/unclosed_string.json")

    conforms_not("non_valid_parser/numbers_invalid.json")

};

