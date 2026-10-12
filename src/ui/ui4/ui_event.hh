
#pragma once


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
    } type;

    union Value
    {
        UIMouseMove mouse_move;
    } value;

    UIEvent() {}

    UIEvent(UIEvent::Type _type)
    {
        type = _type;
    }
};