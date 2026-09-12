
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> // socket
#include <sys/un.h> // sockaddr_un


#include "lib/print.hh"


#include "wayland.hh"
#include "wayland-rend.hh"
#include "swrend/wayland.hh"

#include "wl-state.hh"


#include <wayland-client.h>
// #include "swrend/wayland.hh"

#include "wl-book.hh"
#include "wl-pointer.hh"

#include "swrend.hh"


struct XDG
{
    static Str get_runtime_dir()
    {
        const char *runtime = getenv("XDG_RUNTIME_DIR");
        return Str{runtime};
    };

    static Str get_wayland_display_socket_name()
    {
        const char *wayland_display = getenv("WAYLAND_DISPLAY");
        return Str{wayland_display};
    };
};


struct Socket
{
    static Str read_from_socket(Str socket_path)
    {
        int fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (fd < 0)
            return "ERROR: failed to open socket.";
        
        // int flags = fcntl(fd, F_GETFL, 0);
        // fcntl(fd, F_SETFL, flags | O_NONBLOCK);

        struct sockaddr_un addr = {0};
        addr.sun_family = AF_UNIX;

        strncpy(addr.sun_path, socket_path.to_c_str(), sizeof(addr.sun_path) - 1);

        if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            close(fd);
            return "Connected to socket with error code.";
        }

        // Wayland socket is available. Server will wait for initiation of wl-protocal mesage before providing data.

        char buf[4096];

        ssize_t n;
        // ssize_t n = read(fd, buf, sizeof(buf));
        return "Early return because 'read' blocks the process and no wayland connection is available.";

        if (n < 0) {
            perror("read");
        } else if (n == 0) {
            printf("server disconnected\n");
        } else {
            printf("received %zd bytes\n", n);
        }

        close(fd);

        return "Connected to socket.";
    };
};









struct WLRegistry
{
    typedef struct wl_registry_listener Listener;
};


static void registry_global_remove(
    void *data,
    struct wl_registry *registry,
    uint32_t name)
{
    printf("global removed: %u\n", name);
}

static const WLRegistry::Listener wl_registry_listener = {
    .global        = registry_global,
    .global_remove = registry_global_remove,
};




Wayland::Wayland()
{
    Print::ln("Default Wayland constructor!");


    // opaque object passed to qayland server during communication
    // struct wl_display *display = wl_display_connect(NULL);
    state.wl_display = wl_display_connect(NULL);
    if (!state.wl_display) {
        Print::ln("Failed to connect to Wayland display\n");
        return;
    }
    Print::ln("wl_display_connect: OK");

    // think: wl_display.get_registry(); !!
    // struct wl_registry *registry = wl_display_get_registry(state.wl_display);
    state.wl_registry = wl_display_get_registry(state.wl_display);
    if (!state.wl_registry)
    {
        Print::ln("Failed to get Wayland registry.\n");
        return;
    }
    Print::ln("wl_display_get_registry: OK");

    wl_registry_add_listener(state.wl_registry, &wl_registry_listener, &state);

    // Process all pending requests (and will block until completed)
    int ret = wl_display_roundtrip(state.wl_display);
    if (ret < 0) {
        printf("Wl_display_roundtrip failure. Display error: %d\n", wl_display_get_error(state.wl_display));
        return;
    }

    // printf("display fd = %d\n", wl_display_get_fd(state.wl_display));
    // printf("error = %d\n", wl_display_get_error(state.wl_display));


    state.wl_surface = wl_compositor_create_surface(state.wl_compositor);
    state.xdg_surface = xdg_wm_base_get_xdg_surface(state.xdg_wm_base, state.wl_surface);
    xdg_surface_add_listener(state.xdg_surface, &xdg_surface_listener, &state);
    state.xdg_toplevel = xdg_surface_get_toplevel(state.xdg_surface);
    xdg_toplevel_set_title(state.xdg_toplevel, "Example client");
    wl_surface_commit(state.wl_surface);

    setup_ok = true;
    return;
}


void draw_square(void* )
{

}

void Wayland::run()
{

    wayland_render(0);

    // state.window_dims = {640, 480};
    state.fb.w = 640;
    state.fb.h = 480;

    // init_fb(state);
    state.fb.init();

    SWR::Buf swr_buf {  (PX32*)state.fb.ptr(), 
                        state.fb.w, 
                        state.fb.h,
                        PX32F::ARGB,
                        SWR::Buf::Top                  };
    

    // clear_fb_gray(state);

    state.fb.clear_gray();

    swr_buf.clear(0x00663333);

    swr_buf.draw_point({10, 10}, 0x12345678);
    swr_buf.draw_point({20, 10}, 0x12345678);
    swr_buf.draw_point({30, 10}, 0x12345678);
    swr_buf.draw_point({11, 10}, 0x00FFFFFF);
    swr_buf.draw_point({12, 10}, 0x00FFFFFF);
    swr_buf.draw_point({13, 10}, 0x00FFFFFF);

    Bitmap white_4x4 {4, 4, PX32F::ARGB};
    // white_4x4.clear(0x00FFFFFF); // XRGB format for wayland compatibility
    // white_4x4.clear(0xFFFFFF00); //
    white_4x4.clear_RGBA(0xFFFFFF00); // automatically converts the pixel to match underlying format
    // white_4x4.set_format(PX32F::ARGB);
    swr_buf.paste(white_4x4, {0, 476});


    Bitmap triangle_bmp {40, 40, PX32F::ARGB};
    triangle_bmp.clear_RGBA(0x994444FF);
    swr_buf.paste(triangle_bmp, {100, 100});
    
    

    render_wayland(&state); // initial render to display the window

    // const int width = (int) state.window_dims.x;
    // const int height = (int) state.window_dims.y;

    uint dummy_i = 0;

    while (wl_display_dispatch(state.wl_display))
    {
        // rebind_wl_buffer(state);
        // render_wayland(&state); 

       

        if(state.running == 0)
        {
            Print::ln("exit");
            // wl_display_disconnect(state.wl_display);
            break;
        }
        if(state.sane_pointer.y > 400.0)
        {
            // rebind_wl_buffer(state);
            // render_wayland(&state);
            // init_fb(state);
            // wl_surface_attach(state.wl_surface, state.fb.buffer, 0, 0);
            // wl_surface_commit(state.wl_surface);

            state.fb.clear_gray();
            // clear_fb_gray(state);
            wl_surface_damage_buffer(state.wl_surface, 0, 0, state.fb.w, state.fb.h);
            wl_surface_attach(state.wl_surface, state.fb.buffer, 0, 0);
            wl_surface_commit(state.wl_surface);

        }
        if(state.sane_pointer.y > 450.0)
        {
            state.fb.resize({1000, 600});
            // resize_fb(state, {1000, 600});
            // resize_fb(state, {500, 300});
            // wl_surface_damage_buffer(state.wl_surface, 0, 0, state.fb.w, state.fb.h);
            // wl_surface_attach(state.wl_surface, state.fb.buffer, 0, 0);
            // wl_surface_commit(state.wl_surface);
        }
        if(state.sane_pointer.x < 50.0)
        {
            // resize_fb(state, {640, 480});
            // resize_fb(state, {1000, 600});
            // wl_surfac6, 6e_damage_buffer(state.wl_surface, 0, 0, state.fb.w, state.fb.h);
            // wl_surface_attach(state.wl_surface, state.fb.buffer, 0, 0);
            // wl_surface_commit(state.wl_surface);
        }
        if(state.sane_pointer.x > 500.0)
        {
            // Print::ln("x > 400");
            dummy_i++;
            // state.fb.data[dummy_i] += 65000;

            // destroy_fb(state);
            // init_fb(state);
            state.fb.clear_green();
            // clear_fb_green(state);
            // rebind_wl_buffer(state);
            render_wayland(&state); 


            wl_surface_damage_buffer(state.wl_surface, 0, 0, state.fb.w, state.fb.h);
            wl_surface_attach(state.wl_surface, state.fb.buffer, 0, 0);
            wl_surface_commit(state.wl_surface);
        }
        if(state.sane_pointer.x > 700.0)
        {
            
        }
        if(state.sane_pointer.x > 900.0)
        {
            state.fb.resize({640, 480});
        }
        /* This space deliberately left blank */
    }

    destroy_fb(state);

    wl_display_disconnect(state.wl_display);
}

void Wayland::socket_test()
{
    Print::buf("\n");

    Str xdg_rt_dir = XDG::get_runtime_dir();
    Print::buf("xdg_rt_dir = ");
    Print::ln(xdg_rt_dir);

    Str xdg_wayland_socket = XDG::get_wayland_display_socket_name();
    Print::buf("xdg_wayland_socket = ");
    Print::ln(xdg_wayland_socket);

    Str wl_socket_path = xdg_rt_dir + "/" + xdg_wayland_socket;
    Print::buf("wl_socket_path = ");
    Print::ln(wl_socket_path);

    Str wl_socket_read = Socket::read_from_socket(wl_socket_path);
    Print::buf("wl_socket_read = ");
    Print::ln(wl_socket_read);
}