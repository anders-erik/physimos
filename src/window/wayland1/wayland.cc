
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
#include "wl-state.hh"
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
    state.core.display = wl_display_connect(NULL);
    if (!state.core.display)
    {
        Print::ln("Failed to connect to Wayland display\n");
        return;
    }
    // This generates a runtime message that the 'proxy <address> already has listener'
    wl_display_add_listener(state.core.display, &state.core.display_listener, &state);

    // think: wl_display.get_registry(); !!
    state.core.registry = wl_display_get_registry(state.core.display);
    if (!state.core.registry)
    {
        Print::ln("Failed to get Wayland registry.\n");
        return;
    }

    wl_registry_add_listener(state.core.registry, &state.core.registry_listener, &state);

    // Process all pending requests (and will block until completed)
    int ret = wl_display_roundtrip(state.core.display);
    if (ret < 0)
    {
        printf("Wl_display_roundtrip failure. Display error: %d\n", wl_display_get_error(state.core.display));
        return;
    }

    if (!state.core.compositor || !state.core.subcompositor)
    {
        fprintf(stderr, "Required interfaces (compositor/subcompositor) missing.\n");
        return;
    }

    xdg_wm_base_add_listener(state.xdg.wm_base, &state.xdg.xdg_wm_base_listener, &state);
    wl_seat_add_listener(state.input.seat, &state.input.seat_listener, &state);

    // printf("display fd = %d\n", wl_display_get_fd(state.wl_display));
    // printf("error = %d\n", wl_display_get_error(state.wl_display));
}



    



void Wayland::init(i2 content_dims)
{
    connect_compositor();

    state.surfaces.init(content_dims, &state);
    
    state.xdg.surface = xdg_wm_base_get_xdg_surface(state.xdg.wm_base, state.surfaces.window.surface);
    xdg_surface_add_listener(state.xdg.surface, &state.xdg.xdg_surface_listener, &state);

    state.xdg.toplevel = xdg_surface_get_toplevel(state.xdg.surface);
    xdg_toplevel_add_listener(state.xdg.toplevel, &state.xdg.xdg_toplevel_listener, &state);
    xdg_toplevel_set_title(state.xdg.toplevel, "Example client");


    // state.cursor.create_default(state.core.compositor, state.surfaces.window.fb.wl_shm);
    state.cursor.create_default(state.core.compositor, state.surfaces.pool.wl_shm);

    state.surfaces.commit();
    // wl_display_dispatch(state.core.display); // caused a flash before first proper render
}


Arr<UserInput> Wayland::get_new_input_events()
{
    Arr<UserInput> events = state.input.content_events;
    state.input.content_events.clear();
    return events;
}

bool Wayland::frame_step()
{
    state.surfaces.commit();
    Print::ln("Surfaces commited.");

    // wl_display_dispatch(state.core.display); // process all queued wayland events

    // wl_display_dispatch(state.core.display);
    
    int dispatched_event_count = wl_display_dispatch(state.core.display);
    Print::ln({Str{"N.o. Dispatched events: "}, Str::SI(dispatched_event_count)});

    while(content_surface_is_busy())
    {
        Print::ln("Content buffer still busy.");
        usleep(1000);
        dispatched_event_count = wl_display_dispatch(state.core.display);
        Print::ln({Str{"N.o. Dispatched events: "}, Str::SI(dispatched_event_count)});
    }

    // if(surfaces_are_all_released())
    //     Print::ln("Surfaces are all released. ");
    

    // set_new_size({state.surfaces.content.size.x+1, state.surfaces.content.size.y});
    // set_new_size(state.surfaces.window.size + 1);
    // set_new_size(state.surfaces.content.size);

    if(state.surfaces.size_change_detected)
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

    Print::ln("Frame step done. \n");

    if(state.running)
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

    wl_display_disconnect(state.core.display);
}


void Wayland::run()
{

    uint dummy_i = 0;

    while (wl_display_dispatch(state.core.display))
    {

        if(state.running == 0)
        {
            Print::ln("exit");
            // wl_display_disconnect(state.wl_display);
            break;
        }
        if(state.input.pointer_pos_window_sane.y > 400.0)
        {
            // state.surfaces.window.fb.clear_gray();
            state.surfaces.commit();

        }
        if(state.input.pointer_pos_window_sane.y > 450.0)
        {
            // state.surfaces.window.fb.resize({1000, 600});
        }
        if(state.input.pointer_pos_window_sane.x > 500.0)
        {
            dummy_i++;
            // state.surfaces.window.fb.clear_green();
            // commit_surfaces(); 
        }
        if(state.input.pointer_pos_window_sane.x > 700.0)
        {
            
        }
        if(state.input.pointer_pos_window_sane.x > 900.0)
        {
            // state.surfaces.window.fb.resize({640, 480});
        }

    }

    destroy();
}


bool Wayland::reload_size()
{
    if(!state.surfaces.size_change_detected)
        return false;

    Print::buf("Wayland window reload initiated. New size = { ");
    Print::buf(Str::SI(state.surfaces.size_change_dims.x));
    Print::buf(", ");
    Print::buf(Str::SI(state.surfaces.size_change_dims.y));
    Print::ln(" }");

    state.surfaces.size_change_detected = false;

    i2 new_content_dims = state.surfaces.window_to_content_dims(state.surfaces.size_change_dims);
    state.surfaces.set_content_dims(new_content_dims);
    state.surfaces.new_wl_buffers(&state);

    return true;
}


PixelBuffer Wayland::get_pixel_buffer()
{
    return PixelBuffer {(PX32*)(state.surfaces.get_content_pointer()), 
                                state.surfaces.content.size.x, 
                                state.surfaces.content.size.y,
                                PX32F::ARGB,
                                PixelBuffer::Top    };
}


bool Wayland::content_surface_is_busy() { return state.surfaces.content.surface_is_busy; }
bool Wayland::surfaces_are_all_released() { return       !state.surfaces.window.surface_is_busy 
                                            &&  !state.surfaces.content.surface_is_busy 
                                            &&  !state.surfaces.test.surface_is_busy;    }


void Wayland::close()
{
    state.running = false;
}

