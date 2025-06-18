
#pragma once


#include "render/bitmap.hh"


#include "math/graph2D.hh"

class RendererMathGraph
{

public:

    static Bitmap render_graph2D_bitmap(Graph2D<float>& graph);
    
};