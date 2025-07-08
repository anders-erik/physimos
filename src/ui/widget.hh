
#pragma once

#include "lib/pair.hh"

#include "window/auxevent.hh"

#include "ui/string.hh"


namespace UI
{


/** Widget containing only base-objects.  */
struct Widget
{
    f2 cursor_sane; // cursor position at last successful cursor query

    // To be included using future Base inheritence
    Box2D frame;
    Base frame_base;

    bool is_grabbing_mouse = false; // A widget need to cache if it is grabbing something to maintain grab

public:

    Widget() = default;

    /** 
        Check is point is within widget box or is currently grabbing cursor. 
        Stores the cursor position for events. */
    bool has_cursor(f2 cursor_sane)
    {
        if(!frame.contains_point(cursor_sane))
            return false;

        this->cursor_sane = cursor_sane;

        return true;
    }

};



}