#pragma once

#include "test/test.hh"

#include "test/lib/tmod_file.hh"
#include "test/lib/tmod_str.hh"



TCModule file_mod = {
    "file", {
        {"cat_str", cat_str}
    }
};

TCModule str_mod = {
    "str", {
        {"constructors", constructors},
        {"equality", equality},
        {"concat", concat},
        {"substr", substr},
        {"operator_less_than", operator_less_than},
        {"split", split},
    }
};


TCLibrary tclib_lib = {
    "lib",
    {   
        file_mod,
        str_mod
    }
};

