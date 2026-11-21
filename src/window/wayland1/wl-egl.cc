

#include "wayland-egl.h"
#include <EGL/egl.h>

#include "opengl/loader.hh"
#include "wl-egl.hh"





void WL_EGL::init(struct wl_display* wl_display, struct wl_surface* wl_surface, i2 dims)
{
    width = dims.x;
    height = dims.y;

    // DISPLAY
    display = eglGetDisplay((EGLNativeDisplayType)wl_display);
    eglInitialize(display, &version.major, &version.minor);


    eglChooseConfig(    display, 
                        config_attribs, 
                        &config, 
                        1, 
                        &num_configs    );
    check_egl_error(eglGetError());

    // CONTEXT
    eglBindAPI(EGL_OPENGL_API);
    context = eglCreateContext( display, 
                                config, 
                                EGL_NO_CONTEXT, 
                                ctx_attribs         );
    check_egl_error(eglGetError());
    
    
    // SURFACE
    window = wl_egl_window_create(wl_surface, width, height);
    surface = eglCreateWindowSurface(display, config, window, NULL);
    
    eglMakeCurrent( display, 
                    surface, 
                    surface, 
                    context  );

    check_egl_error(eglGetError());
    

    // PRINT
    if(false)
    {
        fprintf(stdout, "EGL initialized. Version: %d.%d\n", version.major, version.minor);
        printf("EGL Configs found: %d\n", num_configs);
        fprintf(stdout, "EGL Context created: %p\n", context);
        fprintf(stdout, "EGL Surface created: %p\n", surface);
    }


    load_opengl((getProcAddress_T)eglGetProcAddress);
}


void WL_EGL::reload_with_new_content_size(struct wl_surface* wl_surface, i2 content_dims)
{
    width = content_dims.x;
    height = content_dims.y;

    wl_egl_window_resize(window, width, height, 0, 0);
    
    check_egl_error(eglGetError());
}




void WL_EGL::check_egl_error(EGLint error_code)
{
    if (error_code != EGL_SUCCESS) {
        fprintf(stderr, "ERROR: EGL: %d\n", error_code);
        exit(EXIT_FAILURE);
    }
}


void WL_EGL::swap_buffers()
{
    // eglSwapBuffers(display, surface);
    if (eglSwapBuffers(display, surface) == EGL_FALSE) {
        EGLint err = eglGetError();
        fprintf(stderr, "eglSwapBuffers failed: 0x%x\n", err);
        // return false;
    }
}