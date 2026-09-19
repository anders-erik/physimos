
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> // socket
#include <sys/un.h> // sockaddr_un


#include "lib/print.hh"


#include "wayland.hh"

#include "wl-state.hh"


#include <wayland-client.h>
// #include "swrend/wayland.hh"

// #include "wl-book.hh"
#include "wl-pointer.hh"
#include "wl-registry.hh"




void Wayland::render()
{
    wl_surface_damage_buffer(state.wl.surface, 0, 0, state.fb.w, state.fb.h);
    wl_surface_attach(state.wl.surface, state.fb.buffer, 0, 0);
    wl_surface_commit(state.wl.surface);
}





Wayland::Wayland(i2 dims)
{
    init(dims);
}

Wayland::Wayland()
{
    // Print::ln("Default Wayland constructor!");

    i2 default_window_dims = {640, 480};

    init(default_window_dims);

    return;
}


void Wayland::init(i2 dims)
{
    // opaque object passed to qayland server during communication
    // struct wl_display *display = wl_display_connect(NULL);
    state.wl.display = wl_display_connect(NULL);
    if (!state.wl.display)
    {
        Print::ln("Failed to connect to Wayland display\n");
        return;
    }

    // think: wl_display.get_registry(); !!
    state.wl.registry = wl_display_get_registry(state.wl.display);
    if (!state.wl.registry)
    {
        Print::ln("Failed to get Wayland registry.\n");
        return;
    }

    wl_registry_add_listener(state.wl.registry, &wl_registry_listener, &state);

    // Process all pending requests (and will block until completed)
    int ret = wl_display_roundtrip(state.wl.display);
    if (ret < 0) {
        printf("Wl_display_roundtrip failure. Display error: %d\n", wl_display_get_error(state.wl.display));
        return;
    }

    // printf("display fd = %d\n", wl_display_get_fd(state.wl_display));
    // printf("error = %d\n", wl_display_get_error(state.wl_display));


    state.wl.surface = wl_compositor_create_surface(state.wl.compositor);
    state.xdg.surface = xdg_wm_base_get_xdg_surface(state.xdg.wm_base, state.wl.surface);
    xdg_surface_add_listener(state.xdg.surface, &xdg_surface_listener, &state);
    state.xdg.toplevel = xdg_surface_get_toplevel(state.xdg.surface);
    xdg_toplevel_set_title(state.xdg.toplevel, "Example client");
    wl_surface_commit(state.wl.surface);



    state.fb.w = dims.x;
    state.fb.h = dims.y;

    state.fb.init();
    state.fb.clear_gray();    

    render(); // initial render to display the window
}

void draw_square(void* )
{
    
}

Arr<UserInput> Wayland::get_new_input_events()
{
    Arr<UserInput> events = state.input.w_events;
    state.input.w_events.clear();
    return events;
}

bool Wayland::frame_step()
{
    render();

    wl_display_dispatch(state.wl.display); // process all queued wayland events

    if(state.running)
        return true;
    
    return false;
}



void Wayland::close()
{
    state.fb.destroy();

    wl_display_disconnect(state.wl.display);
}


void Wayland::run()
{

    uint dummy_i = 0;

    while (wl_display_dispatch(state.wl.display))
    {

        if(state.running == 0)
        {
            Print::ln("exit");
            // wl_display_disconnect(state.wl_display);
            break;
        }
        if(state.sane_pointer.y > 400.0)
        {
            state.fb.clear_gray();
            render();

        }
        if(state.sane_pointer.y > 450.0)
        {
            state.fb.resize({1000, 600});
        }
        if(state.sane_pointer.x > 500.0)
        {
            dummy_i++;
            state.fb.clear_green();
            render(); 
        }
        if(state.sane_pointer.x > 700.0)
        {
            
        }
        if(state.sane_pointer.x > 900.0)
        {
            state.fb.resize({640, 480});
        }

    }

    close();
}

