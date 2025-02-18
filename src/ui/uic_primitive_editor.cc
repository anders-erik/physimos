
#include "uic_primitive_editor.hh"

#include "lib/lib.hh"

namespace UI::component {

UIC_PrimitiveEditor::UIC_PrimitiveEditor(::UI::Primitive& _primitive) 
    :   boundObject { _primitive },
        title       { PrimitiveString("Primitive Editor") },
        idPrimitive { PrimitiveString(_primitive.id) }
{
    initGraphics();
    appendChild(&title);
    // TODO: BUG : setting font size after string has no effect!
    title.fontSize = FontSize::f15;
    title.set_y("^10x");
    appendChild(&idPrimitive);
    // TODO: BUG : setting font size after string has no effect!
    idPrimitive.fontSize = FontSize::f15;
    idPrimitive.set_y("^30x");
}

void UIC_PrimitiveEditor::update_component() {

    // title.update(boundObject.);
    idPrimitive.update_str(boundObject.id);
}


void UIC_PrimitiveEditor::render_component(){
    render();

    title.render();
    idPrimitive.render();
}


}
