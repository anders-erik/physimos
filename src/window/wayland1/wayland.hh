#pragma once

#include "lib/pixel_buffer.hh"

#include "wl-state.hh"

#define WAYLAND_DEFAULT_CONTENT_SIZE {640, 480}

class Wayland
{
    WL_State state = { 0 };

    void connect_compositor();

    void init(i2 dims);
    
    void destroy(); // destroy framebuffer & disconnect from compositor
    
    
public:

    Wayland()
    {
        init(WAYLAND_DEFAULT_CONTENT_SIZE);
    }
    Wayland(i2 dims)
    {
        init(dims);
    }
    ~Wayland()
    {
        destroy();
    }

    void set_new_size(i2 new_content_dims)
    {
        state.surfaces.set_content_dims(new_content_dims);
        state.surfaces.new_wl_buffers(&state);

        // wl_display_dispatch(state.core.display);
        
        // state.surfaces.commit();
        // 

        // destroy();
        // init(new_content_dims);
        // state.surface_window.resize(new_content_dims, state.core.compositor);
    }

    WL_State& get_state() {return state;}

    PixelBuffer get_pixel_buffer()
    {
        return PixelBuffer {(PX32*)(state.surfaces.get_content_pointer()), 
                                    state.surfaces.content.size.x, 
                                    state.surfaces.content.size.y,
                                    PX32F::ARGB,
                                    PixelBuffer::Top    };
    }

    bool content_surface_is_busy() { return state.surfaces.content.surface_is_busy; }

    Arr<UserInput> get_new_input_events(); // returns a copy all buffered events and clears the wayland buffer    

    bool frame_step(); // renders current frame, process queued events, and returns true if new frame will begin, or false if exit-input detected
    void run(); // transfer process execution into wayland object

    void close()
    {
        state.running = false;
    }
};


