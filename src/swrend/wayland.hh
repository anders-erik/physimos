
#pragma once


#include "wl-state.hh"



class Wayland
{
    struct client_state state = { 0 };

public:
    bool setup_ok = 0;


    Wayland();


    void run();

    void socket_test();
};


