
#pragma once

#include <string>
#include <vector>

#include "test/misc.hh"
#include "test/unit_test.hh"



/** Collection of related unit tests. Usually all tests associated with a particular method of a class. */
class UnitTestCollection
{
    std::string name;   // Name of collection
    std::vector<UnitTestFn> unit_tests; // Tests to run
    std::vector<UnitTestInfo> infos; // returned unit test info objects
    int print_indentation = 4;

public:
    

    UnitTestCollection(std::string collection_name, std::vector<UnitTestFn> unit_tests)
        :   name {collection_name},
            unit_tests {unit_tests} 
    {
    };
    

    std::string get_name()
    {
        return name;
    }

    bool all_passed()
    {
        for(auto& test_info : infos)
        {
            if(!test_info.is_passed())
                return false;
        }
        return true;
    }

    size_t pass_count()
    {
        size_t count = 0;
        for(auto& test_info : infos)
        {
            if(test_info.is_passed())
                ++count;
        }
        return count;
    }
    size_t tests_completed()
    {
        return infos.size();
    }
    size_t total_test_count()
    {
        return unit_tests.size();
    }

    /** Print only check mark and pass count. No list of unit tests. */
    void print_passed()
    {
        print_indent(print_indentation);
        print_check(all_passed());
        std::cout << "Collection: " << name  << " - " << pass_count() << "/" << total_test_count() << std::endl;
    }

    /** Print check mark and pass count, as well as a list of unit tests ran. */
    void print_failed_test_infos()
    {
        print_indent(print_indentation);
        print_check(all_passed());
        std::cout << "Collection: " << name  << " - " << pass_count() << "/" << total_test_count() << std::endl;
        
        for(auto& test_info : infos)
        {
            test_info.print();
        }
    }

    void run_collection()
    {
        for(auto& unit_test : unit_tests)
        {
            UnitTestInfo test_info = unit_test();
            infos.push_back(test_info);
        }
    }


};
