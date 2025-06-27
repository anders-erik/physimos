
#pragma once

#include "lib/str.hh"
#include "lib/print.hh"

#include <functional>
#include <vector>

#include "lib/str.hh"

struct UnitTest;
typedef std::vector<UnitTest> UnitTestArray;

typedef std::function<UnitTest&(UnitTest& utest)> UnitTestFn;

/** Unit test information returned by completed unit test. */
class UnitTest 
{
    std::string     name                = "";            // name of unit test
    UnitTestFn      test_fn;
    std::string     fail_message        = "";            // message printed if unit test fails
    bool            pass_flag           = false;
    int             print_indentation   = 12;

public:

    UnitTest(const char* name , UnitTestFn unit_test_fn) 
        :   name            {name}, 
            test_fn   {unit_test_fn} 
    {
    };

    /** Set passed to true and return itself */
    UnitTest& run()
    {
        return test_fn(*this);
    }


    /** Set passed to true and return itself */
    UnitTest& pass()
    {
        pass_flag = true;
        return *this;
    }

    /** Set passed to false and return itself */
    UnitTest& fail()
    {
        pass_flag = false;
        return *this;
    }

    /** Set passed to false and return itself */
    UnitTest& fail(std::string new_fail_message)
    {
        fail_message = new_fail_message;
        pass_flag = false;
        return *this;
    }

    /** Passes the unit test of true assert statement. */
    UnitTest& assert(bool assert_statement)
    {
        if(assert_statement)
            return pass();
        else
            return fail();
        
    }

    /** Check if test passed. */
    bool is_passed()
    { 
        return pass_flag ? true : false;
    }

    /** Print */
    void print()
    {
        print_indent(print_indentation);
        print_check(pass_flag);
        std::cout << name << ": " << fail_message << std::endl;
    }

};




