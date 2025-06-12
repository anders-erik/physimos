
#pragma once


#include "window/auxevent.hh"


/** Return status of the event handler of a widget.
    Purpose is to indicate cursor grab/release.
 */
struct EventResult {
    enum Type {
        Grab,
        Release,
    } type = Release;

    EventResult() = default;
    EventResult(Type type): type {type} {};

    // inline bool is_none(){return type == None ? true : false;}
    inline bool is_grab(){return type == Grab ? true : false;}
    inline bool is_release(){return type == Release ? true : false;}
};


