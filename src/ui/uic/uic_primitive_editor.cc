#include "glad/glad.h"
#include "uic_primitive_editor.hh"

#include "ui/ui_globals.hh"

#include "lib/lib.hh"

namespace UI::component {

UIC_PrimitiveEditor::UIC_PrimitiveEditor(::UI::Primitive& _primitive) 
    :   boundObject         { _primitive },
        title               { Primitive("Primitive Editor") },
        id_string           { Primitive(_primitive.id) },
        uic_primitive_color { UIC_PrimitiveColor(_primitive) },
        uic_primitive_position  { UIC_PrimitivePosition(_primitive)},
        uic_primitive_size  { UIC_PrimitiveSize(_primitive)}
{
    id = "primitive_editor";
    set_h("400x");
    set_w("96%");
    set_x("|");
    set_color(active_pallete.base1);


    appendChild(&title);
    title.str_setFontSize(FontSize::f32);
    title.set_y("^0x");

    appendChild(&id_string);
    id_string.str_setFontSize(FontSize::f15);
    id_string.set_y("^30x");

    appendChild(&uic_primitive_color);
    uic_primitive_color.set_y("^80x");

    appendChild(&uic_primitive_position);
    uic_primitive_position.set_y("^130x");

    appendChild(&uic_primitive_size);
    uic_primitive_size.set_y("^230x");
    uic_primitive_size.set_x("|");
}

void UIC_PrimitiveEditor::update_component() {

    title.update_str("Primitive Editor"); // Should have no effect! 
    id_string.update_str(boundObject.id);

}


void UIC_PrimitiveEditor::render_component(){

    // STENCIL TEST TESTING
    if(stencil_test)
        color_shader->stencil_start_new_capture();

    render();

    // STENCIL TEST TESTING
    if(stencil_test)
        color_shader->stencil_apply();


    title.render();
    id_string.render();
    uic_primitive_color.render_component();
    uic_primitive_position.render_component();
    uic_primitive_size.render_component();

    // STENCIL TEST TESTING
    if(stencil_test)
        color_shader->stencil_disable();
}


UiResult UIC_PrimitiveEditor::try_find_target_component(double x, double y) {
    // Return if point is not contain within the root component primitive
    if (!containsPoint(x, y))
        return UiResult();

    if (title.containsPoint(x, y))
        return UiResult(true, CAction::None, &title);

    if (id_string.containsPoint(x, y))
        return UiResult(true, CAction::None, &id_string);
    
    UiResult uiResult = uic_primitive_color.try_find_target_component(x, y);
    if (uiResult.success)
        return uiResult;

    UiResult prim_pos_query = uic_primitive_position.try_find_target_component(x, y);
    if (prim_pos_query.success)
        return prim_pos_query;

    UiResult prim_size_query = uic_primitive_size.try_find_target_component(x, y);
    if (prim_size_query.success)
        return prim_size_query;
    
    return UiResult(true, CAction::None, this);
    
}


}
