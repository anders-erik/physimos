
#pragma once

#include "math/vecmat.hh"


enum class WEventType
{
    MouseMove,
    MouseClick,
};

enum class WEventButtonAction
{
    Press,
    Release,
    Hold,
};

struct WMouseMove
{
    WEventType event_type = WEventType::MouseMove;
    d2 old_pos;
    d2 new_pos;

    WMouseMove(d2 _old_pos, d2 _new_pos) : old_pos {_old_pos}, new_pos {_new_pos} {}

    d2 delta() { return d2{new_pos-old_pos}; }
};


struct WMouseClick
{
    WEventType event_type = WEventType::MouseClick;
    enum Button
    {
        Primary,
        Secondary, 
        Tertiary
    } button;
    WEventButtonAction action;

    WMouseClick() {}
    WMouseClick(WMouseClick::Button _mouse_button, WEventButtonAction _action) : button {_mouse_button}, action {_action} {}
};

union WEventData
{
    WMouseMove move;
    WMouseClick mouse_click;

    WEventData() {}
    WEventData(WMouseMove _move) : move{_move} {}
    WEventData(WMouseClick _mouse_click) : mouse_click{_mouse_click} {}
};

struct WEvent
{
    WEventType event_type;
    WEventData event_data;

    WEvent() {}
    WEvent(WEventType _event_type, WEventData _event_data) 
        :   event_type {_event_type},
            event_data {_event_data}
    {}

};
