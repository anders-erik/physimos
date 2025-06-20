
#include "renderer_graph.hh"

Bitmap RendererMathGraph::render_graph2D_bitmap(Graph2D<float>& graph)
{
    graph.update();

    R2<float> graph_view_size = graph.viewbox.size;

    unsigned int bitmap_scale = 1;

    ui2 bitmap_size = { bitmap_scale * (unsigned int) graph_view_size.x,
                        bitmap_scale * (unsigned int) graph_view_size.y};


    Bitmap bitmap {bitmap_size};

    BitmapPixel white = {255, 255, 255, 255};
    BitmapPixel black = {0  , 0  , 0  , 255};

    bitmap.clear(black);

    f2 viewbox_pos = {graph.viewbox.pos.x, graph.viewbox.pos.y};

    for(size_t i = 0; i < graph.point_count; i++)
    {
        
        f2 point_pos = {graph.x[i], graph.y[i]};
        
        f2 pixel_pos = point_pos - viewbox_pos;

        i2 pixel_pos_int = pixel_pos.to_i2();

        bitmap.set_pixel(   bitmap_scale * pixel_pos_int.x, 
                            bitmap_scale * pixel_pos_int.y, 
                            white       );
    }

    
    return bitmap;
}
