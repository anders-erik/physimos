
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> // socket
#include <sys/un.h> // sockaddr_un


#include "lib/print.hh"


#include "wayland.hh"



#include <wayland-client.h>
// #include "swrend/wayland.hh"

// #include "wl-book.hh"
#include "wl-defs.hh"
#include "wl-objects.hh"
#include "wl-surfaces.hh"
// #include "wl-pointer.hh"
// #include "wl-keyboard.hh"
// #include "wl-registry.hh"



Wayland::Wayland()
{
    init(WAYLAND_DEFAULT_CONTENT_SIZE);
}
Wayland::Wayland(i2 dims)
{
    init(dims);
}
Wayland::~Wayland()
{
    destroy();
}




void Wayland::connect_compositor()
{
    // opaque object passed to qayland server during communication
    // struct wl_display *display = wl_display_connect(NULL);
    core.display = wl_display_connect(NULL);
    if (!core.display)
    {
        Print::ln("Failed to connect to Wayland display\n");
        return;
    }
    // This generates a runtime message that the 'proxy <address> already has listener'
    wl_display_add_listener(core.display, &core.display_listener, this);

    // think: wl_display.get_registry(); !!
    core.registry = wl_display_get_registry(core.display);
    if (!core.registry)
    {
        Print::ln("Failed to get Wayland registry.\n");
        return;
    }

    wl_registry_add_listener(core.registry, &core.registry_listener, this);

    // Process all pending requests (and will block until completed)
    int ret = wl_display_roundtrip(core.display);
    if (ret < 0)
    {
        printf("Wl_display_roundtrip failure. Display error: %d\n", wl_display_get_error(core.display));
        return;
    }

    if (!core.compositor || !core.subcompositor)
    {
        fprintf(stderr, "Required interfaces (compositor/subcompositor) missing.\n");
        return;
    }

    xdg_wm_base_add_listener(xdg.wm_base, &xdg.xdg_wm_base_listener, this);
    wl_seat_add_listener(input.seat, &input.seat_listener, this);

    // printf("display fd = %d\n", wl_display_get_fd(state.wl_display));
    // printf("error = %d\n", wl_display_get_error(state.wl_display));
}



    



void Wayland::init(i2 content_dims)
{
    connect_compositor();

    surfaces.init(content_dims, this);
    
    xdg.surface = xdg_wm_base_get_xdg_surface(xdg.wm_base, surfaces.decorations.surface.surface);
    xdg_surface_add_listener(xdg.surface, &xdg.xdg_surface_listener, this);

    xdg.toplevel = xdg_surface_get_toplevel(xdg.surface);
    xdg_toplevel_add_listener(xdg.toplevel, &xdg.xdg_toplevel_listener, this);
    xdg_toplevel_set_title(xdg.toplevel, "Example client");


    // cursor.create_default(core.compositor, surfaces.window.fb.wl_shm);
    cursor.init(core.compositor, surfaces.pool.wl_shm);

    surfaces.commit();
    // wl_display_dispatch(core.display); // caused a flash before first proper render
}


Arr<UserInput> Wayland::get_new_input_events()
{
    Arr<UserInput> events = input.content_events;
    input.content_events.clear();
    return events;
}

bool Wayland::frame_step()
{
    surfaces.commit();
    // Print::ln("Surfaces commited.");

    // wl_display_dispatch(state.core.display); // process all queued wayland events

    // wl_display_dispatch(state.core.display);
    
    int dispatched_event_count = wl_display_dispatch(core.display);
    // Print::ln({Str{"N.o. Dispatched events: "}, Str::SI(dispatched_event_count)});

    while(content_surface_is_busy())
    {
        // Print::ln("Content buffer still busy.");
        usleep(1000);
        dispatched_event_count = wl_display_dispatch(core.display);
        // Print::ln({Str{"N.o. Dispatched events: "}, Str::SI(dispatched_event_count)});
    }

    // if(surfaces_are_all_released())
    //     Print::ln("Surfaces are all released. ");
    

    // set_new_size({state.surfaces.content.size.x+1, state.surfaces.content.size.y});
    // set_new_size(state.surfaces.window.size + 1);
    // set_new_size(state.surfaces.content.size);

    if(surfaces.window_size_change_detected)
    {
    //     // Print::buf("Wayland window size change detected from main app. New size = { ");
    //     // Print::buf(Str::SI(state.surfaces.size_change_dims.x));
    //     // Print::buf(", ");
    //     // Print::buf(Str::SI(state.surfaces.size_change_dims.y));
    //     // Print::ln(" }");

        // state.surfaces.size_change_detected = false;

        // set_new_size(state.surfaces.size_change_dims);
    //     // wayland.reload({1200, 800});
    //     // piano_ui.renderer.set_buffer(wayland.get_pixel_buffer());
    }

    // Print::ln("Frame step done. \n");

    if(running)
    {
        return true;
    }
    
    return false;
}



void Wayland::destroy()
{
//     state.surfaces.window.fb.destroy();
//     state.surfaces.content.fb.destroy();
//     state.surfaces.test.fb.destroy();

    wl_display_disconnect(core.display);
}


void Wayland::run()
{

    uint dummy_i = 0;

    while (wl_display_dispatch(core.display))
    {

        if(running == 0)
        {
            Print::ln("exit");
            // wl_display_disconnect(state.wl_display);
            break;
        }
        if(input.pointer_pos_window_sane.y > 400.0)
        {
            // state.surfaces.window.fb.clear_gray();
            surfaces.commit();

        }
        if(input.pointer_pos_window_sane.y > 450.0)
        {
            // state.surfaces.window.fb.resize({1000, 600});
        }
        if(input.pointer_pos_window_sane.x > 500.0)
        {
            dummy_i++;
            // state.surfaces.window.fb.clear_green();
            // commit_surfaces(); 
        }
        if(input.pointer_pos_window_sane.x > 700.0)
        {
            
        }
        if(input.pointer_pos_window_sane.x > 900.0)
        {
            // state.surfaces.window.fb.resize({640, 480});
        }

    }

    destroy();
}


bool Wayland::set_size()
{
    if(!surfaces.window_size_change_detected)
        return false;

    Print::buf("Wayland window reload initiated. New size = { ");
    Print::buf(Str::SI(surfaces.size_change_dims.x));
    Print::buf(", ");
    Print::buf(Str::SI(surfaces.size_change_dims.y));
    Print::ln(" }");

    surfaces.full_reload_from_window_size(surfaces.size_change_dims, this);
    // surfaces.size_change_detected = false;
    // surfaces.reload_with_new_window_dims(surfaces.size_change_dims);

    

    return true;
}


PixelBuffer Wayland::get_content_buffer()
{
    return PixelBuffer {(PX32*)(surfaces.get_buffer_pointer(WL_Surface::Content)), 
                                surfaces.content.pixel_buffer.w, 
                                surfaces.content.pixel_buffer.h,
                                PX32F::ARGB,
                                PixelBuffer::Top    };
}


bool Wayland::content_surface_is_busy() { return surfaces.content.surface_is_busy; }
bool Wayland::surfaces_are_all_released() { return       !surfaces.decorations.surface.surface_is_busy 
                                            &&  !surfaces.content.surface_is_busy 
                                            &&  !surfaces.test.surface_is_busy;    }


void Wayland::close()
{
    running = false;
}

