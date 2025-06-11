#pragma once


namespace window {


enum class KeyModifierState : unsigned char {
    alt_ctl_shift   = 7,
    alt_ctrl        = 6,
    alt_shift       = 5,
    alt             = 4,
    ctl_shift       = 3,
    ctl             = 2,
    shift           = 1,
    none            = 0,
};

typedef unsigned char ModifierBits;
#define SHIFT           0x1;
#define CTRL            0x2;
#define CTRL_SHIFT      0x3;
#define ALT             0x4;
#define ALT_SHIFT       0x5;
#define ALT_CTRL        0x6;
#define ALT_CTRL_SHIFT  0x7;


struct KeyModifiers 
{
    ModifierBits state = 0;

    KeyModifiers() = default;
    KeyModifiers(ModifierBits state): state {state} {};

    bool shift_is_down()    { return state & SHIFT; }
    bool ctrl_is_down()     { return state & CTRL; }
    bool alt_is_down()      { return state & ALT; }

    void set_shift()        { state |= SHIFT; }
    void unset_shift()      { if(shift_is_down()) state ^= SHIFT; }

    void set_ctrl()         { state |= CTRL; }
    void unset_ctrl()       { if(ctrl_is_down()) state ^= CTRL; }

    void set_alt()          { state |= ALT; }
    void unset_alt()        { if(alt_is_down()) state ^= ALT; }

    bool is_none()          { return state == 0             ; }
    bool is_shift()         { return state == SHIFT         ; }
    bool is_ctrl()          { return state == CTRL          ; }
    bool is_ctrl_shift()    { return state == CTRL_SHIFT    ; }
    bool is_alt()           { return state == ALT           ; }
    bool is_alt_shift()     { return state == ALT_SHIFT     ; }
    bool is_alt_ctrl()      { return state == ALT_CTRL      ; }
    bool is_alt_ctrl_shift(){ return state == ALT_CTRL_SHIFT; }
};


enum class Key {
    None,
    Esc,
    p,
};


Key get_key_from_glfw_key(int _glfw_key);


}

