#pragma once

#include <wayland-client.h>
#include <wayland-cursor.h>


typedef struct WWCursor 
{
    struct wl_cursor_theme *theme;
    struct wl_cursor *wl_cursor;
    struct wl_buffer *buffer;

    struct wl_surface *surface;
    struct wl_cursor_image *image;
} WWCursor;
