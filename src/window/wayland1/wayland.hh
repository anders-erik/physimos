#pragma once

#include "lib/pixel_buffer.hh"

#include "wl-state.hh"

#define WAYLAND_DEFAULT_CONTENT_SIZE {640, 480}

struct Wayland
{
    WL_State state = { 0 };

    Wayland();
    Wayland(i2 dims);
    ~Wayland();

    void connect_compositor();
    void init(i2 dims);
    bool frame_step(); // renders current frame, process queued events, and returns true if new frame will begin, or false if exit-input detected
    void run(); // transfer process execution into wayland object
    void destroy(); // destroy framebuffer & disconnect from compositor
    
    /** Will reload all content if an internal resize event has occured, else nothing will happen. 
        Returns 'true' if reloaded, else 'false'. */
    bool reload_size();


    PixelBuffer get_pixel_buffer();

    bool content_surface_is_busy();
    bool surfaces_are_all_released();

    Arr<UserInput> get_new_input_events(); // returns a copy all buffered events and clears the wayland buffer    


    void close();
};


