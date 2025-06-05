
// Segfault handler
#include <iostream>
#include <csignal>
#include <cstdlib>
#include <unistd.h>

#include <string>
#include <vector>
#include <functional>

#include "str.hh"
#include "print.hh"


// FUNCTIONAL PLAYGROUND
std::function<bool()> lol = []() { println("HAHAHA"); return true; };
std::vector<std::function<int()>> fns = {

    []() -> bool { 
        println("f1");
        return true;
    },

    []() -> bool { 
        println("f2");
        return false;
    },

    lol
};


#ifdef PH_LINUX
/** Linux */
void segfault_handler(int signal) {
    const char msg[] = "Segmentation fault!.\n";
    write(STDERR_FILENO, msg, sizeof(msg) - 1); // write() for async-signal-safety
    _exit(1);
}
#endif

struct TestInfo {
    std::string description;
    bool passed = false;

    TestInfo(std::string description, bool passed) : description {description}, passed {passed} {};
};

#define trycatch(_x_) println("trycatch_x_");

#define try_    try{
#define catch_  }catch(const std::exception& e){
#define end_    }

std::string test_name;

std::vector<std::function<TestInfo()>> test_functions = {


    []() -> TestInfo {
        test_name = "Default constructor.";
        Str str;
        return TestInfo(test_name, true);
    },

    []() -> TestInfo { 
        Str str {};
        return TestInfo("Default constructor initializer list.", true);
    },

    []() -> TestInfo { 
        Str str = Str();
        return TestInfo("Default constructor parens with assignment.", true);
    },

    []() -> TestInfo {
        Str str = Str{};
        return TestInfo("Default constructor list with assignment.", true);
    },

    []() -> TestInfo {
        Str str = "";
        return TestInfo("Constructor: empty c string.", true);
    },

    []() -> TestInfo {
        test_name = "Constructor: integer 123456.";
        Str str = 123456;

        if( str.size() == 6 && str[0] == '1' &&  str[1] == '2' &&  str[2] == '3' &&
            str[3] == '4' &&  str[4] == '5' &&  str[5] == '6' )
            return TestInfo(test_name, true);
        else
            return TestInfo(test_name, false);
    },


};



int main(){


    // Signal handler
    signal(SIGSEGV, segfault_handler);


    // lol();
    // for(std::function<void()> f : fns)
    //     f();

    trycatch(b)

    println("\nStarting Str tests.\n");


    struct TestContext {
        size_t total_test_count = test_functions.size();

        size_t tests_completed = 0;
        size_t pass_count = 0;

        TestInfo previous_test_info {"initial test info", false};

    } context;
    

    try_

        for(std::function<TestInfo()> function : test_functions){

            TestInfo test_info = function();
            
            ++context.tests_completed;

            if(test_info.passed)
                ++context.pass_count;


            context.previous_test_info = test_info;

            // if(test_info.passed)
            //     std::cout << test_info.description << ": PASS" << std::endl;
            // else
            //     std::cout << test_info.description << ": FAIL" << std::endl;
                
        }

        std::cout << "Test Str : passed " << context.pass_count << "/" << context.total_test_count << "\n";

    catch_

        std::cout << "Uncatch error in test_str.cc" << std::endl;
        std::cout << "Last successful test description: " << context.previous_test_info.description << std::endl;
        std::cout << "Tests Str : completed " << context.tests_completed << "/" << context.total_test_count << "\n\n";
        std::cout << "Test Str : passed " << context.pass_count << "/" << context.total_test_count << "\n";

    end_
    
    

    
    
    // print("Test Str: 0/0 \n");

    return 0;
}

