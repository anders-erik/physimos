#pragma once

#include "test/test.hh"

#include "test/lib/tmod_file.hh"
#include "test/lib/tmod_str.hh"



TestModule file_mod = {
    "file", {
        {"cat_str", cat_str}
    }
};

TestModule str_mod = {
    "str", {
        {"constructors", constructors},
        {"equality", equality},
        {"concat", concat},
        {"substr", substr},
    }
};


TestLibrary library_lib = {
    "lib",
    {   
        file_mod,
        str_mod
    }
};

