#ifndef UIC_PRIMITIVE_EDITOR_HH
#define UIC_PRIMITIVE_EDITOR_HH


#include "uic_primitive_color.hh"
#include "uic_primitive_position.hh"
#include "uic_primitive_size.hh"

#include "ui/ui_primitive.hh"


namespace UI::component {

// class Component_PrimitiveEditor;


struct UIC_PrimitiveEditor : public ::UI::Primitive {

    ::UI::Primitive& boundObject;

    PrimitiveString title;
    PrimitiveString id_string;

    UIC_PrimitiveColor uic_primitive_color;
    UIC_PrimitivePosition uic_primitive_position;
    UIC_PrimitiveSize uic_primitive_size;


    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_PrimitiveEditor(::UI::Primitive& _primitive);
};




}


#endif