#pragma once

#include <wayland-client.h>
#include <wayland-egl.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GL/gl.h>

#include "ww_cursor.hh"

// #ifdef  __cplusplus
// extern "C" {
// #endif


typedef struct WGlobal {
    struct wl_compositor* compositor;
    struct wl_display* display;
    struct wl_registry* registry;
    struct wl_shm* shm;
} WGlobal;

typedef struct WWindow {
    struct xdg_wm_base* wm_base;
    struct xdg_surface* xdg_surface;
    struct xdg_toplevel* toplevel;
    struct wl_subcompositor* subcompositor;

    struct wl_surface* surface;
    uint32_t content_surface_id;
    int hovering_content_surface;
    struct wl_surface* surface_render;
    struct wl_subsurface* subsurface;
} WWindow;

typedef struct WInput {
    struct wl_seat* seat;
    struct wl_pointer* pointer;
    struct wl_keyboard* keyboard;
    
    struct wl_surface *cursor_surface;
    struct wl_cursor_image *cursor_image;
} WInput;


typedef struct EGLState {
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    EGLConfig config;
    struct wl_egl_window* window;
} EGLState;

typedef struct WState {
    WGlobal global;
    WWindow window;
    EGLState egl;
    WInput input;
    WWCursor cursor;
} WState;


// #ifdef  __cplusplus
// }
// #endif
