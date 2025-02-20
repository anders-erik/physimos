#ifndef UIC_PRIMITIVE_EDITOR_HH
#define UIC_PRIMITIVE_EDITOR_HH


#include "uic_primitive_color.hh"

#include "ui/ui_primitive.hh"


namespace UI::component {

class Component_PrimitiveEditor;


struct UIC_PrimitiveEditor : public ::UI::Primitive {

    ::UI::Primitive& boundObject;

    PrimitiveString title;
    PrimitiveString id_string;

    UIC_PrimitiveColor uic_primitive_color;


    void update_component();
    void render_component();

    /** Find primitive in component that match passed screen coordinates. Returns success if terget found within component. False otherwise. 
     *  Will only match primitives/components within the bounds of the component itself. 
     */
    UiResult try_find_target_component(double x, double y);
    
    UIC_PrimitiveEditor(::UI::Primitive& _primitive);
};




}


#endif