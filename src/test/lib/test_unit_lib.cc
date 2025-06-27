
#include <cstring>

#include "test/test.hh"

#include "test/lib/tlib_lib.hh"

#include "test/lib/tmod_str.hh"




int main()
{
    println("\nStarting Lib unit tests.\n");

    library_lib.run();

    println("\nEnd Lib unit tests.\n");

    return 0;
}

