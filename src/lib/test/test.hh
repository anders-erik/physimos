
#pragma once


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


/** Unit test information returned by completed test. */
struct InfoUnitTest {

    std::string description;
    bool pass_flag = false;

    /** Set passed to true and return itself */
    InfoUnitTest& pass(){
        pass_flag = true;
        return *this;
    }
    /** Set passed to false and return itself */
    InfoUnitTest& fail(){
        pass_flag = false;
        return *this;
    }
    bool is_passed(){ return pass_flag ? true : false;}

    
    InfoUnitTest(const char* description) : description {std::string(description)} {};
    InfoUnitTest(const char* description, bool passed) : description {std::string(description)}, pass_flag {passed} {};
    InfoUnitTest(std::string description) : description {description}, pass_flag {false} {};
    InfoUnitTest(std::string description, bool passed) : description {description}, pass_flag {passed} {};


    void print(){

        if(pass_flag)
            std::cout << "    " << description << ": PASS" << std::endl;
        else
            std::cout << "    " << description << ": FAIL" << std::endl;
    }
};


/** Information about a module (collection of unit tests) test. */
struct InfoUnitTestCollection {

    std::string test_name;

    size_t total_test_count = 0;

    size_t tests_completed = 0;
    size_t pass_count = 0;

    InfoUnitTest previous_test_info = {"initial test info", false};

    InfoUnitTestCollection(    std::string test_name,
                    std::vector<std::function<InfoUnitTest()>>& unit_test_collection
                )
        :   test_name {test_name},
            total_test_count {unit_test_collection.size()} 
    {
    };

    bool all_passed(){
        return pass_count == total_test_count ? true : false;
    }

    void print_check(){
        if(all_passed())
            std::cout << "✅  ";
        else
            std::cout << "❌  ";
    }

    void print_pass(){
        print_check();
        std::cout << test_name << ": passed " << pass_count << "/" << total_test_count << "\n";
    }
    void print_unexpected_throw(){
        print_check();
        std::cout << "Uncatch error in test_str.cc" << std::endl;
        std::cout << "Last successful unit test: " << previous_test_info.description << std::endl;
        std::cout << test_name << ": completed " << tests_completed << "/" << total_test_count << "\n\n";
        std::cout << test_name << ": passed " << pass_count << "/" << tests_completed << "\n";
    }

};

typedef std::function<InfoUnitTest()> UnitTest;
typedef std::vector<UnitTest> UnitTestCollection;



void unit_tests_run(std::string test_name, UnitTestCollection& unit_test_collection){


    InfoUnitTestCollection info_collection {test_name, unit_test_collection};
    
    bool verbose = false;


    try_

        for(std::function<InfoUnitTest()> function : unit_test_collection){

            InfoUnitTest test_info = function();
            if(verbose)
                test_info.print();
            
            if(test_info.is_passed())
                ++info_collection.pass_count;

            ++info_collection.tests_completed;

            // Cache previous test for debugging if next test throws
            info_collection.previous_test_info = test_info;
        }

        info_collection.print_pass();

    catch_

        info_collection.print_unexpected_throw();

    end_

}



