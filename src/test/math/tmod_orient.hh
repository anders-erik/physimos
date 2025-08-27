
#pragma once


#include "test/test.hh"

#include "math/rtcd.hh"






UnitTestArray orient_2D = {

    {   "orient_2D : C right of AB",
    [](UnitTest& utest) -> void
    {
        f2 A {0.0f, 0.0f};
        f2 B {1.0f, 1.0f};
        f2 C {1.0f, 0.0f};
        utest.assert_fundamental(   Orient2D(A, B, C),
                                    -1.0f              );
    }},

    {   "orient_2D : C left of AB",
    [](UnitTest& utest) -> void
    {
        f2 A {0.0f, 0.0f};
        f2 B {1.0f, 1.0f};
        f2 C {0.0f, 1.0f};
        utest.assert_fundamental(   Orient2D(A, B, C),
                                    1.0f              );
    }},

    {   "orient_2D : C on AB",
    [](UnitTest& utest) -> void
    {
        f2 A {0.0f, 0.0f};
        f2 B {1.0f, 1.0f};
        f2 C {1.0f, 1.0f};
        utest.assert_fundamental(   Orient2D(A, B, C),
                                    0.0f              );
    }},

};


