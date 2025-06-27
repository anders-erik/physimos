#pragma once

#include "test/test.hh"

#include "test/math/tmod_core.hh"
#include "test/math/tmod_vec.hh"
#include "test/math/tmod_graph2D.hh"
#include "test/math/tmod_polynomial.hh"




TestModule mod_math_core = {
    "Core", {
        {"pow_fns", pow_fns},
    }
};

TestModule mod_math_vec = {
    "Vec", {
        {"construct", construct_vec},
        {"arithmetic", arithmetic_vec},
    }
};


TestModule mod_math_polynomial = {
    "Polynomial", {
        {"construct", construct_poly},
        {"evaluate", evaluate},
    }
};

TestModule mod_math_graph2D = {
    "Graph2D", {
        {"construct", graph2D_construct},
    }
};


TestLibrary library_math = {
    "math",
    {   
        mod_math_core,
        mod_math_vec,
        mod_math_polynomial,
        mod_math_graph2D,
    }
};

