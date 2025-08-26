#pragma once

#include "test/test.hh"

#include "test/math/tmod_core.hh"
#include "test/math/tmod_vec.hh"
#include "test/math/tmod_graph2D.hh"
#include "test/math/tmod_polynomial.hh"
#include "test/math/tmod_orient.hh"




TCModule mod_math_core = {
    "Core", {
        {"pow_fns", pow_fns},
    }
};

TCModule mod_math_vec = {
    "Vec", {
        {"construct", construct_vec},
        {"arithmetic", arithmetic_vec},
    }
};


TCModule mod_math_polynomial = {
    "Polynomial", {
        {"construct", construct_poly},
        {"evaluate", evaluate},
    }
};

TCModule mod_math_graph2D = {
    "Graph2D", {
        {"construct", graph2D_construct},
    }
};

TCModule mod_math_orient = {
    "Orient2d", {
        {"Orient2d", orient_2D},
    }
};


TCLibrary tclib_math = {
    "math",
    {   
        mod_math_core,
        mod_math_vec,
        mod_math_polynomial,
        mod_math_graph2D,
        mod_math_orient,
    }
};

