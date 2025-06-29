#pragma once

#include <vector>

#include "lib/str.hh"
#include "lib/print.hh"

#include "test/misc.hh"


template <typename T, unsigned char Indent>
struct TCollection
{
    Str             name;
    std::vector<T>  tests;
    unsigned char   indentation = Indent;

    TCollection(Str collection_name, std::vector<T> tests)
        :   name    {collection_name}
        ,   tests   {tests}
    {
    };

    bool passed()
    {
        for(auto& test : tests)
        {
            if(!test.passed())
                return false;
        }
        return true;
    }

    size_t pass_count()
    {
        size_t count = 0;
        for(auto& test : tests)
        {
            count += test.pass_count();
        }
        return count;
    }

    size_t test_count()
    {
        size_t counter = 0;
        for(auto& test : tests)
            counter += test.test_count();
        return counter;
    }


    void print_result()
    {
        print_indent(indentation);
        print_check(passed());
        Print::line(    name, 
                        ": ", 
                        Str::UI(pass_count()), 
                        "/", 
                        Str::UI(test_count()) );

        if(!passed())
            print_failed();
    }

    void print_failed()
    {
        for(auto& test : tests)
        {
            test.print_result();
        }
    }

    void print_unexpected_throw(T& test)
    {
        Print::line("Uncatch error in collection : ", name);
        Print::line("Uncatch error in test       : ", test.name);
    }

    void run()
    {
        for(auto& test : tests)
        {
            try_
                test.run();
            catch_
                print_unexpected_throw(test);
            end_
        }
    }

};