
#pragma once

#include "lib/pair.hh"

#include "ui/pui_event.hh"

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

public:

    Widget() = default;

    /** 
        Check is point is within widget box or is currently grabbing cursor. 
        Stores the cursor position for events. */
    bool has_cursor(f2 cursor_sane);

};



}