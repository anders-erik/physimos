
#pragma once


#include "wl-state.hh"



class Wayland
{
public:

    struct client_state state = { 0 }; 
    bool setup_ok = 0;

    Wayland(i2 dims);  
    Wayland();

    struct client_state get_state() { return state; }

    void init(i2 dims);

    Arr<UserInput> get_new_input_events(); // returns a copy all buffered events and clears the wayland buffer
    void update();
    bool frame_step(); // renders current frame, process new events, and returns true if new frame will begin, or false if exit-input detected
    void dispatch() {wl_display_dispatch(state.wl.display);} // process all wayland events
    void main_loop();
    void render();
    void run();
    void close();

    void socket_test();
};


