
#pragma once


#include "test/test.hh"

#include "math/vec.hh"






UnitTestArray construct_vec = {

    {   "Vec<double>{}.size() == 0",
    [](UnitTest& utest) -> void
    {
        utest.assert_custom(   Vec<double>{}.size(),
                        (uint) 0);
    }},


    {   "Vec<double>{100}.size() == 100",
    [](UnitTest& utest) -> void
    {
        utest.assert_custom(   Vec<double>{(uint)100}.size(), 
                        (uint)100               );
    }},


    {   "Vec<double>{4, 1.0} == expected[0-3] = 1.0",
    [](UnitTest& utest) -> void
    {
        Vec<double> expected {(uint) 4};
        expected[0] = 1.0;
        expected[1] = 1.0;
        expected[2] = 1.0;
        expected[3] = 1.0;
        utest.assert_custom(   Vec<double>{(uint)4, 1.0},
                        expected                    );
    }},


    {   "Vec<double> {2, 5.55} == expected[0-1] = 5.55",
    [](UnitTest& utest) -> void
    {
        Vec<double> expected {(uint) 2};
        expected[0] = 5.55;
        expected[1] = 5.55;

        utest.assert_custom(   Vec<double>{2, 5.55},
                        expected                    );
    }},


    {   "Vec<double>{9} : set(123.0)",
    [](UnitTest& utest) -> void
    {
        Vec<double> vec_man {(uint)9};
        Vec<double> vec_set {(uint)9};

        for(uint i = 0; i < vec_man.size(); i++)
            vec_man[i] = 123.0;

        utest.assert_custom(   vec_set.set(123.0),
                        vec_man             );
    }},

    {   "Vec<double>{9, 123} : copy()",
    [](UnitTest& utest) -> void
    {
        Vec<double> vec_target {(uint)9, 123.0};

        Vec<double> vec_copy = vec_target;

        utest.assert_custom(   vec_copy,
                        vec_target  );
    }},

};






UnitTestArray arithmetic_vec = {

    {   "Vec<double> {2, 1.0} : *= 2.0",
    [](UnitTest& utest) -> void
    {
        Vec<double> vec {(uint)2, 1.0};
        Vec<double> vec_target {(uint)2, 2.0};

        vec *= 2.0;

        utest.assert_custom(   vec,
                        vec_target  );
    }},

    {   "Vec<double> {2, 1.0} : /= 2.0",
    [](UnitTest& utest) -> void
    {
        Vec<double> vec {(uint)2, 1.0};
        Vec<double> vec_target {(uint)2, 0.5};

        vec /= 2.0;

        utest.assert_custom(   vec,
                        vec_target  );
    }},

    {   "Vec<double> {2, 1.0} : += 2.0",
    [](UnitTest& utest) -> void
    {
        Vec<double> vec {(uint)2, 1.0};
        Vec<double> vec_target {(uint)2, 3.0};

        vec += 2.0;

        utest.assert_custom(   vec,
                        vec_target  );
    }},

    {   "Vec<double> {2, 3.0} : -= 2.0",
    [](UnitTest& utest) -> void
    {
        Vec<double> vec {(uint)2, 3.0};
        Vec<double> vec_target {(uint)2, 1.0};

        vec -= 2.0;

        utest.assert_custom(   vec,
                        vec_target  );
    }},

};

