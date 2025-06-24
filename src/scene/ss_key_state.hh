#pragma once

#include "window/auxevent.hh"
#include "window/key.hh"
#include "window/keymod.hh"

using namespace window;

namespace SS 
{

struct KeyState
{
    Key key_1 = Key::None;
    Key key_2 = Key::None;

    bool inactive()     { return key_1 == Key::None; }
    bool key_1_active()       { return key_1 != Key::None; }
    bool key_2_active()       { return key_2 != Key::None; }
    void clear_1()            { key_1 = Key::None; }
    void clear_2()            { key_2 = Key::None; }
    void clear_all()            { key_1 = Key::None; key_2 = Key::None; }

    /** returns true if keystroke was consumed. */
    bool new_keystroke(KeyStrokeEvent keystroke)
    {
        Key key = keystroke.key;

        switch (key)
        {
        case Key::g:
            break;
        
        default:
            return false; // not consumed
            break;
        }

        if(key_1_active())
            key_2 = key;
        else
            key_1 = key;
        
        return true;
    }
};


}

