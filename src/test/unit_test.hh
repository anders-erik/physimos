
#pragma once

#include "lib/str.hh"
#include "lib/print.hh"

#include "io/pretty_str.hh"

#include <functional>
#include <vector>

#include "lib/str.hh"

struct UnitTest;

using UnitTestFn = std::function<void(UnitTest& utest)>;

using UnitTestFnArray = std::vector<std::function<void(UnitTest& utest)>>;

template <typename T>
std::function<void(UnitTestFnArray& utests, T t)> UnitTestFnTemplate;


/** Test function and info. */
struct UnitTest 
{
    Str         name;                       // name of unit test
    UnitTestFn  test_fn;
    Str         fail_message        = "Test not run.";               // message printed if unit test fails
    bool        run_flag            = false;    // 
    bool        has_been_asserted   = false;
    bool        success_flag        = false;

    unsigned int indentation       = 12;
    unsigned int fail_msg_indent   = 16;


    UnitTest(const char* name , UnitTestFn unit_test_fn) 
        :   name            {name}, 
            test_fn   {unit_test_fn} 
    {
    };

    /** Set passed to true and return itself */
    void run()
    {
        if(run_flag)
            return;

        test_fn(*this);
        run_flag = true;
    }


    /** Set passed to true and return itself */
    void pass()
    {
        success_flag = true;
    }

    /** Set passed to false and return itself */
    void fail()
    {
        success_flag = false;
    }

    /** Set passed to false and return itself */
    void fail(Str new_fail_message)
    {
        fail_message = new_fail_message;
        success_flag = false;
    }

    /** Passes the unit test of true assert statement. */
    void assert(bool assert_statement, Str new_fail_message)
    {
        if(has_been_asserted)
            return;
        has_been_asserted = true;

        if(assert_statement)
            success_flag = true;
        else
            fail(new_fail_message);
        
    }

    /** Passes the unit test of true assert statement. */
    template<typename AssertType>
    void assert(AssertType actual, AssertType expected)
    {
        if(has_been_asserted)
            return;
        has_been_asserted = true;

        if(actual == expected) // ambigous warning for Err == Err
        {
            fail_message = "";
            success_flag = true;
        }
        else
        {
            // fail_message = {    "\n",
            //                     Pretty::Spaces(fail_msg_indent),
            //                     "Actual:   ", Pretty::StrSL(actual),
            //                     "\n",
            //                     Pretty::Spaces(fail_msg_indent),
            //                     "Expected: ", Pretty::StrSL(expected)
            //                 };


            Str str_indnt {indentation, ' '};

            fail_message = {    "\n",
                                str_indnt, "Actual:   \n", 
                                Pretty::StrM(fail_msg_indent, actual),
                                str_indnt, "Expected: \n", 
                                Pretty::StrM(fail_msg_indent, expected)
                            };
            success_flag = false;
        }
    }

    /** Passes the unit test of true assert statement. */
    void assert(bool assert_statement)
    {
        if(assert_statement)
            pass();
        else
            fail();
    }

    /** Check if test passed. */
    bool passed()
    {
        return run_flag && has_been_asserted && success_flag;
    }

    size_t test_count() { return 1; }
    size_t pass_count() { return passed() ? 1 : 0; }

    /** Print */
    void print_result()
    {
        print_indent(indentation);
        print_check(passed());
        
        Str message;

        if(!run_flag)
            message = "test never ran.";
        else if(!has_been_asserted)
            message = "test never asserted.";
        else if(!success_flag)
            message = fail_message;
        // else passed

        Print::line(name, ": ", message);
    }

};




