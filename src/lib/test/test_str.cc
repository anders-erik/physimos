
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




struct TestInfo {
    std::string description;
    bool passed = false;

    // TestInfo() = default;
    // TestInfo(std::string description) : description {description} {};
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
        try_
            Str str;
            return TestInfo(test_name, true);
        catch_
            return TestInfo(test_name, false);
        end_
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


};



int main(){


    // lol();
    // for(std::function<void()> f : fns)
    //     f();

    trycatch(b)

    println("\nStarting Str tests.\n");

    size_t test_count = test_functions.size();
    size_t pass_count = 0;

    for(std::function<TestInfo()> function : test_functions){

        TestInfo info = function();

        if(info.passed)
            ++pass_count;


        // if(info.passed)
        //     std::cout << info.description << ": PASS" << std::endl;
        // else
        //     std::cout << info.description << ": FAIL" << std::endl;
            
    }

    std::cout << "Test Str done: passed " << pass_count << "/" << test_count << "\n\n";
    
    // print("Test Str: 0/0 \n");

    return 0;
}

