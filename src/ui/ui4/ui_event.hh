
#pragma once


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
    double x;
    double y;
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

    UIEvent() {}
    UIEvent(UIEvent::Type _type)
    {
        type = _type;
    }
};