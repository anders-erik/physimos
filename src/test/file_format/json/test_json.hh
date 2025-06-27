#pragma once

#include "lib/file.hh"

#include "test/test.hh"

#include "file_format/json/json.hh"




std::vector<UnitTest> json_strings = {

    {"\"hello\"",
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


    {"string.json",
    [](UnitTest& utest) -> UnitTest&
    {

        Str path = "src/test/file_format/json/data/string.json";
        File file {path};
        ResMove<Str> jsource_res = file.cat_as_str_core_xplat();
        if(jsource_res.has_error())
        {
            Str error_msg = jsource_res.consume_error().err_s_to_str();
            return utest.fail(error_msg.to_c_str());
        }
        
        Str source_str = jsource_res.consume_value();
        std::string source = source_str.to_c_str();
        Json json {source};

        json.lex_parse();

        JsonVar jvar = json.get_root();


        if(!jvar.is_string())
            return utest.fail();    

        if(jvar.get_string() != "valid_string")
            return utest.fail();

        return utest.pass();
    }},



};