#pragma once

#include "test/test.hh"
#include "test/io/bmp/test_bmp.hh"


TCModule bmp_mod = {
    "bmp_mod",{
        {"one_black_pixel", one_black_pixel},
        {"endian_swap", endian_swap},
    }
};

TCLibrary tclib_bmp = {
    "bmp",
    {
        bmp_mod,
    },
};


