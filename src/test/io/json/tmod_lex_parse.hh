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

        if(!jvar.is_string())
            utest.fail();    

        if(jvar.get_string() != "hello")
            utest.fail("jvar.get_string() != \"hello\"");

        utest.pass();
    }},


    {   "cat_json_source utility fn",
    [](UnitTest& utest) -> void
    {
        Str json_source = cat_json_source("primitives/string.json");

        ResMove<JsonVar> jroot_res = Json::parse(json_source);
        if(jroot_res.has_error())
            utest.fail(jroot_res.consume_error().to_str());
        
        JsonVar jroot = jroot_res.consume_value();

        utest.assert(    jroot.get_string() == "valid_string", 
                                "jroot not string type" ); 
    }},


    {   "parse_json_test_file utility fn",
    [](UnitTest& utest) -> void
    {
        JsonVar jroot = parse_json_test_file("primitives/string.json");

        utest.assert(    jroot.is_string(), 
                                "jroot not string type" ); 
    }},
};






UnitTestArray json_psps = {


    {   "primitives/string.json",
    [](UnitTest& utest) -> void
    {
        JsonVar parsed_1 = parse_json_test_file("primitives/string.json");
        Str serialized_1 = Json::serialize(parsed_1);
        ResMove<JsonVar> parsed_2 = Json::parse(serialized_1);
        Str serialized_2 = Json::serialize(parsed_2.consume_value());

        utest.assert(serialized_1 == serialized_2,
                            "serialized_1 != serialized_2");

    }},
};