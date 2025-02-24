#include <iostream>

#include "glad/glad.h"
#include "uic_primitive_list_editor.hh"

#include "ui/ui_globals.hh"

#include "lib/lib.hh"

namespace UI::component {


UIC_PrimitiveListEditor_add::UIC_PrimitiveListEditor_add() {
    id = "prim_list_editor_add";
    set_w("20x");
    set_h("20x");
    set_color({0.0, 0.0, 1.0, 1.0});
}
UiResult UIC_PrimitiveListEditor_add::click() {
    UIC_PrimitiveListEditor* uic_PrimitiveListEditor = (UIC_PrimitiveListEditor*)this->parent;

    // std::cout << "ADD CLICKED" << std::endl;
    uic_PrimitiveListEditor->boundObject.new_list_object();
    
    // int rand_width = std::rand() % 300;
    // uic_PrimitiveColor->boundObject.set_w(std::to_string(rand_width) + "x");

    // uic_PrimitiveColor->boundObject.set_color_texture(ColorTexture::Red);
    // uic_PrimitiveListEditor->boundObject.set_color({1.0, 0.0, 0.0, 1.0});
    return UiResult(true, Action::None, this);
}



UIC_PrimitiveListEditor::UIC_PrimitiveListEditor(::UI::component::UIC_PrimitiveList& _primitive_list, ::UI::Primitive& _primitive_for_list_to_bind) 
    :   boundObject         { _primitive_list },
        title               { "Primitive List Editor" },
        add_btn             { UIC_PrimitiveListEditor_add( ) }
        // add_btn             { UIC_Button( ) }
        // uic_primitive_list  { UIC_PrimitiveList(_primitive) }
{
    id = "prim_list_editor";
    set_color(active_pallete.base1);

    appendChild(&title);
    title.str_setFontSize(FontSize::f24);
    title.set_y("^0x");

    appendChild(&add_btn);
    add_btn.set_y("^40x");
    add_btn.set_x("<40x");


    // UNABLE TO BIND THE NON STATIC METHOD!
    // TODO: Maybe add static methods where I pass the specific type of component? (Essentially c-style programming!)

    // boundObject.new_list_object();
    // boundObject.new_list_object()

    // std::cout << "" <<  << std::endl;
    
    // add_btn.set_click_callback(boundObject.new_list_object);
    // _primitive_list.new_list_object()

}

void UIC_PrimitiveListEditor::update_component() {

    title.update_str("Primitive Editor"); // Should have no effect! 

}


void UIC_PrimitiveListEditor::render_component(){

    // STENCIL TEST TESTING
    if(stencil_test)
        color_shader->stencil_start_new_capture();

    render();

    // STENCIL TEST TESTING
    if(stencil_test)
        color_shader->stencil_apply();


    title.render();

    add_btn.render();

    // STENCIL TEST TESTING
    if(stencil_test)
        color_shader->stencil_disable();
}


UiResult UIC_PrimitiveListEditor::try_find_target_component(double x, double y) {
    // Return if point is not contain within the root component primitive
    if (!containsPoint(x, y))
        return UiResult();

    if (add_btn.containsPoint(x, y))
        return UiResult(true, Action::None, &add_btn);

    
    return UiResult(true, Action::None, this);
    
}


}
