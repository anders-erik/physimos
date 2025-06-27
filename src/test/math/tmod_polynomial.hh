
#pragma once


#include "test/test.hh"

#include "math/polynomial.hh"






std::vector<UnitTest> construct_poly = {


    {   "no coefficent",
    [](UnitTest& utest) -> UnitTest&
    {
        Polynomial<double> p {{}};

        if(p.is_valid())
            return utest.fail();
        
        return utest.pass();
    }},

};



std::vector<UnitTest> evaluate = {


    {   "order 0: 5",
    [](UnitTest& utest) -> UnitTest&
    {
        Polynomial<double> p {{5.0}};

        if(p.evaluate(0) == 5.0)
            return utest.pass();
        
        return utest.fail();
    }},


    {   "order 0: 0.000001",
    [](UnitTest& utest) -> UnitTest&
    {
        Polynomial<double> p {{0.000001}};

        if(p.evaluate(0) == 0.000001)
            return utest.pass();
        
        return utest.fail();
    }},


    {   "order 1: {0, 1}, 5 -> 5",
    [](UnitTest& utest) -> UnitTest&
    {
        Polynomial<double> poly {{0, 1}};

        if(poly(5.0) == 5.0)
            return utest.pass();
        
        return utest.fail();
    }},


    {   "order 3: {0, 1, 0, 1}, 5 -> 5 + 5^3",
    [](UnitTest& utest) -> UnitTest&
    {
        Polynomial<double> poly {{0, 1, 0, 1}};

        // 130.0
        double expected_result = 1.0*5.0 + 1.0*5.0*5.0*5.0;
        

        if(poly(5.0) == expected_result)
            return utest.pass();
        
        return utest.fail();
    }},


    {   "order 3: {0, -1, 0, 0.0001}",
    [](UnitTest& utest) -> UnitTest&
    {
        Polynomial<double> poly {{0, -1, -0.2, 0.0001}};

        double input = 5.0;
        double expected_result = -1.0*input + -0.2 * input * input + 0.0001*input*input*input;
        

        if(poly(input) == expected_result)
            return utest.pass();
        
        return utest.fail();
    }},

};

