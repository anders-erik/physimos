
#include <cstdlib>
#include <string>

#include "test/test.hh"

#include "test/lib/tlib_lib.hh"
#include "test/math/tlib_math.hh"
#include "test/io/json/tlib_json.hh"
#include "test/io/gltf/tlib_gltf.hh"
#include "test/io/bmp/tlib_bmp.hh"


int main()
{
    println("\nStarting all unit tests.\n");

    std::string explct = std::string{100, '5'};

    std::vector<TCLibrary> tclib_tests =
    {
        tclib_lib,
        tclib_math,
        tclib_json,
        tclib_gltf,
        tclib_bmp,
    };


    for(auto& tclib : tclib_tests)
    {
        tclib.run();
        tclib.print_result();
    }



    println("\nDone all unit tests.\n");

    return 0;
}