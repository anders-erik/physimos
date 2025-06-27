
#pragma once


#include "test/test.hh"

#include "math/core.hh"






std::vector<UnitTestFn> pow_fns = {


    []() -> UnitTestInfo {
        UnitTestInfo test_info = "pow(5.0, 3)";


        if(pow(5.0, 3) != 125.0)
            return test_info.fail();
        
        return test_info.pass();
    },

};


