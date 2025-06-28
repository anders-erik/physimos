#pragma once

#include "lib/file.hh"

#include "test/test.hh"
#include "test/io/json/cat_json.hh"

#include "io/json/json.hh"



UnitTestArray json_strings = {

    {   "\"hello\"",
    [](UnitTest& utest) -> UnitTest&
    {
        Json json {"\"hello\""};

        json.lex_parse();

        JsonVar jvar = json.get_root();

        if(!jvar.is_string())
            return utest.fail();    

        if(jvar.get_string() != "hello")
            return utest.fail("jvar.get_string() != \"hello\"");

        return utest.pass();
    }},


    {   "cat_json_source utility fn",
    [](UnitTest& utest) -> UnitTest&
    {
        Str json_source = cat_json_source("primitives/string.json");

        ResMove<JsonVar> jroot_res = Json::parse(json_source);
        if(jroot_res.has_error())
            return utest.fail(jroot_res.consume_error().str());
        
        JsonVar jroot = jroot_res.consume_value();

        return utest.assert(    jroot.get_string() == "valid_string", 
                                "jroot not string type" ); 
    }},


    {   "cat_and_parse_json utility fn",
    [](UnitTest& utest) -> UnitTest&
    {
        JsonVar jroot = cat_and_parse_json("primitives/string.json");

        return utest.assert(    jroot.is_string(), 
                                "jroot not string type" ); 
    }},



};