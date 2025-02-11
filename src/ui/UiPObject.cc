
#include "ui/UiPObject.hh"


namespace UI::PObject {


Context* uiPObjectContext = nullptr;



Container::Container(::PObject* _pObject) {
    pObject = _pObject;

    id = pObject->name + "_container";
    uiTransform.vertRef = UI::VertRef::Bottom;
    setWidth(400);
    setHeight(200);
    initGraphics();
    setX(10);
    setY(10);
}
void Container::setPObject(::PObject* _pObject) {
    pObject = _pObject;

    id = pObject->name + "_container";
}



NameLabel::NameLabel(::PObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_name_label";
    uiTransform.vertRef = UI::VertRef::Top;
    initGraphics();
    fontSize = UI::FontSize::f24;
    setString(_pObject->name);
    setX(10);
    setY(10);
}
void NameLabel::setPObject(::PObject* _pObject) {
    pObject = _pObject;

    id = pObject->name + "_name_label";
    setString(_pObject->name);
}


XPosition::XPosition(::PObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_x_position";
    uiTransform.vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    std::string x_pos_string = std::to_string(_pObject->transform->position.data[0]);
    setString("x = " + x_pos_string.substr(0, 5));
    // setX(300);
    // setY(80);
    setX(220);
    setY(80);
}
void XPosition::reload() {
    std::string x_pos_string = std::to_string(pObject->transform->position.data[0]);
    setString("x = " + x_pos_string.substr(0, 5));
}
void XPosition::setPObject(::PObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_name_label";

    std::string x_pos_string = std::to_string(_pObject->transform->position.data[0]);
    setString("x = " + x_pos_string.substr(0, 5));
}

XPosIncrease::XPosIncrease(::PObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_x_pos_increase";
    isHoverable = true;
    uiTransform.vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("inc");
    setX(320);
    setY(80);
}
void XPosIncrease::setPObject(::PObject* _pObject) {
    pObject = _pObject;
}
UI::Action XPosIncrease::click() {
    pObject->transform->position.data[0] += 2.0;

    return UI::Action::ReloadPObject;
}

XPosDecrease::XPosDecrease(::PObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_x_pos_decrease";
    isHoverable = true;
    uiTransform.vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("dec");
    setX(360);
    setY(80);
}
void XPosDecrease::setPObject(::PObject* _pObject) {
    pObject = _pObject;
}
UI::Action XPosDecrease::click() {
    pObject->transform->position.data[0] -= 2.0;

    return UI::Action::ReloadPObject;
}



ToggleWireframe::ToggleWireframe(::PObject* _pObject) {
    pObject = _pObject;
    isHoverable = true;
    id = pObject->name + "_toggle_wireframe_";
    uiTransform.vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("Toggle wireframe");
    setX(10);
    setY(10);
}
void ToggleWireframe::setPObject(::PObject* _pObject) {
    pObject = _pObject;
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



void Context::newPObject(::PObject* _pObject){
    container->setPObject(_pObject);

    nameLabel->setPObject(_pObject);

    xPosition->setPObject(_pObject);
    xPosIncrease->setPObject(_pObject);
    xPosDecrease->setPObject(_pObject);

    yPosition->setPObject(_pObject);
    yPosIncrease->setPObject(_pObject);
    yPosDecrease->setPObject(_pObject);

    zPosition->setPObject(_pObject);
    zPosIncrease->setPObject(_pObject);
    zPosDecrease->setPObject(_pObject);

    toggleWireframe->setPObject(_pObject);
}

void Context::reloadComponent() {
    xPosition->reload();
}


void Context::populateContext(::PObject* _pObject) {
    // std::cout << "POPOLATING CONTEXT" << std::endl;
    
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


    // Wireframe Toggle
    toggleWireframe = new UI::PObject::ToggleWireframe(_pObject);
    container->appendChild(toggleWireframe);



    // Y and Z below will not be changed until component actually usable

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


}







/*
    CURRENTLY STAGNANT Z and Y position Primitives
*/








YPosition::YPosition(::PObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_y_position";
    uiTransform.vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    std::string x_pos_string = std::to_string(_pObject->transform->position.data[1]);
    setString("y = " + x_pos_string.substr(0, 5));
    setX(220);
    setY(60);
}
YPosIncrease::YPosIncrease(::PObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_y_pos_increase";
    isHoverable = true;
    uiTransform.vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("inc");
    setX(320);
    setY(60);
}
UI::Action YPosIncrease::click() {
    pObject->transform->position.data[1] += 2.0;

    return UI::Action::None;
}
YPosDecrease::YPosDecrease(::PObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_y_pos_decrease";
    isHoverable = true;
    uiTransform.vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("dec");
    setX(360);
    setY(60);
}
UI::Action YPosDecrease::click() {
    pObject->transform->position.data[1] -= 2.0;

    return UI::Action::None;
}



ZPosition::ZPosition(::PObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_z_position";
    uiTransform.vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    std::string x_pos_string = std::to_string(_pObject->transform->position.data[2]);
    setString("z = " + x_pos_string.substr(0, 5));
    setX(220);
    setY(40);
}
ZPosIncrease::ZPosIncrease(::PObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_z_pos_increase";
    isHoverable = true;
    uiTransform.vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("inc");
    setX(320);
    setY(40);
}
UI::Action ZPosIncrease::click() {
    pObject->transform->position.data[2] += 2.0;

    return UI::Action::None;
}
ZPosDecrease::ZPosDecrease(::PObject* _pObject) {
    pObject = _pObject;
    id = pObject->name + "_z_pos_decrease";
    isHoverable = true;
    uiTransform.vertRef = UI::VertRef::Bottom;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString("dec");
    setX(360);
    setY(40);
}
UI::Action ZPosDecrease::click() {
    pObject->transform->position.data[2] -= 2.0;

    return UI::Action::None;
}


}
