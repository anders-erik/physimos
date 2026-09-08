
#pragma once

#include <stdio.h>

#include "wayland-client.h"

#include "math/vecmat.hh"



struct WLFB
{
    uint32_t *data = 0; // pointer to the mmaped framebuffer
    struct wl_buffer *buffer = 0; 
    struct wl_shm_pool *pool;
    int shm_fd;
};

/* Wayland code */
typedef struct client_state {
    /* Globals */
    struct wl_display *wl_display;
    struct wl_registry *wl_registry;
    struct wl_shm *wl_shm;
    struct wl_compositor *wl_compositor;
    struct xdg_wm_base *xdg_wm_base;
    /* Objects */
    struct wl_surface *wl_surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
    /* Buffer objects */
    WLFB fb;
    /* Input*/
    struct wl_seat *wl_seat;
    int running = 1;
    u2 window_dims;
    d2 raw_pointer;
    d2 sane_pointer;
} client_state;