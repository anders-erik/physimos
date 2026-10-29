
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

    xdg_wm_base_add_listener(state.xdg.wm_base, &xdg_wm_base_listener, &state);
    wl_seat_add_listener(state.input.seat, &state.input.seat_listener, &state);

    // printf("display fd = %d\n", wl_display_get_fd(state.wl_display));
    // printf("error = %d\n", wl_display_get_error(state.wl_display));
}

void Wayland::create_surfaces(i2 dims)
{
    state.core.surface_window = wl_compositor_create_surface(state.core.compositor);

    state.fb.w = dims.x;
    state.fb.h = dims.y;
    state.fb.init(&(this->state));
    state.fb.clear_gray(); 
    

    state.surface_content.size = {state.fb.w*0.7, state.fb.h*0.7};
    state.surface_content.pos = {450, 30};
    // state.surface_content.size = {80, 80}; // BUG: surface content is not initialized with default size... this line is necessary
    state.surface_content.init(state.core.compositor, &state);
    // state.surface_content.fb.clear_green();
    state.surface_content.create_subsurface(state.core.subcompositor, state.core.surface_window);
    
    
    state.surface_test.init(state.core.compositor, &state);
    state.surface_test.create_subsurface(state.core.subcompositor, state.core.surface_window);


    state.xdg.surface = xdg_wm_base_get_xdg_surface(state.xdg.wm_base, state.core.surface_window);
    xdg_surface_add_listener(state.xdg.surface, &xdg_surface_listener, &state);

    state.xdg.toplevel = xdg_surface_get_toplevel(state.xdg.surface);
    xdg_toplevel_set_title(state.xdg.toplevel, "Example client");
}



void Wayland::init(i2 dims)
{
    connect_compositor();
    create_surfaces(dims);

    state.cursor.create_default(state.core.compositor, state.fb.wl_shm);

    render(); // initial render to display the window
}

void Wayland::render()
{
    wl_surface_damage_buffer(state.core.surface_window, 0, 0, state.fb.w, state.fb.h);
    wl_surface_attach(state.core.surface_window, state.fb.buffer, 0, 0);
    wl_surface_commit(state.core.surface_window);

    state.surface_content.render();
    state.surface_test.render();

    state.fb.buffer_busy = true;
}


Arr<UserInput> Wayland::get_new_input_events()
{
    Arr<UserInput> events = state.input.content_events;
    state.input.content_events.clear();
    return events;
}

bool Wayland::frame_step()
{
    render();

    wl_display_dispatch(state.core.display); // process all queued wayland events

    if(state.running)
        return true;
    
    return false;
}



void Wayland::destroy()
{
    state.fb.destroy();

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

    destroy();
}

