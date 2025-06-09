
#pragma once

#include "lib/pair.hh"

#include "ui/render/renderer_base.hh"

#include "ui/string.hh"

#include "scene/manager.hh"
#include "scene/scene2D.hh"
#include "scene/quadS2D.hh"

#include <functional>

namespace UI {


/** Return status of the event handler of a widget.
    Purpose is to indicate cursor grab/release.
 */
struct EventResult {
    enum Type {
        None,
        Grab,
        Release,
    } type = None;

    EventResult() = default;
    EventResult(Type type): type {type} {};

    inline bool is_none(){return type == None ? true : false;}
    inline bool is_grab(){return type == Grab ? true : false;}
    inline bool is_release(){return type == Release ? true : false;}
};


/** Widget containing only base-objects.  */
struct Widget
{
    f2 cursor_sane; // cursor position at last successful cursor query

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