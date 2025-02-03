
#include "ui/UiWorldObject.hh"


namespace UI::WorldObject {


void ToggleWireframe::click() {
    std::cout << "Clicked ToggleWireframe" << std::endl;
    if (worldObject == nullptr) {
        std::cout << "ERROR: Cant toggle wireframe. Bound world object is null. " << std::endl;
        return;
    }
    worldObject->toggleWireframe();
}



Primitive* newComponent(::WorldObject* _worldObject) {
    Primitive* worldObjectComponentHead = new UI::Primitive();

    std::string _name = _worldObject->name;

    // World Object Head Primitive
    // Primitive* worldObjectComponentHead = new UI::Primitive();
    worldObjectComponentHead->id = _name + "_primitive_head";
    worldObjectComponentHead->vertRef = UI::VertRef::Bottom;
    worldObjectComponentHead->setWidth(400);
    worldObjectComponentHead->setHeight(100);
    worldObjectComponentHead->initGraphics();
    worldObjectComponentHead->setX(300);
    worldObjectComponentHead->setY(10);
    // worldObjectComponentHead->fontSize = UI::FontSize::f15;
    // _primitive_root->setString("I am root primitive!");
    // primitiveTree.push_back(_primitive_root);
    // primitiveList.push_back(_primitive_root);

    // worldObjectComponentHead->printId();

    // World Object Name Label
    UI::Primitive* name_label = new UI::Primitive();
    // primitiveList.push_back(_primitive_child);
    name_label->id = _name + "_name_label";
    worldObjectComponentHead->appendChild(name_label);
    // name_label->isHoverable = true;
    name_label->vertRef = UI::VertRef::Top;
    name_label->initGraphics();
    name_label->fontSize = UI::FontSize::f15;
    name_label->setString(_worldObject->name);
    name_label->setX(10);
    name_label->setY(10);


    // X pos
    UI::Primitive* x_pos = new UI::Primitive();
    // primitiveList.push_back(_primitive_child_2);
    x_pos->id = _name + "_x_pos";
    worldObjectComponentHead->appendChild(x_pos);
    x_pos->vertRef = UI::VertRef::Bottom;
    // toggle_wireframe->horiRef = UI::HoriRef::Right;
    x_pos->initGraphics();
    x_pos->fontSize = UI::FontSize::f15;
    std::string x_pos_string = std::to_string(_worldObject->transform->position.x);
    x_pos->setString("x = " + x_pos_string.substr(0, 5));
    x_pos->setX(300);
    x_pos->setY(80);

    // Y pos
    UI::Primitive* y_pos = new UI::Primitive();
    // primitiveList.push_back(_primitive_child_2);
    y_pos->id = _name + "_y_pos";
    worldObjectComponentHead->appendChild(y_pos);
    y_pos->vertRef = UI::VertRef::Bottom;
    // toggle_wireframe->horiRef = UI::HoriRef::Right;
    y_pos->initGraphics();
    y_pos->fontSize = UI::FontSize::f15;
    std::string y_pos_string = std::to_string(_worldObject->transform->position.y);
    y_pos->setString("y = " + y_pos_string.substr(0, 5));
    y_pos->setX(300);
    y_pos->setY(60);

    // Z pos
    UI::Primitive* z_pos = new UI::Primitive();
    // primitiveList.push_back(_primitive_child_2);
    z_pos->id = _name + "_z_pos";
    worldObjectComponentHead->appendChild(z_pos);
    z_pos->vertRef = UI::VertRef::Bottom;
    // toggle_wireframe->horiRef = UI::HoriRef::Right;
    z_pos->initGraphics();
    z_pos->fontSize = UI::FontSize::f15;
    std::string z_pos_string = std::to_string(_worldObject->transform->position.z);
    z_pos->setString("z = " + z_pos_string.substr(0, 5));
    z_pos->setX(300);
    z_pos->setY(40);

    // Wireframe Toggle ( NOT IMPLEMENTED!)
    // UI::Primitive* toggle_wireframe_ = new UI::Primitive();
    UI::WorldObject::ToggleWireframe* toggle_wireframe_ = new UI::WorldObject::ToggleWireframe();
    toggle_wireframe_->worldObject = _worldObject;
    // primitiveList.push_back(_primitive_child_2);
    toggle_wireframe_->id = _name + "_toggle_wireframe_";
    worldObjectComponentHead->appendChild(toggle_wireframe_);
    toggle_wireframe_->isHoverable = true;
    // toggle_wireframe_->clickCallback = _worldObject->toggleWireframe;
    toggle_wireframe_->vertRef = UI::VertRef::Bottom;
    // toggle_wireframe->horiRef = UI::HoriRef::Right;
    toggle_wireframe_->initGraphics();
    toggle_wireframe_->fontSize = UI::FontSize::f15;
    toggle_wireframe_->setString("Toggle wireframe");
    toggle_wireframe_->setX(10);
    toggle_wireframe_->setY(10);

    return worldObjectComponentHead;
}

}
