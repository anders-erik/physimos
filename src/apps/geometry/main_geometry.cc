
#include "lib/print.hh"
#include "rend/sw/swrend.hh"
#include "window/wayland1/wayland.hh"

int main()
{
    Print::ln("Hello, from Geometry! \n");

    Wayland wayland;
    wayland.init({  .width = 800, .height = 600, .rendering_type = WaylandRenderingType::Buffer});
    // wayland.init({  .width = 800, .height = 600, .rendering_type = WaylandRenderingType::OpenGL});

    SWR::Renderer swrenderer;
    // if(wayland.surfaces.render_type == WaylandRenderingType::Buffer)
    // {
    //     swrenderer.set_buffer( wayland.get_content_buffer() );
    //     swrenderer.clear(0xFF555555);
    // }
    

    while(wayland.frame_step())
    {
        if(wayland.set_size())
        {
        }
        Arr<UserInput> events = wayland.get_new_input_events();

        for(uint i = 0; i < events.count(); i++)
        {
        }

        if(wayland.surfaces.render_type == WaylandRenderingType::Buffer)
        {
            swrenderer.set_buffer( wayland.get_content_buffer() );
            swrenderer.clear(0xFF654321);
        }

        usleep(10000);
    }

    return 0;
}