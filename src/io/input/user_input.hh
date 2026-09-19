
#pragma once


#include "math/vecmat.hh"


#include "mouse_input.hh"


enum class UserInputType
{
    MouseMove,
    MouseClick,
};


union UserInputData
{
    MouseMovement move;
    MouseClick mouse_click;

    UserInputData() {}
    UserInputData(MouseMovement _move) : move{_move} {}
    UserInputData(MouseClick _mouse_click) : mouse_click{_mouse_click} {}
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

};
