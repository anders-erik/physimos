
#pragma once

#include <functional>
#include <vector>
#include <type_traits> 
#include <cassert>

#include "lib/str.hh"
#include "lib/print.hh"

#include "io/pretty_str.hh"


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
    void assert_old_msg(bool assert_statement, Str new_fail_message)
    {
        if(has_been_asserted)
            return;
        has_been_asserted = true;

        if(assert_statement)
            success_flag = true;
        else
            fail(new_fail_message);
        
    }

    // Assert fundamental C++ types
    template<typename AssertBuiltInType>
    void assert_fundamental(  const AssertBuiltInType& A, 
                                const AssertBuiltInType& E
                             )
    {
        static_assert(  std::is_integral_v<AssertBuiltInType> ||
                        std::is_floating_point_v<AssertBuiltInType>,
                        "AssertIntegralType must be an integral type."  );

        if(has_been_asserted)
            return;

        has_been_asserted = true;

        if(A == E) // ambigous warning for Err == Err
        {
            fail_message = "";
            success_flag = true;
        }
        else
        {
            Str str_indnt {indentation, ' '};

            Str actual_info     = Str("\nActual: \n").indent_space(indentation);
            Str expected_info   = Str("Expected: \n").indent_space(indentation);
            
            Str actual_str      = Str::to_str(A);
            Str expected_str    = Str::to_str(E);
            
            actual_str = actual_str.indent_space(indentation);
            expected_str = expected_str.indent_space(indentation);

            fail_message = {
                                actual_info, 
                                actual_str,
                                expected_info, 
                                expected_str
                            };
            success_flag = false;
        }

    }

    /** Assert custom user defined type.
        Type must provide a `to_str()` method.  */
    template<typename AssertType>
    void assert_custom(AssertType actual, AssertType expected)
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

            Str actual_info   = Str("\nActual: \n").indent_space(indentation);
            Str actual_str;
            Str expected_info = Str("Expected: \n").indent_space(indentation);
            Str expected_str;

            // check if type int
            if(std::is_same_v<AssertType, bool>)
            {
                // actual_str   = StrUtil::to_str(actual);
                // expected_str =  StrUtil::to_str(expected);
            }
            else if(std::is_same_v<AssertType, Str>)
            {
                actual_str.to_str();
                expected_str.to_str();
            }
            else
            {
                actual_str   = Pretty::StrM(fail_msg_indent, actual);
                expected_str = Pretty::StrM(fail_msg_indent, expected);
            }

            fail_message = {
                                actual_info, 
                                actual_str,
                                expected_info, 
                                expected_str
                            };
            success_flag = false;
        }
    }

    /** Passes the unit test of true assert statement. */
    void assert_old(bool assert_statement)
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




