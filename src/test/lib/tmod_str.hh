
#pragma once


#include "str.hh"
#include "str_util.hh"

#include "test/test.hh"



UnitTestArray constructors = {

    {
    "Default.",
    [](UnitTest& utest) -> void 
    {
        Str str1;
        Str str2 {};
        Str str3 = Str{};

        utest.assert(true, true);
    }},

    
    
    {"c-string.",
    [](UnitTest& utest) -> void
    {
        Str str1 = "";
        Str str2 = "000";

        utest.assert(true, true);
    }},


    {"integer 123456: size",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::SI(123456).size(),
                        (uint) 6                );
    }},
    {"integer 123456: [0]",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::SI(123456)[0],
                        (char) '1'            );
    }},
    {"integer 123456: [1]",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::SI(123456)[1],
                        (char) '2');
    }},
    {"integer 123456: [2]",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::SI(123456)[2],
                        (char) '3');
    }},
    {"integer 123456: [3]",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::SI(123456)[3],
                        (char) '4');
    }},
    {"integer 123456: [4]",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::SI(123456)[4],
                        (char) '5');
    }},
    {"integer 123456: [5]",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::SI(123456)[5],
                        (char) '6');
    }},


    
    {"Str::FL(1.1f, 1)",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::FL(1.1f, (uchar)1, Str::FloatRep::Fixed),
                        Str{"1.1"}          );
    }},
    {"Str::FL(55.7f, 1)",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::FL(55.7f, (uchar)1, Str::FloatRep::Fixed),
                        Str{"55.7"}          );
    }},
    {"Str::FL(55.789f, 2)",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str::FL(55.789f, (uchar)2, Str::FloatRep::Fixed),
                        Str{"55.79"}          );
    }},


    
    {"Str{5, 'a'}",
    [](UnitTest& utest) -> void
    {
        utest.assert(   Str{(uint) 5, 'a'}, 
                        Str("aaaaa")    );
    }},


};



UnitTestArray equality = {


    {   "hi[0]",
    [](UnitTest& utest) -> void
    {
        char chars[] = "hi";
        Str str = "hi";
        utest.assert(str[0], chars[0]);
    }},
    {   "hi[1]",
    [](UnitTest& utest) -> void
    {
        char chars[] = "hi";
        Str str = "hi";
        utest.assert(str[1], chars[1]);
    }},


    
    {"shrunk equals default.",
    [](UnitTest& utest) -> void
    {
        Str str_empty_shrunk = "asdfasdfasdlja lskdfjalsdkdfalskd 1111000";
        str_empty_shrunk = "";
        utest.assert(str_empty_shrunk, Str{});
    }},
};



UnitTestArray concat = {


    {   "concat: compare to c_str using strcat().",
    [](UnitTest& utest) -> void
    {

        const char* first_half = "first_half ";
        const char* second_half = "second_half";

        // construct the c string
        char* c_str = new char[22];
        memset(c_str, 0x0, 22);
        memcpy(c_str, first_half, strlen(first_half));
        c_str = strcat(c_str, second_half);
        
        // construct Str
        Str str_first  = first_half;
        Str str_second = second_half;
        Str str = str_first + str_second;

        utest.assert(   str, 
                        Str{c_str}  );
    }},


    
    {   "+ vs. c_str",
    [](UnitTest& utest) -> void
    {
        // construct Str
        Str str_first  = "first_half ";
        Str str_second = "second_half";
        Str str_add = str_first + str_second;
        Str str_manual_concat = "first_half second_half";

        utest.assert(   str_add, 
                        str_manual_concat   );
    }},

};




UnitTestArray substr = {


    {   "cut_to_substr(0, 3) == \"aaa\"",
    [](UnitTest& utest) -> void
    {
        unsigned int rand_size = (unsigned int) 50 + (rand() % 100);
        Str str {rand_size, 'a'};
        
        utest.assert(   str.cut_to_substr(0, 3),
                        Str{"aaa"});
    }},


};




UnitTestArray operator_less_than = {


    {   "\"a\" < \"b\"",
    [](UnitTest& utest) -> void
    {
        Str str_a = "a";
        Str str_b = "b";
        
        utest.assert(   str_a < str_b,
                        true            );
    }},

    {   "\"a\" < \"b\"",
    [](UnitTest& utest) -> void
    {
        Str str_a = "a";
        Str str_b = "b";
        
        utest.assert(   str_a < str_b,
                        true            );
    }},


    {   "\"a\" < \"aa\"",
    [](UnitTest& utest) -> void
    {
        Str str_a = "a";
        Str str_aa = "aa";
        
        utest.assert(   str_a < str_aa,
                        true            );
    }},

    {   "\"a\" < \"aa\"",
    [](UnitTest& utest) -> void
    {
        Str str_a = "a";
        Str str_aa = "aa";
        
        utest.assert(   str_a < str_aa,
                        true            );
    }},


    {   "\"cp0\" < \"ka\"",
    [](UnitTest& utest) -> void
    {
        Str str_ka = "ka";
        Str str_cp0 = "cp0";
        
        utest.assert(   str_cp0 < str_ka,
                        true            );
    }},

    {   "\"ka\" < \"kb\"",
    [](UnitTest& utest) -> void
    {
        Str str_ka = "ka";
        Str str_kb = "kb";
        
        utest.assert(   str_ka < str_kb,
                        true            );
    }},


    {   "\"kb\" < \"kba\"",
    [](UnitTest& utest) -> void
    {
        Str str_kb = "kb";
        Str str_kba = "kba";
        
        utest.assert(   str_kb < str_kba,
                        true            );
    }},

    {   "\"cp0\" < \"kb\"",
    [](UnitTest& utest) -> void
    {
        Str str_cp0 = "cp0";
        Str str_kb  = "kb";
        
        utest.assert(   str_cp0 < str_kb,
                        true            );
    }},


    {   "!(\"k\" < \"cp\")",
    [](UnitTest& utest) -> void
    {
        Str str_k  = "k";
        Str str_cp = "cp";
        
        utest.assert(   !(str_k < str_cp),
                        true            );
    }},


};










UnitTestArray split = {


    {   "split(\"aa, bb, cb\", ',')",
    [](UnitTest& utest) -> void
    {
        Str str = "aa,bb,cc";
        Arr<Str> target_arr;
        target_arr.push_back("aa");
        target_arr.push_back("bb");
        target_arr.push_back("cc");
        
        utest.assert(   StrUtil::split(str, ','),
                        target_arr                  );
    }},


};