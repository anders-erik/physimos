
#pragma once


#include "str.hh"

#include "test/test.hh"






UnitTestArray constructors = {

    {
    "Default.",
    [](UnitTest& utest) -> void 
    {
        Str str1;
        Str str2 {};
        Str str3 = Str{};

        utest.assert(true);
    }},


    
    {"c-string.",
    [](UnitTest& utest) -> void
    {
        Str str1 = "";
        Str str2 = "000";

        utest.assert(true);
    }},


    {"integer 123456: size",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::SI(123456).size() == 6,
                        "Str::SI(123456).size() != 6");
    }},
    {"integer 123456: [0]",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::SI(123456)[0] == 1,
                        "Str::SI(123456)[0] != 1");
    }},
    {"integer 123456: [1]",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::SI(123456)[1] == 2,
                        "Str::SI(123456)[1] != 2");
    }},
    {"integer 123456: [2]",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::SI(123456)[2] == 3,
                        "Str::SI(123456)[2] != 3");
    }},
    {"integer 123456: [3]",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::SI(123456)[3] == 4,
                        "Str::SI(123456)[3] != 4");
    }},
    {"integer 123456: [4]",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::SI(123456)[4] == 5,
                        "Str::SI(123456)[4] != 5");
    }},
    {"integer 123456: [5]",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::SI(123456)[5] == 6,
                        "Str::SI(123456)[5] != 6");
    }},

    
    {"float.",
    [](UnitTest& utest) -> void
    {
        
        Str str;
        Str target;

        str = Str::FL(1.1f, 1);
        target = "1.1";
        if( str != target )
            utest.fail();
        
        str = Str::FL(55.7f, 1);
        target = "55.7";
        if( str != target )
            utest.fail();

        str = Str::FL(55.789f, 2);
        target = "55.79";
        if( str != target )
            utest.fail();
        
        utest.pass();
    }},


    
    {"initalization char.",
    [](UnitTest& utest) -> void
    {
        Str str_c = "aaaaa";

        Str str;
        str.reserve(5);
        str.fill_alloc('a');

        if( str != str_c )
            utest.fail();
        else
            utest.pass();

    }},


};



UnitTestArray equality = {


    {   "Equality: Str and source c string chars.",
    [](UnitTest& utest) -> void
    {

        const char* chars = "hi";
        Str str = "hi";

        if(str.size() != 2 || str[0] != chars[0] || str[1] != chars[1])
            utest.fail();

        utest.pass();
    }},


    
    {"equality: equal to itself.",
    [](UnitTest& utest) -> void
    {

        Str str = "str";

        for(size_t i = 1; i < 10; i++)
        {
            str += "str";
            if(str != str)
                utest.fail();
        }

        Str str_empty_shrunk = "asdfasdfasdlja lskdfjalsdkdfalskd 1111000";
        str_empty_shrunk = "";
        if(str_empty_shrunk != str_empty_shrunk)
            utest.fail();

        Str str_empty_constructed = "";
        if(str_empty_constructed != str_empty_shrunk)
            utest.fail();

        utest.pass();
    }},


};



UnitTestArray concat = {


    
    {   "concat: compare to c_str using strcat().",
    [](UnitTest& utest) -> void
    {

        const char* first_half = "first_half ";
        const char* second_half = "second_half";

        // construct the c string
        char* c_str = new char[100];
        memset(c_str, 0x0, 100);
        memcpy(c_str, first_half, strlen(first_half));
        c_str = strcat(c_str, second_half);
        
        // construct Str
        Str str_first  = first_half;
        Str str_second = second_half;
        Str str = str_first + str_second;
        
        if(str_equal_c_str_chars(str, c_str))
            utest.pass();

        utest.fail();
    }},


    
    {   "concat: first plus second == manual paste.",
    [](UnitTest& utest) -> void
    {
        // construct Str
        Str str_first  = "first_half ";
        Str str_second = "second_half";
        Str str_concat = str_first + str_second;
        Str str_manual_concat = "first_half second_half";

        
        if(str_concat != str_manual_concat)
            utest.fail();

        utest.pass();
    }},

};




UnitTestArray substr = {


    
    {   "substr: random cuts to substr.",
    [](UnitTest& utest) -> void
    {
        Str str = "";

        Str str_aaa = "aaa";

        for(size_t i = 0; i < 10; i++)
        {
            unsigned int rand_var = (unsigned int) rand() % 100;
            str.reserve(rand_var);
            str.fill_alloc('a');

            if(str.capacity() < rand_var)
                utest.fail("Capacity not enough");

            str.cut_to_substr(0, 3);

            if(str != str_aaa)
                utest.fail("Strings not equal");
        }
        
        utest.pass();

        
    }},


};

