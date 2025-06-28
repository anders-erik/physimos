
#pragma once


#include "str.hh"

#include "test/test.hh"






UnitTestArray constructors = {

    {
    "Default.",
    [](UnitTest& utest) -> UnitTest& 
    {
        Str str1;
        Str str2 {};
        Str str3 = Str{};

        return utest.pass();
    }},


    
    {"c-string.",
    [](UnitTest& utest) -> UnitTest&
    {
        Str str1 = "";
        Str str2 = "000";

        return utest.pass();
    }},


    
    {"integer 123456.",
    [](UnitTest& utest) -> UnitTest&
    {
        Str str = Str::SI(123456);

        if( str.size() == 6 && str[0] == '1' &&  str[1] == '2' &&  str[2] == '3'
            && str[3] == '4' &&  str[4] == '5' &&  str[5] == '6' )
            return utest.pass();
        else
            return utest.fail();
    }},

    
    {"float.",
    [](UnitTest& utest) -> UnitTest&
    {
        
        Str str;
        Str target;

        str = Str::FL(1.1f, 1);
        target = "1.1";
        if( str != target )
            return utest.fail();
        
        str = Str::FL(55.7f, 1);
        target = "55.7";
        if( str != target )
            return utest.fail();

        str = Str::FL(55.789f, 2);
        target = "55.79";
        if( str != target )
            return utest.fail();
        
        return utest.pass();
    }},


    
    {"initalization char.",
    [](UnitTest& utest) -> UnitTest&
    {
        Str str_c = "aaaaa";

        Str str;
        str.reserve(5);
        str.fill_alloc('a');

        if( str != str_c )
            return utest.fail();
        else
            return utest.pass();

    }},


};



UnitTestArray equality = {


    {   "Equality: Str and source c string chars.",
    [](UnitTest& utest) -> UnitTest&
    {

        const char* chars = "hi";
        Str str = "hi";

        if(str.size() != 2 || str[0] != chars[0] || str[1] != chars[1])
            return utest.fail();

        return utest.pass();
    }},


    
    {"equality: equal to itself.",
    [](UnitTest& utest) -> UnitTest&
    {

        Str str = "str";

        for(size_t i = 1; i < 10; i++)
        {
            str += "str";
            if(str != str)
                return utest.fail();
        }

        Str str_empty_shrunk = "asdfasdfasdlja lskdfjalsdkdfalskd 1111000";
        str_empty_shrunk = "";
        if(str_empty_shrunk != str_empty_shrunk)
            return utest.fail();

        Str str_empty_constructed = "";
        if(str_empty_constructed != str_empty_shrunk)
            return utest.fail();

        return utest.pass();
    }},


};



UnitTestArray concat = {


    
    {   "concat: compare to c_str using strcat().",
    [](UnitTest& utest) -> UnitTest&
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
            return utest.pass();

        return utest.fail();
    }},


    
    {   "concat: first plus second == manual paste.",
    [](UnitTest& utest) -> UnitTest&
    {
        // construct Str
        Str str_first  = "first_half ";
        Str str_second = "second_half";
        Str str_concat = str_first + str_second;
        Str str_manual_concat = "first_half second_half";

        
        if(str_concat != str_manual_concat)
            return utest.fail();

        return utest.pass();
    }},

};




UnitTestArray substr = {


    
    {   "substr: random cuts to substr.",
    [](UnitTest& utest) -> UnitTest&
    {
        Str str = "";

        Str str_aaa = "aaa";

        for(size_t i = 0; i < 10; i++)
        {
            unsigned int rand_var = (unsigned int) rand() % 100;
            str.reserve(rand_var);
            str.fill_alloc('a');

            if(str.capacity() < rand_var)
                return utest.fail("Capacity not enough");

            str.cut_to_substr(0, 3);

            if(str != str_aaa)
                return utest.fail("Strings not equal");
        }
        
        return utest.pass();

        
    }},


};

