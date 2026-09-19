
#pragma once


#include "math/vecmat.hh"



enum class MouseButtonAction
{
    Press,
    Release,
    Hold,
};

struct MouseMovement
{
    d2 old_pos;
    d2 new_pos;

    MouseMovement(d2 _old_pos, d2 _new_pos) : old_pos {_old_pos}, new_pos {_new_pos} {}

    d2 delta() { return d2{new_pos-old_pos}; }
};


struct MouseClick
{
    enum Button
    {
        Primary,
        Secondary, 
        Tertiary
    } button;
    MouseButtonAction action;

    MouseClick() {}
    MouseClick(MouseClick::Button _mouse_button, MouseButtonAction _action) : button {_mouse_button}, action {_action} {}
};


