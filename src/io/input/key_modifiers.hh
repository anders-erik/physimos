#pragma once

#include "lib/print.hh"


struct KeyModifiers
{
    bool shift = false;
    bool control = false;
    bool alt = false;


    void print()
    {
        Print::buf("KeyModifierState = ");
        if(shift)
            Print::buf("shift ");
        if(control)
            Print::buf("control ");
        if(alt)
            Print::buf("alt ");
        Print::buf("\n");
    }
};