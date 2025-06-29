
#pragma once


#include "test/test.hh"

#include "math/core.hh"






UnitTestArray pow_fns = {

    {   "pow(5.0, 3)",
    [](UnitTest& utest) -> void
    {
        utest.assert(   pow(5.0, 3),
                        125.0       );
    }},

};


