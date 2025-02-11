
#include "ui.hh"

#include "ui/ui_globals.hh"
#include "ui/font.hh"
#include "ui/ui_primitive.hh"
#include "ui/uic_pobject.hh"
#include "ui/uic_scene.hh"
#include "ui/uic_transform.hh"


#include "Input.hpp"

#include "scene/pscene.hh"
#include "scene/pobject.hh"
struct PObject;

// KEEP THE OLD UI ALIVE FOR A BIT LONGER
// #include "ui_old.cpp"
#include "PSO_util.hpp"




namespace UI {

// primitives with no parents, usually ui module containers
std::vector<UI::Primitive*> primitiveTreeHeads;
// a linear list of all Primitives. Used for some hover/click tests
std::vector<UI::Primitive*> primitiveList;

double cursor_x = 0.0;
double cursor_y = 0.0;
Primitive* currentlyHoveredPrimitive = nullptr;


void init(){
    // Necessary to render the UI Scene Module
    // ::ui_setWindowSize(SCREEN_INIT_WIDTH, SCREEN_INIT_HEIGHT);
    setViewportDimensions(SCREEN_INIT_WIDTH, SCREEN_INIT_HEIGHT);

    // Load characters-2.bmp character map
    UI::loadFont();

    



    // INITIALLY SELECTED POBJECT
    ::PObject* house1_wo = ::PScene::getWorldObjectByName("house1_obj");
    if (house1_wo == nullptr) {
        std::cout << "Error init ui. Unable to find house 1 obj." << std::endl;
    }
    // WORLD OBJECT COMPONENT 
    // WorldObject* ground_01 = PScene::getWorldObjectByName("ground_01");
    // if (ground_01 != nullptr) {
    //     std::cout << "house1_wo->name = " << ground_01->name << std::endl;
    // }

    // UI::Primitive* worldObjectComponent_tree = UI::PObject::newComponent(house1_wo);
    // primitiveTreeHeads.push_back(worldObjectComponent_tree);
    // // Flatten component for easy traversal
    // std::vector<UI::Primitive*> flatComponent = worldObjectComponent_tree->flattenTree();
    // for (UI::Primitive* _primitive : flatComponent ){
    //     std::cout << "_primitive->id = " << _primitive->id << std::endl;
    //     primitiveList.push_back(_primitive);
    // }


    // PObject Context
    UI::component::pobjectContext = new UI::component::PobjectComponent();
    UI::component::pobjectContext->populateContext(house1_wo);
    // UI::PObject::uiPObjectContext->newPObject(ground_01);
    // add the uiPObject to the global ui lists
    UI::primitiveTreeHeads.push_back(UI::component::pobjectContext->container);
    std::vector<UI::Primitive*> flatComponent = UI::component::pobjectContext->container->flattenTree();
    for (UI::Primitive* _primitive : flatComponent) {
        // std::cout << "_primitive->id = " << _primitive->id << std::endl;
        UI::primitiveList.push_back(_primitive);
    }

    // UiPScene Context
    UI::component::psceneContext = new UI::component::PsceneComponent();
    UI::component::psceneContext->populateContext(::PScene::getCurrentScene());
    std::vector<UI::Primitive*> flatUiPScene = UI::component::psceneContext->container->flattenTree();
    UI::primitiveTreeHeads.push_back(UI::component::psceneContext->container);
    for (UI::Primitive* _primitive : flatUiPScene) {
        // std::cout << "_primitive->id = " << _primitive->id << std::endl;
        UI::primitiveList.push_back(_primitive);
    }


    // TRANSFORM COMPONENT TESTS
    // UI::component::transform::TransformContext* transformContext = new UI::component::transform::TransformContext();
    // transformContext->populateContext(house1_wo->transform);
    // UI::primitiveTreeHeads.push_back(transformContext->container);

    // std::vector<UI::Primitive*> flatTransformContext = transformContext->container->flattenTree();
    // for (UI::Primitive* _primitive : flatTransformContext) {
    //     // std::cout << "_primitive->id = " << _primitive->id << std::endl;
    //     UI::primitiveList.push_back(_primitive);
    // }


    // UI::primtiiveUiInitialTests();


    // Subscribe to cursor position from input library
    ::PInput::subscribeCursorPosition(pointerPositionCallback);
    ::PInput::subscribeLeftClickPosition(leftClickCallback);
    ::PInput::subscribeFrameBufferUpdated(setViewportDimensions);

}

void update(){
    glDisable(GL_DEPTH_TEST);

    for (UI::Primitive* primitiveTreeHead : UI::primitiveTreeHeads) {
        // Will remove transprancy during hover
        primitiveTreeHead->updateShaderMatrixesRecursively();
        primitiveTreeHead->renderRecursive();
    }
}


// Callback for cursor position subscription
void pointerPositionCallback(double x, double y) {
    cursor_x = x;

    double viewport_height_double = (double) viewport_height;
    cursor_y = -(y - viewport_height_double);


    // clear currently hovering primitive
    if (currentlyHoveredPrimitive != nullptr) {
        currentlyHoveredPrimitive->setState(PrimitiveState::Default);
        currentlyHoveredPrimitive = nullptr;
    }

    // setCurrentlyHoveredPrimitive();
    Primitive* targetedPrimitive = getTargetingPrimitive();
    if(targetedPrimitive == nullptr){
        // std::cout << "No primitive targeted." << std::endl;
        return;
    }
    // targetedPrimitive->printId();


    // Update currently hovered primitive
    setCurrentlyHoveredPrimitive(targetedPrimitive);


}



// Don't need cursor position update as it will stay current using pointer position callback
void leftClickCallback(double x, double y) {
    
    
    // Grab current target
    Primitive* targetedPrimitive = getTargetingPrimitive();
    if (targetedPrimitive == nullptr) {
        // std::cout << "No primitive targeted during click." << std::endl;
        return;
    }

    UI::Action postClickAction = targetedPrimitive->click();


    // cast the PObject primitives in the Primary PObject component, all of which has a World/PObject
    // UI::PObject::Base* pObjectPrimitive = dynamic_cast<UI::PObject::Base*>(targetedPrimitive);
    // // bool isValid
    // if (pObjectPrimitive && pObjectPrimitive->pObject != nullptr) {
    //     std::cout << "Valid pObjectPrimitive! clicked!!" << std::endl;
    // }

    // cast the PObject primtiives in the PScene UI element
    UI::component::PscenePObjectListObject* uiPScenePObjectPrimitive = dynamic_cast<UI::component::PscenePObjectListObject*>(targetedPrimitive);

    // Post Click Actions!
    switch (postClickAction)
    {
    case Action::ReloadPObject :
        std::cout << "uiPObjectContext->reloadComponent()" << std::endl;
        UI::component::pobjectContext->reloadModule();
        
        break;

    case Action::LoadPObject :
        std::cout << "newPObject(...)" << std::endl;
        
        UI::component::pobjectContext->newPObject(uiPScenePObjectPrimitive->pObject);

        break;
    
    default:

        break;
    }
    
}


// Returns the primitive that the current mouse cursor is targeting
// Note: assumes that all children to be found is within the parent primitive bounds.
Primitive* getTargetingPrimitive() {

    // flag to check if any primitive maches the cursor location
    // used in conjunction with matchedNonLeaves to indentify non-leaf matches
    bool targetingAPrimitive = false;
    // keep track of which non-leaves primitives are being targeted
    // is used when no leaf matches were found
    std::vector<UI::Primitive*> matchedNonLeaves;


    // sort leaf and non-leaves. Return immediately if matched a targeted leaf.
    for (Primitive* primitive : primitiveList) {

        bool isLeaf = primitive->isLeaf();
        bool containsPoint = primitive->containsPoint(cursor_x, cursor_y);

        if (!containsPoint)
            continue;
        

        // If the primitive is a leaf and contains the cursor location, the primitive is being we found our target!
        if (isLeaf) {
            return primitive;
        }

        targetingAPrimitive = true;
        matchedNonLeaves.push_back(primitive);
    }


    // No matches, so not necessary to check non-leaves
    if (!targetingAPrimitive)
        return nullptr;


    // heck non-leaf primitives
    // if none of a targeted primitives children are being targeted, then the primitive itself is being targeted
    for (Primitive* primitive : matchedNonLeaves) {

        bool childrenContainsPoint = primitive->childrenContainPoint(cursor_x, cursor_y);

        if (!childrenContainsPoint) 
            return primitive;

    }


    std::cout << "Warn: matching non-leaves in forst loop BUT not matched in second loop!" << std::endl;    
    return nullptr;
}


void setCurrentlyHoveredPrimitive(Primitive* newHoverPrimitive){

    // Simply do nothing of primitive is not set to hoverable
    if (newHoverPrimitive->isHoverable) {
        currentlyHoveredPrimitive = newHoverPrimitive;
        newHoverPrimitive->setState(UI::PrimitiveState::Hover);
    }

    return;
}


// extern std::vector<UI::List*> uiLists;
// void listsInitialRefactoring(){
//     // REFACTOR - 2024-10-04
//     // REFACTOR - 2024-10-9
//     std::vector<UI::List*> listsLoadedFromFile = UI::loadListsFromFile();
//     // std::cout << "Yello";
//     for(UI::List* _list_ptr : listsLoadedFromFile)
//         uiLists.push_back(_list_ptr);


//     // UI::List* _woList = new UI::List(UI::ListType::Stack, "woToggleList");
//     UI::List* _woList = new UI::List();
//     _woList->initWoListList();

//     uiLists.push_back(_woList);
// }

void primtiiveUiInitialTests(){


    // test-primitives
    UI::Primitive* _primitive_root = new UI::Primitive();
    _primitive_root->id = "root";
    _primitive_root->uiTransform.vertRef = UI::VertRef::Top;
    _primitive_root->initGraphics();
    _primitive_root->setXrecursive(300);
    _primitive_root->setYrecursive(100);
    _primitive_root->fontSize = UI::FontSize::f15;
    // _primitive_root->setString("I am root primitive!");
    primitiveTreeHeads.push_back(_primitive_root);
    primitiveList.push_back(_primitive_root);

    _primitive_root->printId();

    UI::Primitive* _primitive_child = new UI::Primitive();
    primitiveList.push_back(_primitive_child);
    _primitive_child->id = "1st child";
    _primitive_root->appendChild(_primitive_child);
    _primitive_child->isHoverable = true;
    _primitive_child->uiTransform.vertRef = UI::VertRef::Top;
    _primitive_child->initGraphics();
    _primitive_child->fontSize = UI::FontSize::f15;
    _primitive_child->setString("I am child!");
    _primitive_child->setXrecursive(10);
    _primitive_child->setYrecursive(10);

    UI::Primitive* _primitive_grandchild = new UI::Primitive();
    primitiveList.push_back(_primitive_grandchild);
    _primitive_grandchild->id = "1st grandchild";
    _primitive_child->appendChild(_primitive_grandchild);
    _primitive_grandchild->isHoverable = true;
    _primitive_grandchild->uiTransform.vertRef = UI::VertRef::Top;
    _primitive_grandchild->initGraphics();
    _primitive_grandchild->fontSize = UI::FontSize::f15;
    _primitive_grandchild->setString("I am grandchild!");
    _primitive_grandchild->setXrecursive(10);
    _primitive_grandchild->setYrecursive(20);

    UI::Primitive* _primitive_child_2 = new UI::Primitive();
    primitiveList.push_back(_primitive_child_2);
    _primitive_child_2->id = "2st child";
    _primitive_root->appendChild(_primitive_child_2);
    _primitive_child_2->uiTransform.vertRef = UI::VertRef::Top;
    _primitive_child_2->initGraphics();
    _primitive_child_2->fontSize = UI::FontSize::f15;
    _primitive_child_2->setString("I am 2nd child!");
    _primitive_child_2->setXrecursive(10);
    _primitive_child_2->setYrecursive(60);

}


} // End UI namespace

