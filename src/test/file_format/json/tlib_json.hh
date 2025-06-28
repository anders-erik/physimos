#pragma once

#include "test/test.hh"
#include "test/file_format/json/tmod_json_strings.hh"


TCModule json_primitives = {
    "json_primitives",{
        {"strings", json_strings},
    }
};

TCLibrary tclib_json = {
    "json",
    {
        json_primitives,
    },
};


