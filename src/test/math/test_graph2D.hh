
#pragma once


#include "test/test.hh"

#include "math/graph2D.hh"




std::vector<UnitTestFn> graph2D_construct = {


    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "no default";

        // Graph2D<float> graph;
        
        return test_info.pass();
    },


    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "float, 3 points, f(x) = x^2, NDC";

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
            return test_info.fail();
        if(first_point.y != 1.0f)
            return test_info.fail();
        if(second_point.x != 0.0f)
            return test_info.fail();
        if(second_point.y != 0.0f)
            return test_info.fail();
        if(third_point.x != 1.0f)
            return test_info.fail();
        if(third_point.y != 1.0f)
            return test_info.fail();
        
        return test_info.pass();
    },

};






