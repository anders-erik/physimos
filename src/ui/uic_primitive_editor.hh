#ifndef UIC_PRIMITIVE_EDITOR_HH
#define UIC_PRIMITIVE_EDITOR_HH


#include "ui/ui_component.hh"

#include "ui/ui_primitive.hh"


namespace UI::component {

class Component_PrimitiveEditor;


struct UIC_PrimitiveEditor : public ::UI::Primitive {

    ::UI::Primitive& boundObject;

    PrimitiveString title;
    PrimitiveString id_string;


    void update_component();
    void render_component();

    bool try_hover_component(double x, double y);
    
    UIC_PrimitiveEditor(::UI::Primitive& _primitive);
};




}


#endif