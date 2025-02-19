
#include "ui/uic_scene.hh"


namespace UI::component {


PsceneComponent* psceneContext = nullptr;



PsceneContainer::PsceneContainer(PsceneComponent* psceneContext) {
    component = psceneContext;

    id = "UiPScenelContainer_1";
    uiTransform.vertRef = UI::VertRef::Top;
    uiTransform.horiRef = UI::HoriRef::Left;
    setWidth(300);
    setHeight(400);
    // initGraphics();
    // setXrecursive(10);
    // setYrecursive(150);
}



// PsceneNameLabel::PsceneNameLabel(std::string sceneName) {
PsceneNameLabel::PsceneNameLabel(PsceneComponent* psceneContext) {
    component = psceneContext;

    ::PScene::Scene* scene = psceneContext->boundObject;

    id = "UiPScenelNameLabel_1";
    uiTransform.vertRef = UI::VertRef::Top;
    uiTransform.horiRef = UI::HoriRef::Right;
    // initGraphics();
    // fontSize = UI::FontSize::f24;
    // setString(scene->name);
    // setXrecursive(12);
    // setYrecursive(10);
}



PscenePObjectListObject::PscenePObjectListObject(::PObject* _pObject) {
    pObject = _pObject;
    id = _pObject->name;
    uiTransform.vertRef = UI::VertRef::Top;
    isHoverable = true;
    // initGraphics();
    // fontSize = UI::FontSize::f15;
    // setString(_pObject->name);
    // setXrecursive(10);
}

PscenePObjectList::PscenePObjectList(PsceneComponent* psceneContext) {
    component = psceneContext;

    id = "UiPScenelObjectList_1";
    uiTransform.vertRef = UI::VertRef::Top;
    // initGraphics();
    // fontSize = UI::FontSize::f24;
    // setString("PObject List");
    // setXrecursive(10);
    // setYrecursive(50);

    reloadList();
}


void PscenePObjectList::reloadList() {

    ::PScene::Scene* scene = psceneContext->boundObject;
    std::vector<::PObject*> pObjects = scene->pObjects;

    int index = 0;
    for(::PObject* _pObject : pObjects){
        index++;

        ::UI::Primitive* _pObjectPrimtive = new ::UI::component::PscenePObjectListObject(_pObject);
        
        // _pObjectPrimtive->setYrecursive(10 + 20*index);
        
        appendChild(_pObjectPrimtive);
        
    }
}



void PsceneComponent::populateContext(::PScene::Scene* _scene) {
    boundObject = _scene;
    
    // Root primitive
    container = new UI::component::PsceneContainer(this);

    // Primitives
    nameLabel = new UI::component::PsceneNameLabel(this);
    container->appendChild(nameLabel);

    pObjectList = new UI::component::PscenePObjectList(this);
    container->appendChild(pObjectList);

}


}

