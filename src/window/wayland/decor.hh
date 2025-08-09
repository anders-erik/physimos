#pragma once

#include <cstdio>

#include <libdecor.h>

#include "w_types.hh"

#include "gl_triangle.hh"

struct Decor 
{
    struct libdecor *context;
    struct libdecor_frame *frame;
    bool configured = false;
};

static Decor decor;


// Callback functions for frame events
static void handle_configure_decor(struct libdecor_frame *frame, struct libdecor_configuration *configuration, void *data)
{
    WState *state = (WState *)data;
    enum libdecor_window_state decor_window_state;
    struct libdecor_state *decor_state;

    fprintf(stdout, "[DECOR] Received configure event\n");
    int width, height;
    // The size available within the libdecor frame and thus available for rendering
    libdecor_configuration_get_content_size(configuration, frame, &width, &height);
    fprintf(stdout, "[DECOR] Configured content size: %dx%d\n", width, height);

    if (!libdecor_configuration_get_window_state(configuration, &decor_window_state))
    {
		decor_window_state = LIBDECOR_WINDOW_STATE_NONE;
        fprintf(stdout, "[DECOR] Failed to get window state, using default: LIBDECOR_WINDOW_STATE_NONE\n");
    }


    

    // SET SIZE
    {
        decor_state = libdecor_state_new(width, height);
        gl_triangle_set_viewport(width, height);
        wl_egl_window_resize(state->egl.window, width, height, 0, 0);
    }

    libdecor_frame_commit(frame, decor_state, configuration);
    libdecor_state_free(decor_state);

    if (!state->egl.window) {
        fprintf(stderr, "Error creating EGL surface\n");
        exit(1);
    }

    // Create EGL surface if needed
    if (!state->egl.surface) {
        fprintf(stderr, "Error creating EGL surface\n");
        exit(1);
    }

    
    if(!decor.configured)
    {
        fprintf(stdout, "First configure event received, setting up decor frame.\n");
        // Segfaults
        // libdecor_frame_commit(frame, NULL, configuration);

        decor.configured = true;
    }

    // eglSwapBuffers(state->egl.display, state->egl.surface);
    
    // CONTENT SIZE
    int decor_content_width = 0;
    int decor_content_height = 0;

    libdecor_frame_get_max_content_size(    decor.frame,
                                            &decor_content_width,
                                            &decor_content_height );
    // fprintf(stdout, "Decor MAX content size: %dx%d\n", decor_content_width, decor_content_height);

    bool got_size_ok = libdecor_configuration_get_content_size( configuration,
                                                                decor.frame,
                                                                &decor_content_width,
                                                                &decor_content_height );
    // fprintf(stdout, "Decor content size: %dx%d\n", decor_content_width, decor_content_height);
}

static void handle_close_decor(struct libdecor_frame *frame, void *data)
{
    fprintf(stdout, "[DECOR] Received close event\n");
    exit(EXIT_SUCCESS);
}

static void handle_commit(struct libdecor_frame *frame,
                    void *data)
{
    WState *state = (WState *)data;
    fprintf(stdout, "[DECOR] Frame commit called\n");
    // commit surface
    wl_surface_commit(state->window.surface);
}

static void handle_dismiss_popup(struct libdecor_frame *frame, const char *seat_name, void *user_data)
{
}

// Define the interface with your event handlers
static struct libdecor_frame_interface frame_iface = {
    .configure = handle_configure_decor,
    .close = handle_close_decor,
    .commit = handle_commit,
    .dismiss_popup = handle_dismiss_popup,
};


void init_decor(WState* state)
{
     // CONTEXT
    decor.context = libdecor_new(state->global.display, NULL);
    if (!decor.context) {
        // Handle error
        fprintf(stderr, "Failed to create libdecor context\n");
    }

    // FRAME
    decor.frame = libdecor_decorate(    decor.context, 
                                        state->window.surface, 
                                        &frame_iface, 
                                        state                   );
    if (!decor.frame)
    {
        fprintf(stderr, "Failed to create libdecor frame\n");
        exit(1);
    }
    libdecor_frame_set_app_id(decor.frame, "physimos_payland");
    libdecor_frame_set_title(decor.frame, "Physimos - Wayland");
    libdecor_frame_map(decor.frame);

    // libdecor_frame_commit(decor_frame, NULL, NULL);
    // libdecor_decorate(  decor, 
    //                     state->window.surface, 
    //                     &frame_iface,
    //                     state                   );

    // print frame pointer
    // fprintf(stdout, "[DECOR] Frame pointer: %p\n", decor_frame);
    
    // wl_display_roundtrip(state->global.display);
    // wl_display_dispatch(state->global.display);
    // while (!decor_configured) {
    // }
    
}