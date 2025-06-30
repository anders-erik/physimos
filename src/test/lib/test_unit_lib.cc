
#include <cstring>

#include "test/test.hh"

#include "test/lib/tlib_lib.hh"

#include "test/lib/tmod_str.hh"




int main()
{
    println("\nStarting Lib unit tests.\n");

    tclib_lib.run();
    tclib_lib.print_result();

    println("\nEnd Lib unit tests.\n");

    return 0;
}

