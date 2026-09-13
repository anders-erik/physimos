
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


