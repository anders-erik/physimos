#ifndef UIC_PRIMITIVE_COLOR_HH
#define UIC_PRIMITIVE_COLOR_HH


#include "ui/ui_primitive.hh"

#include "ui/ui_globals.hh"


namespace UI::component {


struct UIC_PrimitiveColor_SetGreen : public ::UI::Primitive {
    UiResult click();
    UiResult grabbed(double dx, double dy);
    UiResult scroll(double y_change);
    UiResult hover_enter();
    UiResult hover_exit();
    UIC_PrimitiveColor_SetGreen();
};
struct UIC_PrimitiveColor_SetRed : public ::UI::Primitive {
    UiResult click();
    UIC_PrimitiveColor_SetRed();
};


struct UIC_PrimitiveColor : public ::UI::Primitive {

    ::UI::Primitive& boundObject;

    PrimitiveString title;

    UIC_PrimitiveColor_SetGreen set_green_btn;
    UIC_PrimitiveColor_SetRed set_red_btn;

    UiResult try_find_target_component(double x, double y);
    void render_component();
    
    UIC_PrimitiveColor(::UI::Primitive& _primitive);
};



}


#endif