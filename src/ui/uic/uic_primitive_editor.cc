
#include "uic_primitive_editor.hh"

#include "lib/lib.hh"

namespace UI::component {

UIC_PrimitiveEditor::UIC_PrimitiveEditor(::UI::Primitive& _primitive) 
    :   boundObject         { _primitive },
        title               { PrimitiveString("Primitive Editor") },
        id_string           { PrimitiveString(_primitive.id) },
        uic_primitive_color { UIC_PrimitiveColor(_primitive) }
{
    isHoverable = true;
    // initGraphics();

    appendChild(&title);
    title.str_setFontSize(FontSize::f15);
    title.set_y("^10x");

    appendChild(&id_string);
    id_string.str_setFontSize(FontSize::f15);
    id_string.set_y("^30x");

    appendChild(&uic_primitive_color);
    uic_primitive_color.set_y("^50x");
}

void UIC_PrimitiveEditor::update_component() {

    title.update_str("Primitive Editor"); // Should have no effect! 
    id_string.update_str(boundObject.id);

}


void UIC_PrimitiveEditor::render_component(){
    render();

    title.render();
    id_string.render();
    uic_primitive_color.render_component();
}


UiResult UIC_PrimitiveEditor::try_find_target_component(double x, double y) {
    // Return if point is not contain within the root component primitive
    if (!containsPoint(x, y))
        return UiResult();

    if (title.containsPoint(x, y))
        return UiResult(true, Action::None, &title);

    if (id_string.containsPoint(x, y))
        return UiResult(true, Action::None, &id_string);
    
    UiResult uiResult = uic_primitive_color.try_find_target_component(x, y);
    if (uiResult.success)
        return uiResult;
    
    return UiResult(true, Action::None, this);
    
}


}
