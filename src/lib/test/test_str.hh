
#pragma once




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



std::vector<UnitTestFn> constructors = {


    []() -> UnitTestInfo {
        UnitTestInfo test_info = "Default.";

        Str str1;
        Str str2 {};
        Str str3 = Str{};

        return test_info.pass();
    },


    []() -> UnitTestInfo {
        UnitTestInfo test_info = "c-string.";

        Str str1 = "";
        Str str2 = "000";

        return test_info.pass();
    },


    []() -> UnitTestInfo {
        UnitTestInfo test_info = "integer 123456.";

        Str str = 123456;

        if( str.size() == 6 && str[0] == '1' &&  str[1] == '2' &&  str[2] == '3'
            && str[3] == '4' &&  str[4] == '5' &&  str[5] == '6' )
            return test_info.pass();
        else
            return test_info.fail();
    },

    []() -> UnitTestInfo {
        UnitTestInfo test_info = "float.";
        
        Str str;
        Str target;

        str = {1.1f, 1};
        target = "1.1";
        if( str != target )
            return test_info.fail();
        
        str = {55.7f, 1};
        target = "55.7";
        if( str != target )
            return test_info.fail();

        str = {55.789f, 2};
        target = "55.79";
        if( str != target )
            return test_info.fail();
        
        return test_info.pass();
    },


    []() -> UnitTestInfo {
        UnitTestInfo test_info = "initalization char.";

        Str str = Str(5, 'a');
        Str str_c = "aaaaa";

        if( str != str_c )
            return test_info.fail();
        else
            return test_info.pass();

    },


};



std::vector<UnitTestFn> equality = {


    []() -> UnitTestInfo {
        UnitTestInfo test_info = {"Equality: Str and source c string chars."};

        const char* chars = "hi";
        Str str = "hi";

        if(str.size() != 2 || str[0] != chars[0] || str[1] != chars[1])
            return test_info.fail();

        return test_info.pass();
    },


    []() -> UnitTestInfo {
        UnitTestInfo test_info = "equality: equal to itself.";

        Str str = "str";

        for(size_t i = 1; i < 10; i++)
        {
            str += "str";
            if(str != str)
                return test_info.fail();
        }

        Str str_empty_shrunk = "asdfasdfasdlja lskdfjalsdkdfalskd 1111000";
        str_empty_shrunk = "";
        if(str_empty_shrunk != str_empty_shrunk)
            return test_info.fail();

        Str str_empty_constructed = "";
        if(str_empty_constructed != str_empty_shrunk)
            return test_info.fail();

        return test_info.pass();
    },


};



std::vector<UnitTestFn> concat = {


    []() -> UnitTestInfo {
        UnitTestInfo test_info = "concat: compare to c_str using strcat().";

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


    []() -> UnitTestInfo {
        UnitTestInfo test_info = "concat: first plus second == manual paste.";

        // construct Str
        Str str_first  = "first_half ";
        Str str_second = "second_half";
        Str str_concat = str_first + str_second;
        Str str_manual_concat = "first_half second_half";

        
        if(str_concat != str_manual_concat)
            return test_info.fail();

        return test_info.pass();
    },

};




std::vector<UnitTestFn> substr = {


    []() -> UnitTestInfo {
        UnitTestInfo test_info = "substr: random cuts to substr.";

        Str str = "";

        Str str_aaa = "aaa";

        for(size_t i = 0; i < 10; i++)
        {
            unsigned int rand_var = (unsigned int) rand() % 100;
            str = Str{rand_var, 'a'};

            if(str.size() != rand_var)
                return test_info.fail();

            str.cut_to_substr(0, 3);

            if(str != str_aaa)
                return test_info.fail();
        }
        
        return test_info.pass();

        
    },


};

