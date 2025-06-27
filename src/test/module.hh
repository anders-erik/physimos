#pragma once

#include "test/unit_test_collection.hh"


/** 
    A collection of Unit test collections.
    Usually represents all tests for a specific class or struct. 
*/
class TestModule
{
    std::vector<UnitTestCollection> unit_test_collections;
    std::string name;

public:

    TestModule() = default;
    TestModule(std::string name)
        :   name {name} 
    {
    };

    void add_collection(const UnitTestCollection& collection)
    {
        unit_test_collections.push_back(collection);
    }

    size_t pass_count()
    {
        size_t count = 0;
        for(auto& collection : unit_test_collections)
        {
            if(collection.all_passed())
                ++count;
        }
        return count;
    }
    size_t total_test_count()
    {
        return unit_test_collections.size();
    }

    bool all_passed()
    {
        for(auto& collection : unit_test_collections)
        {
            if(!collection.all_passed())
            {
                return false;
            }
        }    
        return true;
    }

    void print_result() 
    {
        print_check(all_passed());
        std::cout << name << ": " << pass_count() << "/" << total_test_count() << "\n";
    }

    void print_failed()
    {
        for(auto& collection : unit_test_collections)
        {
            if(!collection.all_passed())
            {
                collection.print_failed_test_infos();
            }
            else
            {
                collection.print_passed();
            }
        }    
    }

    void print_unexpected_throw(UnitTestCollection& collection)
    {
        std::cout << "Uncatch error:" << std::endl;
        std::cout << "        module    : " << name << std::endl;
        std::cout << "        collection: " << collection.get_name() << std::endl;
    }

    void run()
    {
        // Signal handler for non-thrown errors
        signal(SIGSEGV, segfault_handler);

        
        for(auto& unit_test_collection : unit_test_collections)
        {
            try_
                unit_test_collection.run_collection();
            catch_
                print_unexpected_throw(unit_test_collection);
            end_
        }

        print_result();
        if(!all_passed())
        {
            print_failed();
        }
    }

};
