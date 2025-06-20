
#pragma once


#include "test/test.hh"

#include "math/vec.hh"






std::vector<UnitTestFn> construct_vec = {


    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "Vec<double> {}";

        Vec<double> vec;

        if(vec.size() != 0)
            return test_info.fail();
        
        return test_info.pass();
    },



    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "Vec<double> {100}";

        Vec<double> vec {100};

        if(vec.size() != 100)
            return test_info.fail();
        
        return test_info.pass();
    },



    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "set : double, 1.0";

        Vec<double> vec_db {2};

        vec_db.set(1.0);

        if(vec_db[0] != 1.0)
            return test_info.fail();
        
        if(vec_db[1] != 1.0)
            return test_info.fail();
        
        return test_info.pass();
    },


    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "Vec<double> {2, 5.55}";

        Vec<double> vec {2, 5.55};

        if(vec[0] != 5.55)
            return test_info.fail();
        if(vec[1] != 5.55)
            return test_info.fail();
        
        return test_info.pass();
    },


    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "copy";

        Vec<double> vec_db {2};

        vec_db.set(123.0);

        Vec<double> vec_copy = vec_db;

        if(vec_copy[0] != 123.0)
            return test_info.fail();
        if(vec_copy[1] != 123.0)
            return test_info.fail();

        if(vec_copy != vec_copy)
            return test_info.fail();
        
        return test_info.pass();
    },

};






std::vector<UnitTestFn> arithmetic_vec = {


    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "multiply : 2.0";

        Vec<double> vec_db {2};

        vec_db.set(1.0);
        vec_db * 2.0;

        if(vec_db[0] != 2.0)
            return test_info.fail();
        
        if(vec_db[1] != 2.0)
            return test_info.fail();
        
        return test_info.pass();
    },

    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "divide : 2.0";

        Vec<double> vec_db {2};

        vec_db.set(1.0);
        vec_db / 2.0;

        if(vec_db[0] != 0.5)
            return test_info.fail();
        
        if(vec_db[1] != 0.5)
            return test_info.fail();
        
        return test_info.pass();
    },

    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "add : 2.0";

        Vec<double> vec_src {2}; // inital vec
        Vec<double> vec_target {2}; // target vec adter addition

        vec_src.set(1.0);
        vec_target.set(3.0);

        vec_src + 2.0;

        if(vec_src != vec_target)
            return test_info.fail();
        
        return test_info.pass();
    },

    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "subtract : 2.0";

        double inital_value = 1.0;
        double sub_value = 2.0;
        double target_value = -1.0;

        Vec<double> vec_db {2};

        vec_db.set(inital_value);

        vec_db - sub_value;


        if(vec_db[0] != target_value)
            return test_info.fail();
        
        if(vec_db[1] != target_value)
            return test_info.fail();
        
        return test_info.pass();
    },

};

