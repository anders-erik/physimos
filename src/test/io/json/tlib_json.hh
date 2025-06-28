#pragma once

#include "test/test.hh"
#include "test/io/json/tmod_lex_parse.hh"


TCModule json_lex_parse = {
    "json_lex_parse",{
        {"json_test_setup", json_test_setup},
        {"json_psps", json_psps},
    }
};

TCLibrary tclib_json = {
    "json",
    {
        json_lex_parse,
    },
};


