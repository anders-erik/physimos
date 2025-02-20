#ifndef UIC_PRIMITIVE_SIZE_HH
#define UIC_PRIMITIVE_SIZE_HH


#include "ui/ui_primitive.hh"

#include "ui/ui_globals.hh"


namespace UI::component {


struct UIC_PrimitiveSize_drag : public ::UI::Primitive {
    double x_accum = 0.0;
    double y_accum = 0.0;
    UiResult grabbed(double dx, double dy);
    UIC_PrimitiveSize_drag();
};


struct UIC_PrimitiveSize : public ::UI::Primitive {

    ::UI::Primitive& boundObject;

    PrimitiveString title;

    UIC_PrimitiveSize_drag      drag;

    UiResult    try_find_target_component(double x, double y);
    void        render_component();
    
    UIC_PrimitiveSize(::UI::Primitive& _primitive);
};



}


#endif