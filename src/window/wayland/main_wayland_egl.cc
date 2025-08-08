
#include <libdecor.h>

#include "gl_triangle.hh"
#include "w_types.hh"
#include "input.hh"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wayland-client.h>
#include <wayland-cursor.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GL/gl.h>

#include "protocols/xdg-shell-client-protocol.h"


static bool decor_configured = false;
static struct libdecor *decor;
static struct libdecor_frame *decor_frame;
static bool decor_active = false;



static void
wl_surface_frame_done(void *data, struct wl_callback *cb, uint32_t time);

static const struct wl_callback_listener wl_surface_frame_listener = {
    .done = wl_surface_frame_done,
};


static void
wl_surface_frame_done(void *data, struct wl_callback *cb, uint32_t time)
{
    WState *state = (WState *)data;
    
    // fprintf(stdout, "Frame done at time: %u\n", time);
    wl_callback_destroy(cb);

    eglSwapBuffers(state->egl.display, state->egl.surface);

    // print decor active
    if (decor_active) {
        // fprintf(stdout, "Decor is active.\n");
    } else {
        // fprintf(stdout, "Decor is not active.\n");
    }

    // Recreate the frame callback for the next frame
    struct wl_callback *new_cb = wl_surface_frame(state->window.surface);
    wl_callback_add_listener(new_cb, &wl_surface_frame_listener, state);

    // Here you can handle the frame done event, e.g., update the state or trigger a redraw
    // state.frame.time_prev = time; // Update previous frame time if needed

    // libdecor_frame_commit(decor_frame, NULL, NULL);

}

static void shm_format(void* data, struct wl_shm* shm, uint32_t format)
{
    fprintf(stderr, "Received shm format: %u\n", format);
    if (format == WL_SHM_FORMAT_XRGB8888) {
        fprintf(stderr, "Supported format: WL_SHM_FORMAT_XRGB8888\n");
    } else {
        fprintf(stderr, "Unsupported format: %u\n", format);
    }
}

static const struct wl_shm_listener shm_listener = {
    .format = shm_format
};


static void registry_handler(void* data, struct wl_registry* registry,
                             uint32_t id, const char* interface, uint32_t version)
{
    WState* state = (WState*)data;

    if (strcmp(interface, "wl_compositor") == 0) {
        state->global.compositor = (struct wl_compositor*) wl_registry_bind(registry, id, &wl_compositor_interface, 1);
    }
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0){
        state->window.wm_base = (xdg_wm_base *) wl_registry_bind(registry, id, &xdg_wm_base_interface, 1);
    }
    else if (strcmp(interface, wl_shm_interface.name) == 0) {
        state->global.shm = (wl_shm*) wl_registry_bind(registry, id, &wl_shm_interface, 1);
        wl_shm_add_listener(state->global.shm, &shm_listener, NULL);
    }
    else if(strcmp(interface, wl_seat_interface.name) == 0)
    {
        state->input.seat = (wl_seat*) wl_registry_bind(registry, id, &wl_seat_interface, 1);

        state->input.pointer = wl_seat_get_pointer(state->input.seat);
        wl_pointer_add_listener(state->input.pointer, &pointer_listener, state);

        state->input.keyboard = wl_seat_get_keyboard(state->input.seat);
        wl_keyboard_add_listener(state->input.keyboard, &keyboard_listener, NULL);
    }
}

static void registry_remover(void* data, struct wl_registry* registry, uint32_t id) {}

static const struct wl_registry_listener registry_listener = {
    registry_handler,
    registry_remover
};



static void xdg_ping(void* data, struct xdg_wm_base* wm, uint32_t serial) {
    xdg_wm_base_pong(wm, serial);
    // fprintf(stderr, "Received ping from compositor with serial: %u\n", serial);
}

static const struct xdg_wm_base_listener wm_base_listener = {
    .ping = xdg_ping
};


static void xdg_surface_configure(void* data, struct xdg_surface* s, uint32_t serial)
{
    xdg_surface_ack_configure(s, serial);
    fprintf(stdout, "xdg_surface_configure:: serial: %u\n", serial);
}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure
};


// Callback functions for frame events
static void handle_configure_decor(struct libdecor_frame *frame, struct libdecor_configuration *configuration, void *data)
{
    WState *state = (WState *)data;
    enum libdecor_window_state decor_window_state;
    struct libdecor_state *decor_state;

    fprintf(stdout, "[DECOR] Received configure event\n");
    int width, height;
    libdecor_configuration_get_content_size(configuration, frame, &width, &height);
    fprintf(stdout, "[DECOR] Configured size: %dx%d\n", width, height);

    if (!libdecor_configuration_get_window_state(configuration, &decor_window_state))
    {
		decor_window_state = LIBDECOR_WINDOW_STATE_NONE;
        fprintf(stdout, "[DECOR] Failed to get window state, using default: LIBDECOR_WINDOW_STATE_NONE\n");
    }
    else
    {
        if(decor_window_state == LIBDECOR_WINDOW_STATE_TILED_BOTTOM)
            decor_active = true;
        else
            decor_active = false;

        // fprintf(stdout, "[DECOR] Window state: %d\n", decor_window_state);
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

    
    if(!decor_configured)
    {
        fprintf(stdout, "First configure event received, setting up decor frame.\n");
        // Segfaults
        // libdecor_frame_commit(frame, NULL, configuration);

        decor_configured = true;
    }

    // eglSwapBuffers(state->egl.display, state->egl.surface);
    
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



void init_wayland(WState* state)
{
    state->global.display = wl_display_connect(NULL);
    if (!state->global.display)
    {
        fprintf(stderr, "Failed to connect to Wayland\n");
        exit(1);
    }

    state->global.registry = wl_display_get_registry(state->global.display);
    wl_registry_add_listener(state->global.registry, &registry_listener, state);
    wl_display_roundtrip(state->global.display);
    wl_display_roundtrip(state->global.display);


    state->window.surface = wl_compositor_create_surface(state->global.compositor);
}


void init_decor(WState* state)
{
     // CONTEXT
    decor = libdecor_new(state->global.display, NULL);
    if (!decor) {
        // Handle error
        fprintf(stderr, "Failed to create libdecor context\n");
    }

    // FRAME
    decor_frame = libdecor_decorate(decor, state->window.surface, &frame_iface, state);
    if (!decor_frame)
    {
        fprintf(stderr, "Failed to create libdecor frame\n");
        exit(1);
    }
    libdecor_frame_set_app_id(decor_frame, "libdecor-c++-demo");
    libdecor_frame_set_title(decor_frame, "libdecor C++ demo");
    libdecor_frame_map(decor_frame);

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


void check_egl_error(EGLint error_code)
{
    if (error_code != EGL_SUCCESS) {
        fprintf(stderr, "ERROR: EGL: %d\n", error_code);
        exit(EXIT_FAILURE);
    }
}

void init_egl(WState* state)
{
    EGLint major = 0;
    EGLint minor = 0;
    EGLint num_configs = 0;

    // DISPLAY
    state->egl.display = eglGetDisplay((EGLNativeDisplayType)state->global.display);
    eglInitialize(state->egl.display, &major, &minor);
    fprintf(stdout, "EGL initialized. Version: %d.%d\n", major, minor);


    // CONFIG
    EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_NONE
    };
    eglChooseConfig(    state->egl.display, 
                        config_attribs, 
                        &state->egl.config, 
                        1, 
                        &num_configs        );
    printf("EGL Configs found: %d\n", num_configs);

    check_egl_error(eglGetError());


    // CONTEXT
    eglBindAPI(EGL_OPENGL_API);
    EGLint ctx_attribs[] ={
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_CONTEXT_MINOR_VERSION, 3,
        EGL_NONE
    };
    state->egl.context = eglCreateContext(  state->egl.display, 
                                            state->egl.config, 
                                            EGL_NO_CONTEXT, 
                                            ctx_attribs         );

    check_egl_error(eglGetError());
    fprintf(stdout, "EGL Context created: %p\n", state->egl.context);

    // SURFACE
    state->egl.window = wl_egl_window_create(state->window.surface, 800, 600);
    state->egl.surface = eglCreateWindowSurface(state->egl.display, state->egl.config, state->egl.window, NULL);
    eglMakeCurrent( state->egl.display, 
                    state->egl.surface, 
                    state->egl.surface, 
                    state->egl.context  );

    check_egl_error(eglGetError());
    fprintf(stdout, "EGL Surface created: %p\n", state->egl.surface);

    // eglSwapBuffers(state->egl.display, state->egl.surface);
}

void load_glad()
{
    if (!gladLoadGLLoader((GLADloadproc)eglGetProcAddress)) {
        fprintf(stderr, "Failed to load OpenGL functions\n");
        exit(1);
    }
} 



int main()
{
    WState state;
    memset(&state, 0, sizeof(WState));

    init_wayland(&state);

    init_decor(&state);
    init_egl(&state);
    load_glad();

    struct wl_cursor_theme *cursor_theme =
        wl_cursor_theme_load(NULL, 24, state.global.shm);
    struct wl_cursor *cursor =
        wl_cursor_theme_get_cursor(cursor_theme, "left_ptr");
    state.input.cursor_image = cursor->images[0];
    struct wl_buffer *cursor_buffer =
        wl_cursor_image_get_buffer(state.input.cursor_image);

    state.input.cursor_surface = wl_compositor_create_surface(state.global.compositor);
    wl_surface_attach(state.input.cursor_surface, cursor_buffer, 0, 0);
    wl_surface_commit(state.input.cursor_surface);

    struct wl_callback *cb = wl_surface_frame(state.window.surface);
    wl_callback_add_listener(cb, &wl_surface_frame_listener, &state);

    gl_triangle_init();

    // while (libdecor_dispatch(decor_context, -1) >= 0);

    while (1)
    {
        gl_triangle_draw();

        eglSwapBuffers(state.egl.display, state.egl.surface);
        wl_display_dispatch_pending(state.global.display);

        libdecor_dispatch(decor, 0);
        // libdecor_frame_commit(decor_frame, NULL, NULL);


        // fprintf(stdout, "Frame rendered\n");
    }

    return 0;
}
