
#pragma once

#include "lib/str.hh"
#include "lib/print.hh"

#include <functional>
#include <vector>

#include "lib/str.hh"

struct UnitTest;


typedef std::function<UnitTest&(UnitTest& utest)> UnitTestFn;

/** Test function and info. */
struct UnitTest 
{
    Str         name;                       // name of unit test
    UnitTestFn  test_fn;
    Str         fail_message;               // message printed if unit test fails
    bool        pass_flag       = false;
    int         indentation     = 12;


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
    UnitTest& fail(Str new_fail_message)
    {
        fail_message = new_fail_message;
        pass_flag = false;
        return *this;
    }

    /** Passes the unit test of true assert statement. */
    UnitTest& assert(bool assert_statement, Str new_fail_message)
    {
        if(assert_statement)
            return pass();
        else
            return fail(new_fail_message);
        
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
    bool passed()
    { 
        return pass_flag ? true : false;
    }

    /** Print */
    void print_result()
    {
        print_indent(indentation);
        print_check(pass_flag);
        Print::line(name, ": ", fail_message);
    }

};




