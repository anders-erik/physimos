
#include "uic_transform.hh"


namespace UI::component::transform {

int xpos_y_input = 120;

Container::Container(TransformContext* _context) {
    componentContext = _context;

    id = "transform_container";
    uiTransform.vertRef = UI::VertRef::Bottom;
    setWidth(200);
    setHeight(150);
    initGraphics();
    setX(500);
    setY(10);
}


XPosition::XPosition(TransformContext* _context) {
    componentContext = _context;
    ::Transform* transform = ((TransformContext*)componentContext)->boundObject;
    id = "x_pos_label";
    uiTransform.vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    std::string x_pos_string = std::to_string(transform->position.data[0]);
    setString("x = " + x_pos_string.substr(0, 5));
    setX(10);
    setY(xpos_y_input);
}
void XPosition::reload() {
    ::Transform* transform = ((TransformContext*)componentContext)->boundObject;
    std::string x_pos_string = std::to_string(transform->position.data[0]);
    setString("x = " + x_pos_string.substr(0, 5));
}


XPosIncrease::XPosIncrease(TransformContext* _context) {
    componentContext = _context;

    id = "x_pos_increase";
    isHoverable = true;
    uiTransform.vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("inc");
    setX(120);
    setY(xpos_y_input);
}
UI::Action XPosIncrease::click() {
    ::Transform* transform = ((TransformContext*)componentContext)->boundObject;
    transform->position.data[0] += 2.0;

    return UI::Action::ReloadPObject;
}


XPosDecrease::XPosDecrease(TransformContext* _context) {
    componentContext = _context;

    id = "x_pos_decrease";
    isHoverable = true;
    uiTransform.vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("dec");
    setX(160);
    setY(xpos_y_input);
}
UI::Action XPosDecrease::click() {
    ::Transform* transform = ((TransformContext*)componentContext)->boundObject;

    transform->position.data[0] -= 2.0;

    return UI::Action::ReloadPObject;
}



void TransformContext::newTransform(::Transform* _transform) {
    boundObject = _transform;
}

// Reloads components that track data
void TransformContext::reloadComponent() {
    xPosition->reload();
}


void TransformContext::populateContext(::Transform* _transform) {
    boundObject = _transform;
    
    container = new UI::component::transform::Container(this);

    // X position
    xPosition = new UI::component::transform::XPosition(this);
    xPosition->componentContext = this;
    container->appendChild(xPosition);
    // X pos. increase
    xPosIncrease = new UI::component::transform::XPosIncrease(this);
    xPosIncrease->componentContext = this;
    container->appendChild(xPosIncrease);
    // X pos. decrease
    xPosDecrease = new UI::component::transform::XPosDecrease(this);
    xPosDecrease->componentContext = this;
    container->appendChild(xPosDecrease);


}




}
