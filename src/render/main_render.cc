
#include <iostream>

#include "math/graph2D.hh"

#include "render/bitmap.hh"
#include "file_format/bmp/bmp.hh"

#include "render/math/renderer_graph.hh"



int main ()
{
    std::cout << "Main Render" << std::endl << std::endl;
    
    F2<float> view_pos = {-2.0f, -2.0f};
    F2<float> view_size = {4.0f, 4.0f};

    AABB<float> viewbox = {view_pos, view_size};

    Polynomial<float> polynomial {{0, 0, 1}};

    Graph2D<float> graph {5, viewbox, polynomial};

    Bitmap bitmap = RendererMathGraph::render_graph2D_bitmap(graph);

    BitmapPixel white = {255, 255, 255, 255};

    // bitmap.clear(white);

    if(bitmap.has_a_non_black_white_pixel())
        std::cout << "Non black white pixel found!" << std::endl;

    BMP::File bmp_loader {bitmap};

    bmp_loader.write("graph.bmp");

    return 0;
}