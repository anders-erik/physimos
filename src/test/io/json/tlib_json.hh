#pragma once

#include "test/test.hh"
#include "test/io/json/tmod_lex_parse.hh"


TCModule json_lex_parse = {
    "json_lex_parse",{
        {"test_setup",      json_test_setup},
        {"psps",            json_psps},
        {"conforms",        json_conforms},
        {"conforms_not",    json_conforms_not},
    }
};

TCLibrary tclib_json = {
    "json",
    {
        json_lex_parse,
    },
};


