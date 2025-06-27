
#pragma once


#include "test/test.hh"

#include "math/core.hh"






std::vector<UnitTest> pow_fns = {

    {   "pow(5.0, 3)",
    [](UnitTest& utest) -> UnitTest&
    {
        if(pow(5.0, 3) != 125.0)
            return utest.fail();
        
        return utest.pass();
    }},

};


