
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
#include "wl-xdg.hh"
// #include "wl-pointer.hh"
// #include "wl-keyboard.hh"




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
        state->wl.compositor = (wl_compositor *)wl_registry_bind(
                wl_registry, name, &wl_compositor_interface, 4);
    }
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
    {
        state->xdg.wm_base = (xdg_wm_base *)wl_registry_bind(
                wl_registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(state->xdg.wm_base,
                &xdg_wm_base_listener, state);
    }
    else if (strcmp(interface, wl_seat_interface.name) == 0)
    {
        state->wl.seat = (wl_seat *)wl_registry_bind(
            wl_registry,
            name,
            &wl_seat_interface,
            1
        );

        wl_seat_add_listener(
            state->wl.seat,
            &seat_listener,
            data
        );

    }
}


static void registry_global_remove(
    void *data,
    struct wl_registry *registry,
    uint32_t name)
{
    printf("global removed: %u\n", name);
}

static const struct wl_registry_listener wl_registry_listener = {
    .global        = registry_global,
    .global_remove = registry_global_remove,
};
