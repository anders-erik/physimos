#pragma once


#include "wl-state.hh"


class Wayland
{
    WaylandState state = { 0 }; 

    void init(i2 dims);
    void render(); // damage buffer and commit surface

public:

    Wayland(i2 dims);  
    Wayland();

    uint32_t* get_framebuffer_ptr() { return state.fb.data; }
    int get_framebuffer_width() {return state.fb.w; }
    int get_framebuffer_height() {return state.fb.h; }

    Arr<UserInput> get_new_input_events(); // returns a copy all buffered events and clears the wayland buffer    

    bool frame_step(); // renders current frame, process queued events, and returns true if new frame will begin, or false if exit-input detected
    void run(); // transfer process execution into wayland object

    void close(); // destroy framebuffer & disconnect from compositor

};


