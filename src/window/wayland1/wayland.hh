#pragma once

#include "lib/pixel_buffer.hh"

#include "wl-objects.hh"
#include "wl-surfaces.hh"

#define WAYLAND_DEFAULT_CONTENT_SIZE {640, 480}

struct Wayland
{
    /* Wayland Objects */
    WL_Core core;
    /* XDG Objects */
    WL_XDG xdg;
    /* Surface handler*/
    WL_Surfaces surfaces;
    // /* EGL object */
    WL_EGL egl;
    /* Object to handle the cursor image */
    WL_Cursor cursor;
    /* Input*/
    WL_Input input;

    int running = 1;

    Wayland();
    Wayland(i2 dims);
    ~Wayland();

    void connect_compositor();
    void init(i2 dims);
    void run(); // transfer process execution into wayland object
    Arr<UserInput> get_new_input_events(); // returns a copy all buffered events and clears the wayland buffer    
    void close();
    void destroy(); // destroy framebuffer & disconnect from compositor
    
    bool frame_step(); // renders current frame, process queued events, and returns true if new frame will begin, or false if exit-input detected
    bool set_size(); /** Sets new size if resize-flag detected, else nothing will happen. Returns 'true' if resized, 'false' otherwise. */
    PixelBuffer get_content_buffer();

    bool content_surface_is_busy();
    bool surfaces_are_all_released();
};


