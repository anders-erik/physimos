
#include "ui/uic_pobject.hh"
#include "ui/uic_transform.hh"


namespace UI::component {


PobjectComponent* pobjectContext = nullptr;



PobjectContainer::PobjectContainer(PobjectComponent* _componentContext) {
    // pObject = _pObject;
    component = _componentContext;

    ::PObject* pobject = ((PobjectComponent*)component)->boundObject;

    id = pobject->name + "_container";
    uiTransform.vertRef = UI::VertRef::Bottom;
    setWidth(400);
    setHeight(200);
    // initGraphics();
    // setXrecursive(10);
    // setYrecursive(10);
}



PobjectNameLabel::PobjectNameLabel(PobjectComponent* _componentContext) {
    component = _componentContext;

    ::PObject* pobject = ((PobjectComponent*)component)->boundObject;
    
    id = pobject->name + "_name_label";
    uiTransform.vertRef = UI::VertRef::Top;
    // initGraphics();
    // fontSize = UI::FontSize::f24;
    // setString(pobject->name);
    // setXrecursive(10);
    // setYrecursive(10);
}
void PobjectNameLabel::reload(){
    ::PObject* pobject = ((PobjectComponent*)component)->boundObject;
    // setString(pobject->name);
}





ToggleWireframe::ToggleWireframe(PobjectComponent* _componentContext) {
    component = _componentContext;

    ::PObject* pobject = ((PobjectComponent*)component)->boundObject;

    isHoverable = true;
    id = pobject->name + "_toggle_wireframe_";
    uiTransform.vertRef = UI::VertRef::Bottom;
    // initGraphics();
    // fontSize = UI::FontSize::f15;
    // setString("Toggle wireframe");
    // setXrecursive(10);
    // setYrecursive(10);
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




void PobjectComponent::newPObject(::PObject* _pObject){
    boundObject = _pObject;

    transformContext->newTransform(_pObject->transform);

    reloadModule();
}

void PobjectComponent::reloadModule() {
    
    nameLabel->reload();

    transformContext->reloadComponent();
}


void PobjectComponent::populateContext(::PObject* _pObject) {

    boundObject = _pObject;
    
    // Root primitive
    container = new UI::component::PobjectContainer(this);

    // Primitives
    nameLabel = new UI::component::PobjectNameLabel(this);
    container->appendChild(nameLabel);

    toggleWireframe = new UI::component::ToggleWireframe(this);
    container->appendChild(toggleWireframe);


    // Components
    transformContext = new UI::component::transform::TransformComponent();
    transformContext->populateContext(_pObject->transform);
    // transformContext->container->setXrecursive(190);
    // transformContext->container->setYrecursive(10);
    container->appendChild(transformContext->container);

}




}
