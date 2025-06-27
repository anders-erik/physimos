
#pragma once


#include "test/test.hh"

#include "math/polynomial.hh"






std::vector<UnitTestFn> construct_poly = {


    []() -> UnitTestInfo {
        UnitTestInfo test_info = "no coefficent";

        Polynomial<double> p {{}};

        if(p.is_valid())
            return test_info.fail();
        
        return test_info.pass();
    },

};



std::vector<UnitTestFn> evaluate = {


    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "order 0: 5";

        Polynomial<double> p {{5.0}};

        if(p.evaluate(0) == 5.0)
            return test_info.pass();
        
        return test_info.fail();
    },


    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "order 0: 0.000001";

        Polynomial<double> p {{0.000001}};

        if(p.evaluate(0) == 0.000001)
            return test_info.pass();
        
        return test_info.fail();
    },


    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "order 1: {0, 1}, 5 -> 5";

        Polynomial<double> poly {{0, 1}};

        if(poly(5.0) == 5.0)
            return test_info.pass();
        
        return test_info.fail();
    },


    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "order 3: {0, 1, 0, 1}, 5 -> 5 + 5^3";

        Polynomial<double> poly {{0, 1, 0, 1}};

        // 130.0
        double expected_result = 1.0*5.0 + 1.0*5.0*5.0*5.0;
        

        if(poly(5.0) == expected_result)
            return test_info.pass();
        
        return test_info.fail();
    },


    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "order 3: {0, -1, 0, 0.0001}";

        Polynomial<double> poly {{0, -1, -0.2, 0.0001}};

        double input = 5.0;
        double expected_result = -1.0*input + -0.2 * input * input + 0.0001*input*input*input;
        

        if(poly(input) == expected_result)
            return test_info.pass();
        
        return test_info.fail();
    },

};

