#pragma once

#include "math/vecmat.hh"

#include "window/key.hh"



struct CameraState
{
    typedef unsigned int BitType;

    static const BitType NONE        = 0x00;
    static const BitType FORWARD     = 0x01;
    static const BitType BACKWARD    = 0x02;
    static const BitType LEFT        = 0x04;
    static const BitType RIGHT       = 0x08;
    static const BitType UP          = 0x10;
    static const BitType DOWN        = 0x20;

    static const Key ForwardKey    = Key::e;
    static const Key BackwardKey   = Key::d;
    static const Key RightKey      = Key::f;
    static const Key LeftKey       = Key::s;
    static const Key UpKey         = Key::q;
    static const Key DownKey       = Key::a;

    /**  */
    BitType pressed = NONE;
    BitType active = NONE;

    f3 spherical_delta;
    bool grabbing_cursor    = false;
    bool panning            = false;

    void set(BitType new_press)             { pressed |= new_press; }
    void unset(BitType new_release)         { pressed &= ~new_release; }
    bool has(BitType press_type)            { return (pressed & press_type) == press_type; }

    void active_set(BitType new_bits)       { active |= new_bits; }
    void active_unset(BitType new_bits)     { active &= ~new_bits; }
    bool active_is(BitType press_type)      { return (active & press_type) == press_type; }


    // SET - unsets complementary direction if active

    void set_forward()      { set(FORWARD ); active_set(FORWARD ); active_unset(BACKWARD);  }
    void set_backward()     { set(BACKWARD); active_set(BACKWARD); active_unset(FORWARD );  }
    void set_left()         { set(LEFT    ); active_set(LEFT    ); active_unset(RIGHT   );  }
    void set_right()        { set(RIGHT   ); active_set(RIGHT   ); active_unset(LEFT    );  }
    void set_up()           { set(UP      ); active_set(UP      ); active_unset(DOWN    );  }
    void set_down()         { set(DOWN    ); active_set(DOWN    ); active_unset(UP      );  }


    // UNSET - Try activate the complementary direction after unset

    void unset_forward()    { unset(FORWARD ); active_unset(FORWARD ); if(has(BACKWARD)) active_set(BACKWARD );     }
    void unset_backward()   { unset(BACKWARD); active_unset(BACKWARD); if(has(FORWARD )) active_set(FORWARD);     }
    void unset_left()       { unset(LEFT    ); active_unset(LEFT    ); if(has(RIGHT   )) active_set(RIGHT    );     }
    void unset_right()      { unset(RIGHT   ); active_unset(RIGHT   ); if(has(LEFT    )) active_set(LEFT   );     }
    void unset_up()         { unset(UP      ); active_unset(UP      ); if(has(DOWN    )) active_set(DOWN      );     }
    void unset_down()       { unset(DOWN    ); active_unset(DOWN    ); if(has(UP      )) active_set(UP    );     }


    // Is a direction if `has` AND is_active are both true

    bool is_forward()    { return has(FORWARD ) && active_is(FORWARD ); }
    bool is_backward()   { return has(BACKWARD) && active_is(BACKWARD); }
    bool is_left()       { return has(LEFT    ) && active_is(LEFT    ); }
    bool is_right()      { return has(RIGHT   ) && active_is(RIGHT   ); }
    bool is_up()         { return has(UP      ) && active_is(UP      ); }
    bool is_down()       { return has(DOWN    ) && active_is(DOWN    ); }

};




