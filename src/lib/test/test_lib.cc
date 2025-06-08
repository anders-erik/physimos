
#include "test/test.hh"

#include "test/test_str.hh"
#include "test/test_file.hh"



int main()
{
    println("\nStarting Lib tests.\n");



    TestModule str_module {"Str"};
    str_module.add_collection(UnitTestCollection{"constructors", constructors});
    str_module.add_collection(UnitTestCollection{"equality", equality});
    str_module.add_collection(UnitTestCollection{"concat", concat});
    str_module.add_collection(UnitTestCollection{"substr", substr});
    str_module.run();


    TestModule file_module {"File"};
    file_module.add_collection({"cat_str", cat_str});
    file_module.run();


    println("\nEnd Lib tests.\n");
    return 0;
}

