
#pragma once



#include <unistd.h> // Segfault handler

#include <csignal>
#include <cstdlib>

#include <iostream>
#include <new>
#include <string>
#include <vector>
#include <functional>

#include "lib/str.hh"
#include "lib/print.hh"




#define trycatch(_x_) println("trycatch_x_");

#define try_    try{
#define catch_  }catch(const std::exception& e){
#define end_    }


#ifdef PH_LINUX
/** Linux */
void segfault_handler(int signal) {
    const char msg[] = "Segmentation fault!.\n";
    write(STDERR_FILENO, msg, sizeof(msg) - 1); // write() for async-signal-safety
    _exit(1);
}
#endif

void print_check(bool passed)
{
    if(passed)
        std::cout << "✅  ";
    else
        std::cout << "❌  ";
}

void print_indent(int indent_count){
    for(auto i = indent_count; i >= 0; i--)
    {
        std::cout << " ";
    }
}


/** Unit test information returned by completed unit test. */
class UnitTestInfo {

    std::string name; // name of unit test
    bool pass_flag = false;
    int print_indentation = 8;

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
        std::cout << name << std::endl;
    }

};
typedef std::function<UnitTestInfo()> UnitTestFn;



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





