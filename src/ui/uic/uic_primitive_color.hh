#ifndef UIC_PRIMITIVE_COLOR_HH
#define UIC_PRIMITIVE_COLOR_HH


#include "ui/ui_primitive.hh"

#include "ui/ui_globals.hh"


namespace UI::component {

struct UIC_PrimitiveColor_SetBlack : public ::UI::Primitive {
    UiResult click_new();

    UIC_PrimitiveColor_SetBlack();
};

struct UIC_PrimitiveColor : public ::UI::Primitive {

    ::UI::Primitive& boundObject;

    PrimitiveString title;
    UIC_PrimitiveColor_SetBlack set_black_btn;


    // void update_component();
    void render_component();

    /** Find primitive in component that match passed screen coordinates. Returns success if terget found within component. False otherwise. 
     *  Will only match primitives/components within the bounds of the component itself. 
     */
    // UiResult try_find_target_component(double x, double y);
    UiResult try_find_target_component(double x, double y);
    
    UIC_PrimitiveColor(::UI::Primitive& _primitive);
};




}


#endif