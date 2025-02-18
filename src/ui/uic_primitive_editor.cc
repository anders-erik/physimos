
#include "uic_primitive_editor.hh"

#include "lib/lib.hh"

namespace UI::component {

UIC_PrimitiveEditor::UIC_PrimitiveEditor(::UI::Primitive& _primitive) 
    :   boundObject { _primitive },
        title       { PrimitiveString("Primitive Editor") },
        id_string { PrimitiveString(_primitive.id) }
{
    initGraphics();

    appendChild(&title);
    title.str_setFontSize(FontSize::f15);
    title.set_y("^10x");

    appendChild(&id_string);
    id_string.str_setFontSize(FontSize::f15);
    id_string.set_y("^30x");
}

void UIC_PrimitiveEditor::update_component() {

    title.update_str("Primitive Editor"); // Should have no effect! 
    id_string.update_str(boundObject.id);

}


void UIC_PrimitiveEditor::render_component(){
    render();

    title.render();
    id_string.render();
}

bool UIC_PrimitiveEditor::try_hover_component(double x, double y){
    if (containsPoint(x, y) && isHoverable)
        return true;
    
    return false;
}


}
