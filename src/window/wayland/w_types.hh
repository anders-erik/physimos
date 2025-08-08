#pragma once

#include <wayland-client.h>
#include <wayland-cursor.h>
#include <wayland-egl.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GL/gl.h>

typedef struct WGlobal {
    struct wl_compositor* compositor;
    struct wl_display* display;
    struct wl_registry* registry;
    struct wl_shm* shm;
} WGlobal;

typedef struct WWindow {
    struct xdg_wm_base* wm_base;
    struct wl_surface* surface;
    struct xdg_surface* xdg_surface;
    struct xdg_toplevel* toplevel;
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
    
} WState;