
#pragma once


// #define _POSIX_C_SOURCE 200112L
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <wayland-client.h>
#include "xdg_wl_extension/xdg-shell-client-protocol.h"

#include "lib/print.hh"

#include "wl-state.hh"
#include "wl-seat.hh"
// #include "wl-pointer.hh"
// #include "wl-keyboard.hh"





// static struct wl_buffer *
void
draw_frame(struct client_state *state)
{
    

    // const int width = 640, height = 480;
    const int width =  state->fb.w;
    const int height = state->fb.h;

    // int stride = width * 4;
    // int size = stride * height;

    // int fd = allocate_shm_file(size);
    // if (fd == -1) {
    //     return NULL;
    // }

    // // we unmap data before a reallocation
    // if(state->fb.data != 0)
    //     munmap(state->fb.data, size);

    // state->fb.data = (uint32_t*)mmap(NULL, size,
    //         PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    // if (state->fb.data == MAP_FAILED) {
    //     close(fd);
    //     return NULL;
    // }

    // Note: compositor allocates its own memory pool based on the fd. AFter the compoistor-allocation the currently mmaped file is no longer of any use?
    // struct wl_shm_pool *pool = wl_shm_create_pool(state->wl_shm, fd, size);
    // state->fb.buffer = wl_shm_pool_create_buffer(pool, 0, width, height, stride, WL_SHM_FORMAT_XRGB8888);
    // wl_shm_pool_destroy(pool);
    // close(fd);

    Print::ln("draw_frame called!");

    /* Draw checkerboxed background */
    // for (int y = 0; y < height; ++y) {
    //     for (int x = 0; x < width; ++x) {
    //         if ((x + y / 8 * 8) % 16 < 8)
    //             state->fb.data[y * width + x] = 0xFF666666;
    //         else
    //             state->fb.data[y * width + x] = 0xFFEEEEEE;
    //     }
    // }

    

    

    // wl_buffer_add_listener(state->fb.buffer, &wl_buffer_listener, NULL);
    // return state->fb.buffer;
}


void render_wayland(void *data)
{
    Print::ln("~~'render' called!~~");

    client_state *state = (client_state *)data;

    
    // state->fb.buffer = draw_frame(state);
    // draw_frame(state);


    const int width =  state->fb.w;
    const int height = state->fb.h;

    

    wl_surface_attach(state->wl_surface, state->fb.buffer, 0, 0);
    wl_surface_commit(state->wl_surface);
}

// void rebind_wl_buffer(struct client_state& state)
// {
//     const int width =  state.fb.w;
//     const int height = state.fb.h;

//     int size = width * height * 4;
//     int stride = width * 4;

//     state.fb.buffer = wl_shm_pool_create_buffer(state.fb.pool, 0, width, height, stride, WL_SHM_FORMAT_XRGB8888);
//     wl_buffer_add_listener(state.fb.buffer, &wl_buffer_listener, NULL);
// }


// void init_fb(struct client_state& state)
// {
//     const int width =  state.fb.w;
//     const int height = state.fb.h;

//     int size = width * height * 4;
//     int stride = width * 4;

//     state.fb.shm_fd = allocate_shm_file(size);
//     if (state.fb.shm_fd == -1)
//     {
//         Print::ln("Failed to allocate shm file in 'init_fb'");
//         return;
//     }

//     // we unmap data before a reallocation
//     // if(state.fb.data != 0)
//     //     munmap(state.fb.data, size);

//     state.fb.data = (uint32_t*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, state.fb.shm_fd, 0);
//     if (state.fb.data == MAP_FAILED)
//     {
//         close(state.fb.shm_fd);
//         return;
//     }

//     // Note: compositor allocates its own memory pool based on the fd. AFter the compoistor-allocation the currently mmaped file is no longer of any use?
//     // struct wl_shm_pool *pool = wl_shm_create_pool(state.wl_shm, state.fb.shm_fd, size);
//     state.fb.pool = wl_shm_create_pool(state.fb.wl_shm, state.fb.shm_fd, size);
//     // if(state.fb.buffer != 0)
//     //     wl_buffer_destroy(state.fb.buffer);
//     state.fb.buffer = wl_shm_pool_create_buffer(state.fb.pool, 0, width, height, stride, WL_SHM_FORMAT_XRGB8888);
//     // wl_shm_pool_destroy(pool);
//     // close(fd);


//     wl_buffer_add_listener(state.fb.buffer, &wl_buffer_listener, NULL);

//     state.fb.allocated = true;
// }


// void resize_fb(struct client_state& state, i2 new_dims)
// {
//     state.fb.w = new_dims.x;
//     state.fb.h = new_dims.y;

//     int size = state.fb.w * state.fb.h * 4;
//     int stride = state.fb.w * 4;

//     if(state.fb.allocated)
//     {
//         munmap(state.fb.data, size);
//         wl_buffer_destroy(state.fb.buffer);
//         wl_shm_pool_destroy(state.fb.pool);
//         close(state.fb.shm_fd);
//         state.fb.allocated = false;
//     }

//     state.fb.shm_fd = allocate_shm_file(size);
//     if (state.fb.shm_fd == -1)
//     {
//         Print::ln("Failed to allocate shm file in 'init_fb'");
//         return;
//     }

//     // we unmap data before a reallocation
//     // if(state.fb.data != 0)
//     //     munmap(state.fb.data, size);

//     state.fb.data = (uint32_t*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, state.fb.shm_fd, 0);
//     if (state.fb.data == MAP_FAILED)
//     {
//         close(state.fb.shm_fd);
//         return;
//     }

//     // Note: compositor allocates its own memory pool based on the fd. AFter the compoistor-allocation the currently mmaped file is no longer of any use?
//     // struct wl_shm_pool *pool = wl_shm_create_pool(state.wl_shm, state.fb.shm_fd, size);
//     state.fb.pool = wl_shm_create_pool(state.fb.wl_shm, state.fb.shm_fd, size);
//     // if(state.fb.buffer != 0)
//     //     wl_buffer_destroy(state.fb.buffer);
//     state.fb.buffer = wl_shm_pool_create_buffer(state.fb.pool, 0, state.fb.w, state.fb.h, stride, WL_SHM_FORMAT_XRGB8888);
//     // wl_shm_pool_destroy(pool);
//     // close(fd);


//     wl_buffer_add_listener(state.fb.buffer, &wl_buffer_listener, NULL);

//     state.fb.allocated = true;
// }

void destroy_fb(struct client_state& state)
{
    const int width = state.fb.w;
    const int height = state.fb.h;
    const int size = width*height*4;

    munmap(state.fb.data, size);
    wl_buffer_destroy(state.fb.buffer);
    wl_shm_pool_destroy(state.fb.pool);
    close(state.fb.shm_fd);

    state.fb.allocated = false;
}

static void
xdg_surface_configure(  void *data,
                        struct xdg_surface *xdg_surface,
                        uint32_t serial                     )
{
    // struct client_state *state = (struct client_state *)data;
    xdg_surface_ack_configure(xdg_surface, serial);

    Print::ln("xdg_surface_configure called!");

    // render_wayland(data);
}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure,
};

static void
xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};



static void
registry_global(void *data, struct wl_registry *wl_registry,
        uint32_t name, const char *interface, uint32_t version)
{
    struct client_state *state = (struct client_state *)data;

    if (strcmp(interface, wl_shm_interface.name) == 0)
    {
        state->fb.wl_shm = (wl_shm *)wl_registry_bind(
                wl_registry, name, &wl_shm_interface, 1);
    }
    else if (strcmp(interface, wl_compositor_interface.name) == 0)
    {
        state->wl_compositor = (wl_compositor *)wl_registry_bind(
                wl_registry, name, &wl_compositor_interface, 4);
    }
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
    {
        state->xdg_wm_base = (xdg_wm_base *)wl_registry_bind(
                wl_registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(state->xdg_wm_base,
                &xdg_wm_base_listener, state);
    }
    else if (strcmp(interface, wl_seat_interface.name) == 0)
    {
        state->wl_seat = (wl_seat *)wl_registry_bind(
            wl_registry,
            name,
            &wl_seat_interface,
            1
        );

        wl_seat_add_listener(
            state->wl_seat,
            &seat_listener,
            data
        );

    }

    Print::ln("Global");
}
