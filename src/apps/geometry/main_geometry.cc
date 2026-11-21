#include <cmath>

#include "lib/print.hh"
#include "lib/clock.hh"

#include "rend/sw/swrend.hh"


#include "window/wayland1/wayland.hh"
#include "window/wayland1/wl-defs.hh"
#include "window/wayland1/wl-callbacks.hh"

#include "geo_camera.hh"
#include "opengl.hh"
#include "geometry.hh"

Stopwatch stopwatch;



int main()
{
    Print::ln("Hello, from Geometry! \n");

    Clock clock;
    PerformanceClock performance_clock;

    Wayland wayland;
    wayland.init({  .width = 800, 
                    .height = 600, 
                    .rendering_type = WaylandRenderingType::OpenGL, 
                    // .rendering_type = WaylandRenderingType::SWR,
                    .window_title = "Main Geometry"                 });

    struct wl_callback *cb;

    SWR::Renderer swrenderer;
    Geometry geometry;

    if(wayland.surfaces.render_type == WaylandRenderingType::SWR)
    {
        swrenderer.set_buffer( wayland.get_content_buffer() );
        swrenderer.clear(0xFF555555);
    }
    else if(wayland.surfaces.render_type == WaylandRenderingType::OpenGL)
    {
        cb = wl_surface_frame(wayland.surfaces.opengl.wl_surface);
        wl_callback_add_listener(cb, &wayland.surfaces.opengl.callback_listener, &wayland);

        OpenGL::print_opengl_error();
        OpenGL::set_viewport_size(wayland.surfaces.opengl.egl.width, wayland.surfaces.opengl.egl.height);
        OpenGL::print_opengl_error();

        
        geometry.init();
        geometry.draw(clock.get_start_epoch_s());
        // cube.init();
        // cube.draw();
    }

    float current_time_s = 0.0f;

    while(wayland.frame_step())
    {
        if(wayland.set_size())
        {

            if(wayland.surfaces.render_type == WaylandRenderingType::SWR)
            {

            }
            if(wayland.surfaces.render_type == WaylandRenderingType::OpenGL)
            {
                OpenGL::set_viewport_size(wayland.surfaces.opengl.egl.width, wayland.surfaces.opengl.egl.height);
            }
        }

        // = geometry.time_s
        current_time_s  = clock.get_start_epoch_s();
        // Print::buf("current_s = ");
        // Print::ln(Str::FL(current_s, 3, Str::FloatRep::Fixed));

        // geometry.time_s += current_s;
        // geometry.set_time_s(current_s * 100.0f);
        
        // geometry.color += 0.001f;

        Arr<UserInput> user_inputs = wayland.get_new_input_events();
        for(uint i = 0; i < user_inputs.count(); i++)
        {
            UserInput user_input = user_inputs[i];
            if(user_input.is_scroll_input())
            {

                // opengl_triangle.color += 0.1f;
                // opengl_triangle.view_matrix.value.print();
                if(user_input.event_data.scroll_input.scroll_direction == ScrollDirection::Up)
                {
                    geometry.camera.move(f3::X() * 0.1f);
                    // opengl_triangle.view_matrix.value * 0.9f;
                }
                if(user_input.event_data.scroll_input.scroll_direction == ScrollDirection::Down)
                {
                    geometry.camera.move(f3::X() * -0.1f);
                    // opengl_triangle.view_matrix.value * 1.1f;
                }

                // opengl_triangle.view_matrix.value.w.w = 1.0f;
                // Print::ln("Scroll");
                // opengl_triangle.view_matrix.value.print();
            }

        }
        user_inputs.clear();

        if(wayland.surfaces.render_type == WaylandRenderingType::SWR)
        {
            swrenderer.set_buffer( wayland.get_content_buffer() );
            swrenderer.clear(0xFF654321);
        }
        else if(wayland.surfaces.render_type == WaylandRenderingType::OpenGL)
        {
            geometry.set_time_s(current_time_s);
            geometry.draw(clock.get_start_epoch_s());
            wayland.surfaces.opengl.egl.swap_buffers();
        }

        usleep(10000);

        // prev_time = clock.get_unix_epoch_ms();
        // clock.print_current_m_sec();
        // performance_clock.fps_step(true);
        // Print::ln("");
    }

    return 0;
}
