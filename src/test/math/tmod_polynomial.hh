
#pragma once


#include "test/test.hh"

#include "math/polynomial.hh"






UnitTestArray construct_poly = {


    {"At least one coefficient",
    [](UnitTest& utest) -> void
    {
        utest.assert_custom(   Polynomial<double>{}.is_valid(),
                        false                            );
    }},

};



UnitTestArray evaluate = {


    {"f=5 : f(0) == 5",
    [](UnitTest& utest) -> void
    {
        Polynomial<double> p {{5.0}};

        utest.assert_custom(   p.evaluate(0),
                        5.0             );
    }},

    {"f=0.000001 : f(0) == 0.000001",
    [](UnitTest& utest) -> void
    {
        Polynomial<double> p {{0.000001}};

        utest.assert_custom(   p.evaluate(0),
                        0.000001        );
    }},

    {"f=x : f(5) = 5",
    [](UnitTest& utest) -> void
    {
        Polynomial<double> poly {{0, 1}};

        utest.assert_custom(   poly.evaluate(5.0),
                        5.0                 );
    }},


    {   "f= x + x^3 : f(5) = 130",
    [](UnitTest& utest) -> void
    {
        Polynomial<double> poly {{0, 1, 0, 1}};

        utest.assert_custom(   poly(5.0),
                        130.0       );
    }},


    {   "f= -x - 0.3x^2 + 0.0001x^3 x^3  : f(5.0) = ",
    [](UnitTest& utest) -> void
    {
        Polynomial<double> poly {{0, -1, -0.2, 0.0001}};

        // double expected_calc = -1.0*input + -0.2 * input * input + 0.0001*input*input*input;
        double expected = -9.9875000000000007;;

        utest.assert_custom(   poly(5.0),
                        expected );
    }},

};

