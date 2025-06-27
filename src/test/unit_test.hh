
#pragma once

#include <functional>
#include <vector>

#include "lib/str.hh"


/** Unit test information returned by completed unit test. */
class UnitTestInfo {

    std::string     name;                           // name of unit test
    std::string     fail_message;                   // message printed if unit test fails
    bool            pass_flag           = false;
    int             print_indentation   = 8;

public:
    
    UnitTestInfo(const char* name) : name {std::string(name)} {};
    UnitTestInfo(const char* name, bool passed) : name {std::string(name)}, pass_flag {passed} {};
    UnitTestInfo(std::string name) : name {name}, pass_flag {false} {};
    UnitTestInfo(std::string name, bool passed) : name {name}, pass_flag {passed} {};


    /** Set passed to true and return itself */
    UnitTestInfo& pass()
    {
        pass_flag = true;
        return *this;
    }

    /** Set passed to false and return itself */
    UnitTestInfo& fail()
    {
        pass_flag = false;
        return *this;
    }

    /** Set passed to false and return itself */
    UnitTestInfo& fail(std::string new_fail_message)
    {
        fail_message = new_fail_message;
        pass_flag = false;
        return *this;
    }

    /** Passes the unit test of true assert statement. */
    UnitTestInfo& assert(bool assert_statement)
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
typedef std::function<UnitTestInfo()> UnitTestFn;



