
#include "lib/print.hh"
#include "lib/clock.hh"

#include "rend/sw/swrend.hh"
#include "window/wayland1/wayland.hh"

#include "window/wayland1/wl-defs.hh"

Stopwatch stopwatch;

void callback_done(  void *data,
		    struct wl_callback *wl_callback,
		    uint32_t callback_data);


const struct wl_callback_listener callback_listener = {
    .done = callback_done,
};


void callback_done(  void *data,
		    struct wl_callback *wl_callback,
		    uint32_t callback_data)
{
    // stopwatch.stop_and_print_ms();
    // Print::ln("Callback done!");

    Wayland* wayland = (Wayland*) data;

    wl_callback_destroy(wl_callback);
    wl_callback = wl_surface_frame(wayland->surfaces.opengl.wl_surface);
    wl_callback_add_listener(wl_callback, &callback_listener, wayland);

    stopwatch.start();
}



int main()
{
    Print::ln("Hello, from Geometry! \n");

    Clock clock;
    PerformanceClock performance_clock;
    uint64_t prev_time = clock.get_unix_epoch_ms();
    

    Wayland wayland;
    // wayland.init({  .width = 800, .height = 600, .rendering_type = WaylandRenderingType::Buffer});
    wayland.init({  .width = 800, .height = 600, .rendering_type = WaylandRenderingType::OpenGL});


    if(wayland.surfaces.render_type == WaylandRenderingType::OpenGL)
    {
        struct wl_callback *cb = wl_surface_frame(wayland.surfaces.opengl.wl_surface);
        wl_callback_add_listener(cb, &callback_listener, &wayland);
    }

    SWR::Renderer swrenderer;
    // if(wayland.surfaces.render_type == WaylandRenderingType::Buffer)
    // {
    //     swrenderer.set_buffer( wayland.get_content_buffer() );
    //     swrenderer.clear(0xFF555555);
    // }


    

    
    // wayland.surfaces.decorations.surface.commit();
    // wayland.surfaces.decorations.surface.commit();
    // wl_surface_commit(wayland.surfaces.decorations.surface.surface);

    while(wayland.frame_step())
    {
        if(wayland.set_size())
        {
        }
        Arr<UserInput> events = wayland.get_new_input_events();

        for(uint i = 0; i < events.count(); i++)
        {
            events.clear();
        }

        if(wayland.surfaces.render_type == WaylandRenderingType::SWR)
        {
            swrenderer.set_buffer( wayland.get_content_buffer() );
            swrenderer.clear(0xFF654321);
        }
        else if(wayland.surfaces.render_type == WaylandRenderingType::OpenGL)
        {
            wayland.surfaces.opengl.egl.draw();
        }

        usleep(10000);

        // prev_time = clock.get_unix_epoch_ms();
        // clock.print_current_m_sec();

        // performance_clock.fps_step(true);

        // Print::ln("");
    }

    return 0;
}