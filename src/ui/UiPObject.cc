
#include "ui/UiPObject.hh"


namespace UI::PObject {



Container::Container(::WorldObject* _pObject) {
    pObject = _pObject;

    id = pObject->name + "_primitive_head";
    vertRef = UI::VertRef::Bottom;
    setWidth(400);
    setHeight(200);
    initGraphics();
    setX(10);
    setY(10);
}



NameLabel::NameLabel(::WorldObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_name_label";
    vertRef = UI::VertRef::Top;
    initGraphics();
    fontSize = UI::FontSize::f24;
    setString(_pObject->name);
    setX(10);
    setY(10);
}

XPosition::XPosition(::WorldObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_x_position";
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    std::string x_pos_string = std::to_string(_pObject->transform->position.x);
    setString("x = " + x_pos_string.substr(0, 5));
    // setX(300);
    // setY(80);
    setX(220);
    setY(80);
}
XPosIncrease::XPosIncrease(::WorldObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_x_pos_increase";
    isHoverable = true;
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("inc");
    setX(320);
    setY(80);
}
UI::Action XPosIncrease::click() {
    pObject->transform->position.x += 2.0;

    return UI::Action::None;
}
XPosDecrease::XPosDecrease(::WorldObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_x_pos_decrease";
    isHoverable = true;
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("dec");
    setX(360);
    setY(80);
}
UI::Action XPosDecrease::click() {
    pObject->transform->position.x -= 2.0;

    return UI::Action::None;
}




YPosition::YPosition(::WorldObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_y_position";
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    std::string x_pos_string = std::to_string(_pObject->transform->position.y);
    setString("y = " + x_pos_string.substr(0, 5));
    setX(220);
    setY(60);
}
YPosIncrease::YPosIncrease(::WorldObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_y_pos_increase";
    isHoverable = true;
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("inc");
    setX(320);
    setY(60);
}
UI::Action YPosIncrease::click() {
    pObject->transform->position.y += 2.0;

    return UI::Action::None;
}
YPosDecrease::YPosDecrease(::WorldObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_y_pos_decrease";
    isHoverable = true;
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("dec");
    setX(360);
    setY(60);
}
UI::Action YPosDecrease::click() {
    pObject->transform->position.y -= 2.0;

    return UI::Action::None;
}



ZPosition::ZPosition(::WorldObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_z_position";
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    std::string x_pos_string = std::to_string(_pObject->transform->position.z);
    setString("z = " + x_pos_string.substr(0, 5));
    setX(220);
    setY(40);
}
ZPosIncrease::ZPosIncrease(::WorldObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_z_pos_increase";
    isHoverable = true;
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("inc");
    setX(320);
    setY(40);
}
UI::Action ZPosIncrease::click() {
    pObject->transform->position.z += 2.0;

    return UI::Action::None;
}
ZPosDecrease::ZPosDecrease(::WorldObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_z_pos_decrease";
    isHoverable = true;
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("dec");
    setX(360);
    setY(40);
}
UI::Action ZPosDecrease::click() {
    pObject->transform->position.z -= 2.0;

    return UI::Action::None;
}



ToggleWireframe::ToggleWireframe(::WorldObject* _pObject) {
    pObject = _pObject;
    isHoverable = true;
    id = pObject->name + "_toggle_wireframe_";
    vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("Toggle wireframe");
    setX(10);
    setY(10);
}

UI::Action ToggleWireframe::click() {
    // std::cout << "Clicked ToggleWireframe" << std::endl;
    if (pObject == nullptr) {
        std::cout << "ERROR: Cant toggle wireframe. Bound world object is null. " << std::endl;
        return UI::Action::None;;
    }
    pObject->toggleWireframe();

    return UI::Action::None;
}





void Context::populateContext(::WorldObject* _pObject) {
    std::cout << "POPOLATING CONTEXT" << std::endl;
    
    container = new UI::PObject::Container(_pObject);

    // Name Label
    nameLabel = new UI::PObject::NameLabel(_pObject);
    container->appendChild(nameLabel);


    // X position
    xPosition = new UI::PObject::XPosition(_pObject);
    container->appendChild(xPosition);
    // X pos. increase
    xPosIncrease = new UI::PObject::XPosIncrease(_pObject);
    container->appendChild(xPosIncrease);
    // X pos. decrease
    xPosDecrease = new UI::PObject::XPosDecrease(_pObject);
    container->appendChild(xPosDecrease);


    // Y pos
    yPosition = new UI::PObject::YPosition(_pObject);
    container->appendChild(yPosition);
    // Y pos. increase
    yPosIncrease = new UI::PObject::YPosIncrease(_pObject);
    container->appendChild(yPosIncrease);
    // Y pos. decrease
    yPosDecrease = new UI::PObject::YPosDecrease(_pObject);
    container->appendChild(yPosDecrease);


    // Z pos
    zPosition = new UI::PObject::ZPosition(_pObject);
    container->appendChild(zPosition);
    // Z pos. increase
    zPosIncrease = new UI::PObject::ZPosIncrease(_pObject);
    container->appendChild(zPosIncrease);
    // Z pos. decrease
    zPosDecrease = new UI::PObject::ZPosDecrease(_pObject);
    container->appendChild(zPosDecrease);


    // Wireframe Toggle
    toggleWireframe = new UI::PObject::ToggleWireframe(_pObject);
    container->appendChild(toggleWireframe);

}


// Primitive* newComponent(::WorldObject* _pObject) {

//     std::string _name = _pObject->name;

//     // Container
//     Primitive* pObjectComponentHead = new UI::PObject::Container(_pObject);


//     // Name Label
//     UI::Primitive* name_label = new UI::PObject::NameLabel(_pObject);
//     pObjectComponentHead->appendChild(name_label);


//     // X position
//     UI::Primitive* x_position = new UI::PObject::XPosition(_pObject);
//     pObjectComponentHead->appendChild(x_position);
//     // X pos. increase
//     UI::Primitive* x_pos_increase = new UI::PObject::XPosIncrease(_pObject);
//     pObjectComponentHead->appendChild(x_pos_increase);
//     // X pos. decrease
//     UI::Primitive* x_pos_decrease = new UI::PObject::XPosDecrease(_pObject);
//     pObjectComponentHead->appendChild(x_pos_decrease);


//     // Y pos
//     UI::Primitive* y_pos = new UI::PObject::YPosition(_pObject);
//     pObjectComponentHead->appendChild(y_pos);
//     // Y pos. increase
//     UI::Primitive* y_pos_increase = new UI::PObject::YPosIncrease(_pObject);
//     pObjectComponentHead->appendChild(y_pos_increase);
//     // Y pos. decrease
//     UI::Primitive* y_pos_decrease = new UI::PObject::YPosDecrease(_pObject);
//     pObjectComponentHead->appendChild(y_pos_decrease);


//     // Z pos
//     UI::Primitive* z_pos = new UI::PObject::ZPosition(_pObject);
//     pObjectComponentHead->appendChild(z_pos);
//     // Z pos. increase
//     UI::Primitive* z_pos_increase = new UI::PObject::ZPosIncrease(_pObject);
//     pObjectComponentHead->appendChild(z_pos_increase);
//     // Z pos. decrease
//     UI::Primitive* Z_pos_decrease = new UI::PObject::ZPosDecrease(_pObject);
//     pObjectComponentHead->appendChild(Z_pos_decrease);


//     // Wireframe Toggle
//     UI::PObject::ToggleWireframe* toggle_wireframe_ = new UI::PObject::ToggleWireframe(_pObject);
//     pObjectComponentHead->appendChild(toggle_wireframe_);

//     return pObjectComponentHead;
// }



}
