
#pragma once


#include "math/vecmat.hh"


#include "mouse_input.hh"
#include "key_input.hh"
#include "scroll_input.hh"


enum class UserInputType
{
    MouseMove,
    MouseClick,
    KeyInput,
    ScrollInput
};


union UserInputData
{
    MouseMovement move;
    MouseClick mouse_click;
    KeyInput key_input;
    ScrollInput scroll_input;

    UserInputData() {}
    UserInputData(MouseMovement _move) : move{_move} {}
    UserInputData(MouseClick _mouse_click) : mouse_click{_mouse_click} {}
    UserInputData(KeyInput _key_input) : key_input {_key_input} {}
    UserInputData(ScrollInput _scroll_input) : scroll_input {_scroll_input} {}
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
    bool is_key_input() { return (event_type == UserInputType::KeyInput) ? true : false; }
    bool is_scroll_input() { return (event_type == UserInputType::ScrollInput) ? true : false; }
};
