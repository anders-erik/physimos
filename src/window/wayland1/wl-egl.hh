
#pragma once

#include <stdio.h>

#include "wayland-egl.h"
#include <EGL/egl.h>

#include "lib/print.hh"
#include "math/vecmat.hh"

#include "opengl/loader.hh"





struct WL_EGL
{
    GLsizei width = 20;
    GLsizei height = 20;

    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    EGLConfig config;
    struct wl_egl_window* window;

    EGLint num_configs = 0;

    struct Version
    {
        EGLint major = 0;
        EGLint minor = 0;
    } version;

    const EGLint config_attribs[13] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_NONE
    };
    const EGLint ctx_attribs[5] = {
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_CONTEXT_MINOR_VERSION, 3,
        EGL_NONE
    };


    void init(struct wl_display* wl_display, struct wl_surface* wl_surface, i2 dims);
    void reload_with_new_content_size(struct wl_surface* wl_surface, i2 content_dims);
    
    void check_egl_error(EGLint error_code);
    void swap_buffers();
};