
#pragma once


#include "wl-state.hh"



class Wayland
{
    struct client_state state = { 0 }; 

public:
    bool setup_ok = 0;

    Wayland(i2 dims);  
    Wayland();

    struct client_state get_state() { return state; }

    void init(i2 dims);

    Arr<WEvent> process_events(); // returns a copy all buffered events and clears the wayland buffer
    void update();
    void main_loop();
    void render();
    void run();
    void close();

    void socket_test();
};


