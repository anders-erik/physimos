#ifndef UIC_PRIMITIVE_SIZE_HH
#define UIC_PRIMITIVE_SIZE_HH


#include "ui/ui_primitive.hh"

#include "ui/ui_globals.hh"


namespace UI::component {



struct UIC_PrimitiveSize_dec_width : public ::UI::Primitive {
    UiResult click();
    UIC_PrimitiveSize_dec_width();
};
struct UIC_PrimitiveSize_inc_width : public ::UI::Primitive {
    UiResult click();
    UIC_PrimitiveSize_inc_width();
};
struct UIC_PrimitiveSize_dec_height : public ::UI::Primitive {
    UiResult click();
    UIC_PrimitiveSize_dec_height();
};
struct UIC_PrimitiveSize_inc_height : public ::UI::Primitive {
    UiResult click();
    UIC_PrimitiveSize_inc_height();
};



struct UIC_PrimitiveSize_drag : public ::UI::Primitive {
    double x_accum = 0.0;
    double y_accum = 0.0;
    UiResult grabbed(double dx, double dy);
    UIC_PrimitiveSize_drag();
};


struct UIC_PrimitiveSize : public ::UI::Primitive {

    ::UI::Primitive& boundObject;

    Primitive title;

    UIC_PrimitiveSize_dec_width dec_width;
    UIC_PrimitiveSize_inc_width inc_width;
    UIC_PrimitiveSize_dec_height dec_height;
    UIC_PrimitiveSize_inc_height inc_height;

    UIC_PrimitiveSize_drag      drag;

    UiResult    try_find_target_component(double x, double y);
    void        render_component();
    
    UIC_PrimitiveSize(::UI::Primitive& _primitive);
};



}


#endif