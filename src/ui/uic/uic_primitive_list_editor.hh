#ifndef UIC_PRIMITIVE_LIST_EDITOR_HH
#define UIC_PRIMITIVE_LIST_EDITOR_HH


#include "uic_primitive_color.hh"
#include "uic_primitive_position.hh"
#include "uic_primitive_size.hh"

#include "uic_primitive_list.hh"

#include "uic_button.hh"

#include "ui/ui_primitive.hh"


namespace UI::component {

// class Component_PrimitiveEditor;

struct UIC_PrimitiveListEditor_add: public ::UI::Primitive {
    UiResult click();
    UIC_PrimitiveListEditor_add();
};


struct UIC_PrimitiveListEditor : public ::UI::Primitive {

    ::UI::component::UIC_PrimitiveList& boundObject; // PRIMITIVE LIST

    ::UI::Primitive primitive_for_list_to_bind;

    Primitive title;

    UIC_PrimitiveListEditor_add add_btn;
    // UIC_Button del_btn;

    // UIC_PrimitiveList uic_primitive_list;


    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_PrimitiveListEditor(::UI::component::UIC_PrimitiveList& _primitive_list, ::UI::Primitive& _primitive_for_list_to_bind);
};




}


#endif