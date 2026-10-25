
#pragma once

#include "io/input/user_input.hh"

struct UIEventReturn
{
    enum Type
    {
        node_mouse_grab,
        node_mouse_release,
        node_keyboard_grab,
        none,
    } type = Type::none;

    
};

struct UIMouseMove
{
    double dx; // delta x
    double dy; // delta y
    f64 x; // absolute x pos
    f64 y; // absolute y pos
};

struct UIKeyPress
{
    Keys key;
};

struct UIScroll
{
    enum Direction
    {
        Up,
        Down,
    } direction;

    bool is_up() { return direction == Up; }
    bool is_down() { return direction == Down; }
};

struct UIEvent
{
    enum Type
    {
        mouse_move,
        mouse_enter,
        mouse_exit,
        mouse_press,
        mouse_release,
        mouse_scroll,
        key_press,
        key_release,
    } type;

    union Value
    {
        UIMouseMove mouse_move;
        UIKeyPress key_press;
        UIScroll scroll;
    } value;

    bool target_is_grabbed = false;
    
    struct KeyMods
    {
        bool ctrl = false;
        bool shift = false;
        bool alt = false;

        bool is_none()
        {
            return !ctrl && !shift && !alt;
        }

        KeyMods() = default;
        KeyMods(KeyModifiers _key_modifers)
        {
            ctrl = _key_modifers.control;
            shift = _key_modifers.shift;
            alt = _key_modifers.alt;
        }
        
    } key_mods;

    UIEvent() {}
    UIEvent(UIEvent::Type _type)
    {
        type = _type;
    }
};

