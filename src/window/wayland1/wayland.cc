
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

void Wayland::create_surfaces(i2 content_dims)
{
    state.surface_window.size = {   content_dims.x + state.window_border_width * 2, 
                                    content_dims.y + state.window_topbar_height + state.window_border_width};
    state.surface_window.init(state.core.compositor, &state);

    
    state.surface_content.size = {content_dims.x, content_dims.y};
    state.surface_content.pos = {state.window_border_width, state.window_topbar_height};
    state.surface_content.init(state.core.compositor, &state);
    state.surface_content.create_subsurface(state.core.subcompositor, state.surface_window.surface);
    
    
    state.surface_test.init(state.core.compositor, &state);
    state.surface_test.create_subsurface(state.core.subcompositor, state.surface_window.surface);


    state.xdg.surface = xdg_wm_base_get_xdg_surface(state.xdg.wm_base, state.surface_window.surface);
    xdg_surface_add_listener(state.xdg.surface, &state.xdg.xdg_surface_listener, &state);

    state.xdg.toplevel = xdg_surface_get_toplevel(state.xdg.surface);
    xdg_toplevel_add_listener(state.xdg.toplevel, &state.xdg.xdg_toplevel_listener, &state);
    xdg_toplevel_set_title(state.xdg.toplevel, "Example client");
}



void Wayland::init(i2 content_dims)
{
    connect_compositor();
    create_surfaces(content_dims);

    state.cursor.create_default(state.core.compositor, state.surface_window.fb.wl_shm);

    commit_surfaces(); // initial render to display the window
    // wl_display_dispatch(state.core.display); // caused a flash before first proper render
}

void Wayland::commit_surfaces()
{
    state.surface_window.commit();
    state.surface_content.commit();
    state.surface_test.commit();

    // state.surface_window.fb.buffer_busy = true;
}


Arr<UserInput> Wayland::get_new_input_events()
{
    Arr<UserInput> events = state.input.content_events;
    state.input.content_events.clear();
    return events;
}

bool Wayland::frame_step()
{
    commit_surfaces();
    // wl_display_dispatch(state.core.display); // process all queued wayland events

    while(buffer_is_busy())
    {
        // Print::ln("Content buffer still busy.");
        wl_display_dispatch(state.core.display);
    }

    if(state.running)
    {
        return true;
    }
    
    return false;
}



void Wayland::destroy()
{
    state.surface_window.fb.destroy();
    state.surface_content.fb.destroy();
    state.surface_test.fb.destroy();

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
            state.surface_window.fb.clear_gray();
            commit_surfaces();

        }
        if(state.input.pointer_pos_window_sane.y > 450.0)
        {
            state.surface_window.fb.resize({1000, 600});
        }
        if(state.input.pointer_pos_window_sane.x > 500.0)
        {
            dummy_i++;
            state.surface_window.fb.clear_green();
            commit_surfaces(); 
        }
        if(state.input.pointer_pos_window_sane.x > 700.0)
        {
            
        }
        if(state.input.pointer_pos_window_sane.x > 900.0)
        {
            state.surface_window.fb.resize({640, 480});
        }

    }

    destroy();
}

