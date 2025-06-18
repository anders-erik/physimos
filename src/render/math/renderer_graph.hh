
#pragma once


#include "image/bitmap.hh"


#include "math/graph2D.hh"

class RendererMathGraph
{

public:

    static pimage::Bitmap render_graph2D_bitmap(Graph2D<float>& graph);
    
};