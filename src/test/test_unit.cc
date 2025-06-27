
#include <cstdlib>

#include "test/lib/tlib_lib.hh"
#include "test/math/tlib_math.hh"


int main()
{
    println("\nStarting all unit tests.\n");

    library_lib.run();
    library_math.run();

    println("\nDone all unit tests.\n");

    return 0;
}