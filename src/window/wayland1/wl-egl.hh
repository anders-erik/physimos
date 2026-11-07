
#pragma once

#include <stdio.h>

#include "wayland-egl.h"
#include <EGL/egl.h>

#include "lib/print.hh"
#include "math/vecmat.hh"

#include "wl-gl.hh"




struct WL_EGL
{
    GLuint program;
    GLuint vao, vbo;
    float verts[9] = {
        0.0f,  1.0f, 0.0f, // top center
        -1.0f, -1.0f, 0.0f, // bottom left
        1.0f, -1.0f, 0.0f  // bottom right
    };

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

    void check_egl_error(EGLint error_code);

    void init(struct wl_display* wl_display, struct wl_surface* wl_surface, i2 dims);

    void load_GL();

    void print_opengl_error();

    void draw_triangle();

    GLuint compile_shader(GLenum type, const char* src);

};