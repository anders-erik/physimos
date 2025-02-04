
#include "ui/UiWorldObject.hh"


namespace UI::WorldObject {


Container::Container(::WorldObject* _worldObject) {
    worldObject = _worldObject;

    id = worldObject->name + "_primitive_head";
    vertRef = UI::VertRef::Bottom;
    setWidth(400);
    setHeight(200);
    initGraphics();
    setX(10);
    setY(10);
}



NameLabel::NameLabel(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    id = worldObject->name + "_name_label";
    vertRef = UI::VertRef::Top;
    initGraphics();
    fontSize = UI::FontSize::f24;
    setString(_worldObject->name);
    setX(10);
    setY(10);
}

XPosition::XPosition(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    id = worldObject->name + "_x_position";
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    std::string x_pos_string = std::to_string(_worldObject->transform->position.x);
    setString("x = " + x_pos_string.substr(0, 5));
    // setX(300);
    // setY(80);
    setX(220);
    setY(80);
}
XPosIncrease::XPosIncrease(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    id = worldObject->name + "_x_pos_increase";
    isHoverable = true;
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("inc");
    setX(320);
    setY(80);
}
UI::Action XPosIncrease::click() {
    worldObject->transform->position.x += 2.0;

    return UI::Action::None;
}
XPosDecrease::XPosDecrease(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    id = worldObject->name + "_x_pos_decrease";
    isHoverable = true;
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("dec");
    setX(360);
    setY(80);
}
UI::Action XPosDecrease::click() {
    worldObject->transform->position.x -= 2.0;

    return UI::Action::None;
}




YPosition::YPosition(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    id = worldObject->name + "_y_position";
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    std::string x_pos_string = std::to_string(_worldObject->transform->position.y);
    setString("y = " + x_pos_string.substr(0, 5));
    setX(220);
    setY(60);
}
YPosIncrease::YPosIncrease(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    id = worldObject->name + "_y_pos_increase";
    isHoverable = true;
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("inc");
    setX(320);
    setY(60);
}
UI::Action YPosIncrease::click() {
    worldObject->transform->position.y += 2.0;

    return UI::Action::None;
}
YPosDecrease::YPosDecrease(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    id = worldObject->name + "_y_pos_decrease";
    isHoverable = true;
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("dec");
    setX(360);
    setY(60);
}
UI::Action YPosDecrease::click() {
    worldObject->transform->position.y -= 2.0;

    return UI::Action::None;
}



ZPosition::ZPosition(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    id = worldObject->name + "_z_position";
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    std::string x_pos_string = std::to_string(_worldObject->transform->position.z);
    setString("z = " + x_pos_string.substr(0, 5));
    setX(220);
    setY(40);
}
ZPosIncrease::ZPosIncrease(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    id = worldObject->name + "_z_pos_increase";
    isHoverable = true;
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("inc");
    setX(320);
    setY(40);
}
UI::Action ZPosIncrease::click() {
    worldObject->transform->position.z += 2.0;

    return UI::Action::None;
}
ZPosDecrease::ZPosDecrease(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    id = worldObject->name + "_z_pos_decrease";
    isHoverable = true;
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("dec");
    setX(360);
    setY(40);
}
UI::Action ZPosDecrease::click() {
    worldObject->transform->position.z -= 2.0;

    return UI::Action::None;
}



ToggleWireframe::ToggleWireframe(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    isHoverable = true;
    id = worldObject->name + "_toggle_wireframe_";
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("Toggle wireframe");
    setX(10);
    setY(10);
}

UI::Action ToggleWireframe::click() {
    // std::cout << "Clicked ToggleWireframe" << std::endl;
    if (worldObject == nullptr) {
        std::cout << "ERROR: Cant toggle wireframe. Bound world object is null. " << std::endl;
        return UI::Action::None;;
    }
    worldObject->toggleWireframe();

    return UI::Action::None;
}




Primitive* newComponent(::WorldObject* _worldObject) {

    std::string _name = _worldObject->name;

    // Container
    Primitive* worldObjectComponentHead = new UI::WorldObject::Container(_worldObject);


    // Name Label
    UI::Primitive* name_label = new UI::WorldObject::NameLabel(_worldObject);
    worldObjectComponentHead->appendChild(name_label);


    // X position
    UI::Primitive* x_position = new UI::WorldObject::XPosition(_worldObject);
    worldObjectComponentHead->appendChild(x_position);
    // X pos. increase
    UI::Primitive* x_pos_increase = new UI::WorldObject::XPosIncrease(_worldObject);
    worldObjectComponentHead->appendChild(x_pos_increase);
    // X pos. decrease
    UI::Primitive* x_pos_decrease = new UI::WorldObject::XPosDecrease(_worldObject);
    worldObjectComponentHead->appendChild(x_pos_decrease);


    // Y pos
    UI::Primitive* y_pos = new UI::WorldObject::YPosition(_worldObject);
    worldObjectComponentHead->appendChild(y_pos);
    // Y pos. increase
    UI::Primitive* y_pos_increase = new UI::WorldObject::YPosIncrease(_worldObject);
    worldObjectComponentHead->appendChild(y_pos_increase);
    // Y pos. decrease
    UI::Primitive* y_pos_decrease = new UI::WorldObject::YPosDecrease(_worldObject);
    worldObjectComponentHead->appendChild(y_pos_decrease);


    // Z pos
    UI::Primitive* z_pos = new UI::WorldObject::ZPosition(_worldObject);
    worldObjectComponentHead->appendChild(z_pos);
    // Z pos. increase
    UI::Primitive* z_pos_increase = new UI::WorldObject::ZPosIncrease(_worldObject);
    worldObjectComponentHead->appendChild(z_pos_increase);
    // Z pos. decrease
    UI::Primitive* Z_pos_decrease = new UI::WorldObject::ZPosDecrease(_worldObject);
    worldObjectComponentHead->appendChild(Z_pos_decrease);


    // Wireframe Toggle
    UI::WorldObject::ToggleWireframe* toggle_wireframe_ = new UI::WorldObject::ToggleWireframe(_worldObject);
    worldObjectComponentHead->appendChild(toggle_wireframe_);

    return worldObjectComponentHead;
}



}
