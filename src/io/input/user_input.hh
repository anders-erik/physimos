
#pragma once


#include "math/vecmat.hh"


#include "mouse_input.hh"
#include "key_input.hh"


enum class UserInputType
{
    MouseMove,
    MouseClick,
    KeyPress
};


union UserInputData
{
    MouseMovement move;
    MouseClick mouse_click;
    KeyPress key_press;

    UserInputData() {}
    UserInputData(MouseMovement _move) : move{_move} {}
    UserInputData(MouseClick _mouse_click) : mouse_click{_mouse_click} {}
    UserInputData(KeyPress _key_press) : key_press {_key_press} {}
};

struct UserInput
{
    UserInputType event_type;
    UserInputData event_data;

    UserInput() {}
    UserInput(UserInputType _event_type, UserInputData _event_data) 
        :   event_type {_event_type},
            event_data {_event_data}
    {}

    bool is_mouse_move() { return (event_type == UserInputType::MouseMove) ? true : false; }
    bool is_mouse_click() { return (event_type == UserInputType::MouseClick) ? true : false; }
    bool is_key_press() { return (event_type == UserInputType::KeyPress) ? true : false; }
};
