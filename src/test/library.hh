#pragma once

#include "test/module.hh"


/** 
    A collection of Unit test collections.
    Usually represents all tests for a specific class or struct. 
*/
class TestLibrary
{
    std::string name;
    std::vector<TestModule> modules;
    int print_indentation = 0;

public:

    TestLibrary() = default;
    TestLibrary(std::string name)
        :   name {name} 
    {
    };
    TestLibrary(std::string name, std::vector<TestModule> modules)
        :   name    {name},
            modules {modules}
    {
    };

    void add_module(const TestModule& module)
    {
        modules.push_back(module);
    }

    size_t pass_count()
    {
        size_t count = 0;
        for(auto& module : modules)
        {
            if(module.all_passed())
                ++count;
        }
        return count;
    }
    size_t total_test_count()
    {
        return modules.size();
    }

    bool all_passed()
    {
        for(auto& module : modules)
        {
            if(!module.all_passed())
            {
                return false;
            }
        }    
        return true;
    }

    void print_result() 
    {
        print_indent(print_indentation);
        print_check(all_passed());
        std::cout << name << ": " << pass_count() << "/" << total_test_count() << "\n";

        if(!all_passed())
            print_failed();
    }

    void print_failed()
    {
        for(auto& module : modules)
        {
            module.print_result();
        }    
    }

    void print_unexpected_throw(TestModule& module)
    {
        std::cout << "Uncatch error:" << std::endl;
        std::cout << "        library    : " << name << std::endl;
        std::cout << "        module : " << module.get_name() << std::endl;
    }

    void run()
    {
        // Signal handler for non-thrown errors
        signal(SIGSEGV, segfault_handler);

        
        for(auto& module : modules)
        {
            try_
                module.run();
            catch_
                print_unexpected_throw(module);
            end_
        }

        print_result();
    }

};
