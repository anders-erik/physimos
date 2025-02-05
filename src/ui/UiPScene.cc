
#include "ui/UiPScene.hh"


namespace UI::PScene {


Context* uiPSceneContext = nullptr;



Container::Container() {
    id = "UiPScenelContainer_1";
    vertRef = UI::VertRef::Top;
    horiRef = UI::HoriRef::Right;
    setWidth(300);
    setHeight(300);
    initGraphics();
    setX(10);
    setY(10);
}



NameLabel::NameLabel(std::string sceneName) {
    id = "UiPScenelLabel_1";
    vertRef = UI::VertRef::Top;
    initGraphics();
    fontSize = UI::FontSize::f24;
    setString(sceneName);
    setX(10);
    setY(10);
}



PObjectListObject::PObjectListObject(::WorldObject* _pObject) {
    pObject = _pObject;
    id = _pObject->name;
    vertRef = UI::VertRef::Top;
    isHoverable = true;
    initGraphics();
    fontSize = UI::FontSize::f15;
    setString(_pObject->name);
    setX(10);
}

PObjectList::PObjectList(){
    id = "UiPScenelLabel_1";
    vertRef = UI::VertRef::Top;
    initGraphics();
    fontSize = UI::FontSize::f24;
    setString("PObject List");
    setX(10);
    setY(50);
}

void PObjectList::addPObject(::WorldObject* _pObject) {
    pObjects.push_back(_pObject);
    reloadList();
}
void PObjectList::removePObject(::WorldObject* _pObject) {}

void PObjectList::reloadList() {
    int index = 0;
    for(::WorldObject* _pObject : pObjects){
        index++;

        ::UI::Primitive* _pObjectPrimtive = new ::UI::PScene::PObjectListObject(_pObject);
        
        _pObjectPrimtive->setY(10 + 25*index);
        
        appendChild(_pObjectPrimtive);
        
    }
}



// void Context::reloadComponent() {
//     // xPosition->reload();
// }

void Context::populateContext() {
    // std::cout << "POPULATING CONTEXT" << std::endl;
    
    container = new UI::PScene::Container();

    // Name Label
    nameLabel = new UI::PScene::NameLabel("NAME_HERE");
    container->appendChild(nameLabel);

    pObjectList = new UI::PScene::PObjectList();
    container->appendChild(pObjectList);

    ::WorldObject* house1 =  ::PScene::getWorldObjectByName("house1_obj");
    ::WorldObject* ground01 = ::PScene::getWorldObjectByName("ground_01");

    pObjectList->addPObject(house1);
    pObjectList->addPObject(ground01);
    // pObjectList->addPObject(house1);

}


}

