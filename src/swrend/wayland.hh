
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

    void update();
    void main_loop();
    void run();
    void close();

    void socket_test();
};


