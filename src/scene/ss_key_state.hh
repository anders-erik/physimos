#pragma once

#include "window/key.hh"


namespace SS 
{
    inline const Key GRAB_KEY       = Key::g;
    inline const Key ROTATE_KEY     = Key::r;
    inline const Key TRANSLATE_KEY  = Key::t;
    inline const Key SCALE_KEY      = Key::y;

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

};


}

