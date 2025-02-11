
#include "uic_transform.hh"


namespace UI::component::transform {

int xpos_y_input = 120;

Container::Container(TransformComponent* _context) {
    component = _context;

    id = "transform_container";
    uiTransform.vertRef = UI::VertRef::Bottom;
    setWidth(200);
    setHeight(150);
    initGraphics();
    setDefaultColor({0,0,0,255});
    setXrecursive(500);
    setYrecursive(10);
}


XPosition::XPosition(TransformComponent* _context) {
    component = _context;
    ::Transform* transform = ((TransformComponent*)component)->boundObject;
    id = "x_pos_label";
    uiTransform.vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    std::string x_pos_string = std::to_string(transform->position.data[0]);
    setString("x = " + x_pos_string.substr(0, 5));
    setXrecursive(10);
    setYrecursive(xpos_y_input);
}
void XPosition::reload() {
    ::Transform* transform = ((TransformComponent*)component)->boundObject;
    std::string x_pos_string = std::to_string(transform->position.data[0]);
    setString("x = " + x_pos_string.substr(0, 5));
}


XPosIncrease::XPosIncrease(TransformComponent* _context) {
    component = _context;

    id = "x_pos_increase";
    isHoverable = true;
    uiTransform.vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("inc");
    setXrecursive(120);
    setYrecursive(xpos_y_input);
}
UI::Action XPosIncrease::click() {
    ::Transform* transform = ((TransformComponent*)component)->boundObject;
    transform->position.data[0] += 2.0;

    ((TransformComponent*)component)->reloadComponent();
    return UI::Action::ReloadPObject;
}


XPosDecrease::XPosDecrease(TransformComponent* _context) {
    component = _context;

    id = "x_pos_decrease";
    isHoverable = true;
    uiTransform.vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("dec");
    setXrecursive(160);
    setYrecursive(xpos_y_input);
}
UI::Action XPosDecrease::click() {
    ::Transform* transform = ((TransformComponent*)component)->boundObject;

    transform->position.data[0] -= 2.0;

    ((TransformComponent*)component)->reloadComponent();
    return UI::Action::ReloadPObject;
}






void TransformComponent::populateContext(::Transform* _transform) {
    boundObject = _transform;
    
    // Root primitive
    container = new UI::component::transform::Container(this);

    // X position
    xPosition = new UI::component::transform::XPosition(this);
    xPosition->component = this;
    container->appendChild(xPosition);
    // X pos. increase
    xPosIncrease = new UI::component::transform::XPosIncrease(this);
    xPosIncrease->component = this;
    container->appendChild(xPosIncrease);
    // X pos. decrease
    xPosDecrease = new UI::component::transform::XPosDecrease(this);
    xPosDecrease->component = this;
    container->appendChild(xPosDecrease);


}


void TransformComponent::newTransform(::Transform* _transform) {
    boundObject = _transform;
}

// Reloads components that track data
void TransformComponent::reloadComponent() {
    xPosition->reload();
}




}
