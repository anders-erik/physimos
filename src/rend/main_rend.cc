
#include <iostream>

#include "math/graph2D.hh"

#include "rend/bitmap.hh"
#include "io/bmp/bmp.hh"

#include "rend/math/renderer_graph.hh"



int main ()
{
    std::cout << "Main Render" << std::endl << std::endl;
    
    R2<float> domain = {-2.0f, 2.0f};

    Polynomial<float> polynomial {{0, 0, 1}};

    Graph2D<float> graph {domain, 5, polynomial};

    Bitmap bitmap = RendererMathGraph::render_graph2D_bitmap(graph);

    BitmapPixel white = {255, 255, 255, 255};

    // bitmap.clear(white);

    if(bitmap.has_a_non_black_white_pixel())
        std::cout << "Non black white pixel found!" << std::endl;

    BMP::File bmp_loader {bitmap};

    bmp_loader.write("graph.bmp");

    return 0;
}