#pragma once

#include "lib/file.hh"

#include "test/test.hh"
#include "test/io/json/json_test_util.hh"

#include "io/json/json.hh"



UnitTestArray json_test_setup = {

    {   "\"hello\"",
    [](UnitTest& utest) -> void
    {
        Json json {"\"hello\""};

        json.lex_parse();

        JsonVar jvar = json.get_root();

        utest.assert(   (Str) jvar.get_string().c_str(), 
                        (Str) "hello"                ); 
    }},


    {   "cat_json_source utility fn",
    [](UnitTest& utest) -> void
    {
        Str json_source = cat_json_source("primitives/string.json");

        ResMove<JsonVar> jroot_res = Json::parse(json_source);
        if(jroot_res.has_error())
            utest.fail(jroot_res.consume_error().to_str());
        
        JsonVar jroot = jroot_res.consume_value();

        utest.assert(   (Str) jroot.get_string().c_str(), 
                        (Str) "valid_string"                ); 
    }},


    {   "parse_json_test_file utility fn",
    [](UnitTest& utest) -> void
    {
        JsonVar jroot = parse_json_test_file("primitives/string.json");

        utest.assert(   (Str) jroot.get_string().c_str(),
                        (Str) "valid_string"                ); 
    }},
};






UnitTestArray json_psps = {

    #define psps(name)                                                  \
    {   name,                                                           \
    [](UnitTest& utest) -> void                                         \
    {                                                                   \
        JsonVar             p1  = parse_json_test_file(name);           \
        Str                 s1  = Json::serialize(p1);                  \
        ResMove<JsonVar>    p2  = Json::parse(s1);                      \
        Str                 s2  = Json::serialize(p2.consume_value());  \
        utest.assert(   s1,                                             \
                        s2);                                            \
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
            utest.assert(   Json::parse(jsource).has_error(), false); \
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
            utest.assert(   Json::parse(jsource).has_error(), true);    \
        }                                                               \
    },
    

    conforms_not("non_valid_lexer/unclosed_string.json")

    conforms_not("non_valid_parser/numbers_invalid.json")

};



// UnitTestArray json_bugs = {

//     {"memory corruption during for-each loop",
//     [](UnitTest& utest) -> void
//     {
//         JsonVar jroot = parse_json_test_file("misc/mesh_minimal.json");

//         j_object mesh_object = jroot.object_find("mesh").get_object();

//         utest.assert(   mesh_object[0].first,
//                         j_string("sheet")       );
//     }},

// };