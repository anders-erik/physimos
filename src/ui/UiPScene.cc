
#include "ui/UiPScene.hh"


namespace UI::PScene {


Context* uiPSceneContext = nullptr;



Container::Container() {
    id = "UiPScenelContainer_1";
    vertRef = UI::VertRef::Top;
    horiRef = UI::HoriRef::Left;
    setWidth(300);
    setHeight(400);
    initGraphics();
    setX(10);
    setY(150);
}



NameLabel::NameLabel(std::string sceneName) {
    id = "UiPScenelNameLabel_1";
    vertRef = UI::VertRef::Top;
    initGraphics();
    fontSize = UI::FontSize::f24;
    setString(sceneName);
    setX(10);
    setY(10);
}



PObjectListObject::PObjectListObject(::PObject* _pObject) {
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
    id = "UiPScenelObjectList_1";
    vertRef = UI::VertRef::Top;
    initGraphics();
    fontSize = UI::FontSize::f24;
    setString("PObject List");
    setX(10);
    setY(50);
}

void PObjectList::addPObject(::PObject* _pObject) {
    pObjects.push_back(_pObject);
    // reloadList();
}
void PObjectList::removePObject(::PObject* _pObject) {}

void PObjectList::reloadList() {
    int index = 0;
    for(::PObject* _pObject : pObjects){
        index++;

        ::UI::Primitive* _pObjectPrimtive = new ::UI::PScene::PObjectListObject(_pObject);
        
        _pObjectPrimtive->setY(10 + 20*index);
        
        appendChild(_pObjectPrimtive);
        
    }
}



void Context::populateContext(::PScene::Scene* _scene) {
    
    container = new UI::PScene::Container();

    // Name Label
    // nameLabel = new UI::PScene::NameLabel("SCENE MENU");
    nameLabel = new UI::PScene::NameLabel(_scene->name);
    container->appendChild(nameLabel);

    pObjectList = new UI::PScene::PObjectList();
    container->appendChild(pObjectList);

    // ::WorldObject* house1 =  ::PScene::getWorldObjectByName("house1_obj");
    // ::WorldObject* ground01 = ::PScene::getWorldObjectByName("ground_01");
    // pObjectList->addPObject(house1);
    // pObjectList->addPObject(ground01);
    // pObjectList->addPObject(house1);

    std::vector<::PObject*> pObjects = ::PScene::getPObjects();

    for (::PObject* pObject : pObjects){
        // pObjectList->addPObject(pObject);
        pObjectList->pObjects.push_back(pObject);
    }
    pObjectList->reloadList();


}


}

