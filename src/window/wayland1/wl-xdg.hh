
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

// #include "wl-state.hh"
// #include "wl-seat.hh"
// #include "wl-pointer.hh"
// #include "wl-keyboard.hh"





static void
xdg_surface_configure(  void *data,
                        struct xdg_surface *xdg_surface,
                        uint32_t serial                     )
{
    // struct client_state *state = (struct client_state *)data;
    xdg_surface_ack_configure(xdg_surface, serial);

    // Print::buf("serial: ");
    // Print::buf(Str::UI(serial));
    // Print::buf("\n");

    Print::ln("xdg_surface_configure called!");

    // render_wayland(data);
}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure,
};


static void
xdg_toplevel_configure( void *data,
                        struct xdg_toplevel *xdg_toplevel,
                        int32_t width,
                        int32_t height,
                        struct wl_array *states                     )
{
    Print::ln("xdg_toplevel_configure called!");

    Print::ln({Str{"new_width = "}, Str::SI(width), Str{",  new_height = "}, Str::SI(height)});

    Print::ln(Str::SI(states->size));
    uint32_t* array_data = (uint32_t*)states->data;
    Print::ln(Str::SI( *(array_data + 0) ));
    Print::ln(Str::SI( *(array_data + 1) ));
    Print::ln(Str::SI( *(array_data + 2) ));
    // Print::ln(Str::SI( *((((int32_t)states->data))) ));

    // xdg_toplevel_state state = wl_array_for_each(0, states);
    // uint8_t *key;
    // wl_array_for_each(key, (uint8_t *)states)
    // {
    //     // Print::ln(Str::SI(*key));
    //     Print::ln("toplevel configure loop");
    // }
}

static void
xdg_toplevel_close( void *data,
		            struct xdg_toplevel *xdg_toplevel)
{
    Print::ln("xdg_toplevel_close called!");
}

static void
xdg_toplevel_configure_bounds(  void *data,
                                struct xdg_toplevel *xdg_toplevel,
                                int32_t width,
                                int32_t height)
{
    Print::ln("xdg_toplevel_configure_bounds called!");
}

static void
xdg_toplevel_wm_capabilities(   void *data,
                                struct xdg_toplevel *xdg_toplevel,
                                struct wl_array *capabilities)
{
    Print::ln("xdg_toplevel_wm_capabilities called!");
}

static const struct xdg_toplevel_listener xdg_toplevel_listener = {
    .configure = xdg_toplevel_configure,
    .close = xdg_toplevel_close,
    .configure_bounds = xdg_toplevel_configure_bounds,
    .wm_capabilities = xdg_toplevel_wm_capabilities,
};

static void
xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};


