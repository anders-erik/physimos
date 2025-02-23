#ifndef UIC_PRIMITIVE_POSITION_HH
#define UIC_PRIMITIVE_POSITION_HH


#include "ui/ui_primitive.hh"

#include "ui/ui_globals.hh"


namespace UI::component {


struct UIC_PrimitivePosition_up : public ::UI::Primitive {
    UiResult click();
    UIC_PrimitivePosition_up();
};
struct UIC_PrimitivePosition_down : public ::UI::Primitive {
    UiResult click();
    UIC_PrimitivePosition_down();
};
struct UIC_PrimitivePosition_left : public ::UI::Primitive {
    UiResult click();
    UIC_PrimitivePosition_left();
};
struct UIC_PrimitivePosition_right : public ::UI::Primitive {
    UiResult click();
    UIC_PrimitivePosition_right();
};
struct UIC_PrimitivePosition_scroll : public ::UI::Primitive {
    UiResult scroll(double y_change);
    UIC_PrimitivePosition_scroll();
};
struct UIC_PrimitivePosition_drag : public ::UI::Primitive {
    double x_accum = 0.0;
    double y_accum = 0.0;
    UiResult grabbed(double dx, double dy);
    UIC_PrimitivePosition_drag();
};


struct UIC_PrimitivePosition : public ::UI::Primitive {

    ::UI::Primitive& boundObject;

    Primitive title;

    UIC_PrimitivePosition_up        up_btn;
    UIC_PrimitivePosition_down      down_btn;
    UIC_PrimitivePosition_left      left_btn;
    UIC_PrimitivePosition_right     right_btn;

    UIC_PrimitivePosition_scroll    scroll;
    UIC_PrimitivePosition_drag      drag;

    UiResult    try_find_target_component(double x, double y);
    void        render_component();
    
    UIC_PrimitivePosition(::UI::Primitive& _primitive);
};



}


#endif