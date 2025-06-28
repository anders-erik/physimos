
#pragma once


#include "test/test.hh"

#include "math/core.hh"






UnitTestArray pow_fns = {

    {   "pow(5.0, 3)",
    [](UnitTest& utest) -> void
    {
        if(pow(5.0, 3) != 125.0)
            utest.fail();
        
        utest.pass();
    }},

};


