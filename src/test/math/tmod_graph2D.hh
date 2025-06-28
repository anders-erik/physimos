
#pragma once


#include "test/test.hh"

#include "math/graph2D.hh"



UnitTestArray graph2D_construct = {


    {   "no default",
    [](UnitTest& utest) -> void
    {
        // Graph2D<float> graph;
        
        utest.pass();
    }},


    {   "float, 3 points, f(x) = x^2, NDC",
    [](UnitTest& utest) -> void
    {
        R2<float> view_pos = {-1.0f, -1.0f};
        R2<float> view_size = {2.0f, 2.0f};
        
        R2<float> viewbox = {-1.0f, 1.0f};

        Polynomial<float> polynomial {{0, 0, 1}};

        Graph2D<float> graph {3, viewbox, polynomial};

        graph.update();

        R2<float> first_point = graph.at(0);
        R2<float> second_point = graph.at(1);
        R2<float> third_point = graph.at(2);

        if(first_point.x != -1.0f)
            utest.fail();
        if(first_point.y != 1.0f)
            utest.fail();
        if(second_point.x != 0.0f)
            utest.fail();
        if(second_point.y != 0.0f)
            utest.fail();
        if(third_point.x != 1.0f)
            utest.fail();
        if(third_point.y != 1.0f)
            utest.fail();
        
        utest.pass();
    }},

};






