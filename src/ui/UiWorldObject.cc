
#include "ui/UiWorldObject.hh"


namespace UI::WorldObject {


Container::Container(::WorldObject* _worldObject) {
    worldObject = _worldObject;

    id = worldObject->name + "_primitive_head";
    vertRef = UI::VertRef::Bottom;
    setWidth(400);
    setHeight(100);
    initGraphics();
    setX(300);
    setY(10);
}



NameLabel::NameLabel(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    id = worldObject->name + "_name_label";
    vertRef = UI::VertRef::Top;
    initGraphics();
    fontSize = UI::FontSize::f24;
    setString(_worldObject->name);
}

XPosition::XPosition(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    id = worldObject->name + "_x_position";
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    std::string x_pos_string = std::to_string(_worldObject->transform->position.x);
    setString("x = " + x_pos_string.substr(0, 5));
    setX(300);
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
    setX(360);
    setY(80);
}
void XPosIncrease::click(){
    worldObject->transform->position.x += 2.0;
}
XPosDecrease::XPosDecrease(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    id = worldObject->name + "_x_pos_decrease";
    isHoverable = true;
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("dec");
}
void XPosDecrease::click() {
    worldObject->transform->position.x -= 2.0;
}

YPosition::YPosition(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    id = worldObject->name + "_y_position";
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    std::string x_pos_string = std::to_string(_worldObject->transform->position.y);
    setString("y = " + x_pos_string.substr(0, 5));
    setX(300);
    setY(60);
}


ZPosition::ZPosition(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    id = worldObject->name + "_z_position";
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    std::string x_pos_string = std::to_string(_worldObject->transform->position.z);
    setString("z = " + x_pos_string.substr(0, 5));
    setX(300);
    setY(40);
}


ToggleWireframe::ToggleWireframe(::WorldObject* _worldObject) {
    worldObject = _worldObject;
    isHoverable = true;
    id = worldObject->name + "_toggle_wireframe_";
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("Toggle wireframe");
}

void ToggleWireframe::click() {
    // std::cout << "Clicked ToggleWireframe" << std::endl;
    if (worldObject == nullptr) {
        std::cout << "ERROR: Cant toggle wireframe. Bound world object is null. " << std::endl;
        return;
    }
    worldObject->toggleWireframe();
}




Primitive* newComponent(::WorldObject* _worldObject) {

    std::string _name = _worldObject->name;

    // Container
    Primitive* worldObjectComponentHead = new UI::WorldObject::Container(_worldObject);


    // Name Label
    UI::Primitive* name_label = new UI::WorldObject::NameLabel(_worldObject);
    worldObjectComponentHead->appendChild(name_label);
    name_label->setX(10);
    name_label->setY(10);


    // X position
    UI::Primitive* x_position = new UI::WorldObject::XPosition(_worldObject);
    worldObjectComponentHead->appendChild(x_position);
    x_position->setX(220);
    x_position->setY(80);
    // X pos. increase
    UI::Primitive* x_pos_increase = new UI::WorldObject::XPosIncrease(_worldObject);
    worldObjectComponentHead->appendChild(x_pos_increase);
    x_pos_increase->setX(320);
    x_pos_increase->setY(80);
    // X pos. decrease
    UI::Primitive* x_pos_decrease = new UI::WorldObject::XPosDecrease(_worldObject);
    worldObjectComponentHead->appendChild(x_pos_decrease);
    x_pos_decrease->setX(360);
    x_pos_decrease->setY(80);

    // Y pos
    UI::Primitive* y_pos = new UI::WorldObject::YPosition(_worldObject);
    worldObjectComponentHead->appendChild(y_pos);
    y_pos->setX(260);
    y_pos->setY(60);

    // Z pos
    UI::Primitive* z_pos = new UI::WorldObject::ZPosition(_worldObject);
    worldObjectComponentHead->appendChild(z_pos);
    z_pos->setX(260);
    z_pos->setY(40);


    // Wireframe Toggle
    UI::WorldObject::ToggleWireframe* toggle_wireframe_ = new UI::WorldObject::ToggleWireframe(_worldObject);
    worldObjectComponentHead->appendChild(toggle_wireframe_);
    // Necessary to place relative to parent
    toggle_wireframe_->setX(10);
    toggle_wireframe_->setY(10);

    return worldObjectComponentHead;
}



}
