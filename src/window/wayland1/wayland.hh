#pragma once

#include "lib/pixel_buffer.hh"

#include "wl-state.hh"


class Wayland
{
    WL_State state = { 0 }; 

    void connect_compositor();
    void create_surfaces(i2 dims);

    void init(i2 dims);
    void render(); // damage buffer and commit surface
    
    void destroy(); // destroy framebuffer & disconnect from compositor
    
    
public:

    Wayland()
    {
        i2 default_window_dims = {640, 480};

        init(default_window_dims);

        return;
    }
    Wayland(i2 dims)
    {
        init(dims);
    }
    ~Wayland()
    {
        destroy();
    }

    WL_State get_state() {return state;}

    PixelBuffer get_pixel_buffer()
    {
        return PixelBuffer {    (PX32*) state.surface_content.fb.data, 
                                        state.surface_content.fb.w, 
                                        state.surface_content.fb.h,
                                        PX32F::ARGB,
                                        PixelBuffer::Top    };
    }

    bool buffer_is_busy() { return state.fb.buffer_busy; }

    Arr<UserInput> get_new_input_events(); // returns a copy all buffered events and clears the wayland buffer    

    bool frame_step(); // renders current frame, process queued events, and returns true if new frame will begin, or false if exit-input detected
    void run(); // transfer process execution into wayland object

    void close()
    {
        state.running = false;
    }
};


