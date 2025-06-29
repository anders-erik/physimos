
#pragma once


#include "test/test.hh"

#include "math/graph2D.hh"



UnitTestArray graph2D_construct = {


    {   "no default",
    [](UnitTest& utest) -> void
    {
        utest.assert(true, true);
    }},


    {   "f=x^2, [-1, 1], N=3 : n=0 = (-1,1)",
    [](UnitTest& utest) -> void
    {
        Graph2D<float> graph {  {-1.0f, 1.0f},
                                3,
                                {{0, 0, 1}}       };

        utest.assert(   graph.at(0),
                        {-1.0, 1.0});
    }},
    {   "f=x^2, [-1, 1], N=3 : n=1 = (0,0)",
    [](UnitTest& utest) -> void
    {
        Graph2D<float> graph {  {-1.0f, 1.0f},
                                3,
                                {{0, 0, 1}}       };

        utest.assert(   graph.at(1),
                        {0.0, 0.0});
    }},
    {   "f=x^2, [-1, 1], N=3 : n=2 = (1,1)",
    [](UnitTest& utest) -> void
    {
        Graph2D<float> graph {  {-1.0f, 1.0f},
                                3,
                                {{0, 0, 1}}       };

        utest.assert(   graph.at(2),
                        {1.0, 1.0});
    }},

};






