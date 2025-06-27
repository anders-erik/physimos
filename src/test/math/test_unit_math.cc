
#include "test/test.hh"

#include "test/math/tlib_math.hh"


int main()
{
    println("\nStarting Math tests.\n");

    library_math.run();

    // TestModule core_module {"Core"};
    //     core_module.add_collection(UnitTestCollection{"pow_fns", pow_fns});
    // core_module.run();

    // TestModule vec_module {"Vec"};
    //     vec_module.add_collection(UnitTestCollection{"construct", construct_vec});
    //     vec_module.add_collection(UnitTestCollection{"arithmetic", arithmetic_vec});
    // vec_module.run();

    // TestModule polynomial_module {"Polynomial"};
    //     polynomial_module.add_collection(UnitTestCollection{"construct", construct_poly});
    //     polynomial_module.add_collection(UnitTestCollection{"evaluate", evaluate});
    // polynomial_module.run();

    // TestModule graph2D_module {"Graph2D"};
    //     graph2D_module.add_collection(UnitTestCollection{"construct", graph2D_construct});
    // graph2D_module.run();


    println("\nEnd Math tests.\n");
    return 0;
}

