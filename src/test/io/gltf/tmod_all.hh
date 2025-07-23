#pragma once

#include "lib/file.hh"

#include "test/test.hh"

#include "io/json/json.hh"



UnitTestArray camera = {

    {   "\"hello\"",
    [](UnitTest& utest) -> void
    {
        Json j_var;
        j_var.try_parse("\"hello\"");

        utest.assert(   j_var.get_string(), 
                        (Str)"hesllo"                ); 
    }},

};





