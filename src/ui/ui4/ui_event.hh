
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
    } type;

    union Value
    {
        UIMouseMove mouse_move;
    } value;

    bool target_is_grabbed = false;

    UIEvent() {}
    UIEvent(UIEvent::Type _type)
    {
        type = _type;
    }
};