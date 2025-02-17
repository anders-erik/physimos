
#include "uic_primitive_editor.hh"

#include "lib/lib.hh"

namespace UI::component {



PrimitiveEditor_Container::PrimitiveEditor_Container(Component_PrimitiveEditor& _primitiveComponent) :
    BasePrimitive(_primitiveComponent)
{

    // TODO: BUG
    uiTransform.horiRef = HoriRef::Right;
    setXrecursive(100);
    setYrecursive(100);
    setWidth(250);
    setHeight(600);
    initGraphics();

}

PrimitiveEditor_Title::PrimitiveEditor_Title(Component_PrimitiveEditor& _primitiveComponent)
    : BasePrimitive(_primitiveComponent) 
{
    initGraphics();
    setString(title_text);
    uiTransform.vertRef = VertRef::Top;
}



IdPrimitive::IdPrimitive(Component_PrimitiveEditor& _primitiveComponent) 
    : BasePrimitive(_primitiveComponent) {

        // reload();
        // initGraphics()

    fontSize = FontSize::f15;
    initGraphics();
    setString(primitiveComponent.boundObject.id);
    uiTransform.vertRef = VertRef::Top;
    setYrecursive(30);
}

void IdPrimitive::reload(){
    id = primitiveComponent.boundObject.id;
}


// Reloads components that track data
// void PrimitiveComponent::reloadComponent() {
//     // idPrimitive.reload();
// }
void Component_PrimitiveEditor::newPrimitive(::UI::Primitive& ui_primitive) {
    boundObject = ui_primitive;
}

// void PrimitiveComponent::init() {
//     // container = UI::component::ContainerPrimitive(*this);
// }

// PrimitiveComponent::PrimitiveComponent(::UI::Primitive& ui_primitive) {
//     // boundObject = ui_primitive;

//     // container = ContainerPrimitive(*this);

// };

Component_PrimitiveEditor::Component_PrimitiveEditor(::UI::Primitive& ui_primitive) :
    boundObject { ui_primitive },
    container   { *this },
    title       { *this },
    idPrimitive { *this }
{
    container.appendChild(&title);
    container.appendChild(&idPrimitive);

};


}
