
#include <cstdlib>

#include "test/test.hh"

#include "test/lib/tlib_lib.hh"
#include "test/math/tlib_math.hh"
#include "test/io/json/tlib_json.hh"


int main()
{
    println("\nStarting all unit tests.\n");

    std::vector<TCLibrary> tclib_tests =
    {
        tclib_lib,
        tclib_math,
        tclib_json,
    };


    for(auto& tclib : tclib_tests)
    {
        tclib.run();
        tclib.print_result();
    }


    println("\nDone all unit tests.\n");

    return 0;
}