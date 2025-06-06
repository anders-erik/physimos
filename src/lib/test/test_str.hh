
#pragma once

#include <new>

#include <string>
#include <vector>
#include <functional>


#include "str.hh"

#include "test/test.hh"


bool str_equal_c_str_chars(Str str, const char* c_str){

    if(str.size() != strlen(c_str))
        return false;
    
    for(size_t i = 0; i < str.size(); i++)
        if(str[i] != c_str[i])
            return false;

    return true;
}



std::vector<UnitTest> constructors = {


    []() -> InfoUnitTest {
        InfoUnitTest test_info = "Default constructors.";

        Str str1;
        Str str2 {};
        Str str3 = Str{};

        return test_info.pass();
    },


    []() -> InfoUnitTest {
        InfoUnitTest test_info = "C string constructors.";

        Str str1 = "";
        Str str2 = "000";

        return test_info.pass();
    },


    []() -> InfoUnitTest {
        InfoUnitTest test_info = "Constructor: integer 123456.";

        Str str = 123456;

        if( str.size() == 6 && str[0] == '1' &&  str[1] == '2' &&  str[2] == '3'
            && str[3] == '4' &&  str[4] == '5' &&  str[5] == '6' )
            return test_info.pass();
        else
            return test_info.fail();
    },


};



UnitTestCollection equality = {


    []() -> InfoUnitTest {
        InfoUnitTest test_info = "Equality: Str and source c string chars.";

        const char* chars = "hi";
        Str str = chars;

        if(str.size() != 2 || str[0] != chars[0] || str[1] != chars[1])
            return test_info.fail();

        return test_info.pass();
    },


};



UnitTestCollection concat = {


    []() -> InfoUnitTest {
        InfoUnitTest test_info = "concat: compare to c_str using strcat().";

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
            return test_info.pass();

        return test_info.fail();
    },


};

