
#include "uic_primitive.hh"

#include "lib/lib.hh"

namespace UI::component {

int xpos_y_input = 120;

// ContainerPrimitive::ContainerPrimitive(PrimitiveComponent& _primitiveComponent) {
//     primitiveComponent = _primitiveComponent;

// }


// IdPrimitive::IdPrimitive(PrimitiveComponent& primitiveComponent) {
//     component = primitiveComponent;

// }
// void IdPrimitive::reload() {
//     // plog()
//     plog_info(LogScope::UI, "RELOADING ID PRIMITIVE.");
    
//     // ::Transform* transform = ((TransformComponent*)component)->boundObject;
//     // std::string x_pos_string = std::to_string(transform->position.data[0]);
//     // setString("x = " + x_pos_string.substr(0, 5));
// }





// void PrimitiveComponent::populateComponent(::UI::Primitive& ui_primitive) {
//     boundObject = ui_primitive;
    
//     // Root primitive
//     // container = UI::component::ContainerPrimitive(*this);

//     // X position
//     // idPrimitive = new UI::component::IdPrimitive(this);
//     // xPosition->component = this;
//     // container->appendChild(xPosition);


// }


// void PrimitiveComponent::newPrimitive(::UI::Primitive& _transform) {
//     boundObject = _transform;
// }

// ContainerPrimitive::ContainerPrimitive(PrimitiveComponent& _primitiveComponent) {

// }


// Reloads components that track data
// void PrimitiveComponent::reloadComponent() {
//     // idPrimitive.reload();
// }
void PrimitiveComponent::newPrimitive(::UI::Primitive& ui_primitive) {
    boundObject = ui_primitive;
}

// void PrimitiveComponent::init() {
//     // container = UI::component::ContainerPrimitive(*this);
// }

// PrimitiveComponent::PrimitiveComponent(::UI::Primitive& ui_primitive) {
//     // boundObject = ui_primitive;

//     // container = ContainerPrimitive(*this);

// };



}
