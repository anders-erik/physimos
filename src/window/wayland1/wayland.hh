#pragma once

#include "lib/pixel_buffer.hh"

#include "wl-state.hh"


class Wayland
{
    WaylandState state = { 0 }; 

    void init(i2 dims);
    void render(); // damage buffer and commit surface

public:

    Wayland(i2 dims);  
    Wayland();


    PixelBuffer get_pixel_buffer()
    {
        return PixelBuffer {    (PX32*) state.fb.data, 
                                        state.fb.w, 
                                        state.fb.h,
                                        PX32F::ARGB,
                                        PixelBuffer::Top    };
    }

    bool buffer_is_busy() { return state.fb.buffer_busy; }

    Arr<UserInput> get_new_input_events(); // returns a copy all buffered events and clears the wayland buffer    

    bool frame_step(); // renders current frame, process queued events, and returns true if new frame will begin, or false if exit-input detected
    void run(); // transfer process execution into wayland object

    void close(); // destroy framebuffer & disconnect from compositor

};


