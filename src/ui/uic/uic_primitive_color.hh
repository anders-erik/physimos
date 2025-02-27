#ifndef UIC_PRIMITIVE_COLOR_HH
#define UIC_PRIMITIVE_COLOR_HH


#include "ui/ui_primitive.hh"

#include "ui/ui_globals.hh"


namespace UI::component {


struct UIC_PrimitiveColor_SetGreen : public ::UI::Primitive {
    UiResult click();
    GrabState grabbed(double dx, double dy);
    UiResult scroll(double y_change);
    HoverEvent hover_enter();
    HoverEvent hover_exit();
    UIC_PrimitiveColor_SetGreen();
    UIC_PrimitiveColor_SetGreen(Primitive *_parent);
};
struct UIC_PrimitiveColor_SetRed : public ::UI::Primitive {
    UiResult click();
    UIC_PrimitiveColor_SetRed();
    UIC_PrimitiveColor_SetRed(Primitive *_parent);
};


struct UIC_PrimitiveColor : public ::UI::Primitive {

    ::UI::Primitive& boundObject;

    Primitive title;

    UIC_PrimitiveColor_SetGreen set_green_btn;
    UIC_PrimitiveColor_SetRed   set_red_btn;

    /** NOTE: Antipattern during dynamically created components!
     *  Makes sure that the parent and children pointers are pointing to the actual object within the component.
     *  I called immediately after object creation.
     *  Used in cases where I believe the vector is copied during the component creation process, resulting in the original vector going out of scope and the pointers are overwritten.
     */
    void reload_pointers(Primitive* _parent);

    UiResult try_find_target_component(double x, double y);
    void render_component();
    
    UIC_PrimitiveColor(::UI::Primitive& _primitive);
    UIC_PrimitiveColor(Primitive *_parent, ::UI::Primitive& _primitive);
};



}


#endif