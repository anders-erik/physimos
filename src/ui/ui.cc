#include "glad/glad.h"  // glDisable(GL_DEPTH_TEST)
#include "log.hh"

#include "ui.hh"

#include "ui/ui_globals.hh"
#include "ui/ui_texture.hh"
#include "ui/ui_shader.hh"
#include "ui/font.hh"
#include "ui/ui_primitive.hh"
#include "ui/ui_component.hh"

#include "ui/uic_pobject.hh"
#include "ui/uic_scene.hh"
#include "ui/uic_transform.hh"
#include "ui/uic/uic_primitive_editor.hh"


#include "Input.hpp"
#include "Windowing.hpp"

#include "scene/pscene.hh"
#include "scene/pobject.hh"
struct PObject;

// KEEP THE OLD UI ALIVE FOR A BIT LONGER
// #include "ui_old.cpp"
#include "PSO_util.hpp"




namespace UI {

void window_changed_callback(PhysWin physimos_window){
    shader::texture_shader.set_window_info(
        physimos_window.width,
        physimos_window.height,
        physimos_window.xscale,
        physimos_window.yscale
    );
}

// primitives with no parents, usually ui module containers
std::vector<UI::Primitive*> primitiveTreeHeads;
// a linear list of all Primitives. Used for some hover/click tests
std::vector<UI::Primitive*> primitiveList;

/** Returned primitive from finding primitive target during left click. Is reset to nullptr on left release. */
Primitive* grabbed_primitive = nullptr;

/** Current cursor position recieved in the input callback function. */
double cursor_x = 0.0;
/** Current cursor position recieved in the input callback function. y = 0 at the bottom of window.  */
double cursor_y = 0.0;
Primitive* currentlyHoveredPrimitive = nullptr;

UI::Primitive* primitive_to_edit;
// UI::component::Component_PrimitiveEditor* primitive_editor;
UI::component::UIC_PrimitiveEditor* primitive_editor;
void init(){
    PhysWin new_window = get_initial_physimos_window();
    subscribeWindowChange_ui(window_changed_callback);

    // UI::shader::TextureShader();
    shader::texture_shader.set_window_info(new_window.width, new_window.height, new_window.xscale, new_window.yscale);
    shader::texture_shader.compile_shader();
    shader::texture_shader.init();

    UI::texture::init_static_textures();

    // Necessary to render the UI Scene Module
    // ::ui_setWindowSize(SCREEN_INIT_WIDTH, SCREEN_INIT_HEIGHT);
    setViewportDimensions(SCREEN_INIT_WIDTH, SCREEN_INIT_HEIGHT);

    // Load characters-2.bmp character map
    UI::loadFont();

    
    // PRIMITIVE EDITOR COMPONENT
    primitive_to_edit = new UI::Primitive();
    // primitive_to_edit->initGraphics();
    primitive_to_edit->set_color(Colors::DarkGray);
    primitive_to_edit->setState(PrimitiveState::Default);
    primitive_to_edit->id = "primitive_to_test";
    primitive_to_edit->set_x("<500x");
    primitive_to_edit->set_y("_400x");
    primitive_to_edit->set_h("200x");
    primitive_to_edit->set_w("200x");

    // primitive_to_edit.setXrecursive(500);
    // primitive_to_edit.setYrecursive(400);
    UI::primitiveList.push_back(primitive_to_edit);
    UI::primitiveTreeHeads.push_back(primitive_to_edit);

    primitive_editor = new UI::component::UIC_PrimitiveEditor(*primitive_to_edit);
    primitive_editor->set_x("<800x");
    primitive_editor->set_y("_200x");
    primitive_editor->set_h("400x");
    primitive_editor->set_w("200x");
    

    // Subscribe to cursor position from input library
    ::PInput::subscribePointerPosition_ui(pointerPositionCallback);
    ::PInput::subscribeMouseLeftClick_ui(leftClickCallback);
    ::PInput::subscribeMouseLeftRelease_ui(leftReleaseCallback);
    ::PInput::subscribeMouseScrollY_ui(scrollyCallback);
    
    // ::PInput::subscribeFrameBufferUpdated(setViewportDimensions);


    // UI::primitiveList.push_back(primitive_editor);
    // UI::primitiveTreeHeads.push_back(primitive_editor);
    // primitive_editor = new UI::component::Component_PrimitiveEditor(primitive_to_edit);
    // UI::primitiveList.push_back(&(primitive_editor->container));
    // UI::primitiveTreeHeads.push_back(&(primitive_editor->container));

    // UI::component::ComponentPrimitiveEditor primitive_editor = UI::component::ComponentPrimitiveEditor(primitive_to_edit);

    // add the uiPObject to the global ui lists
    // UI::primitiveTreeHeads.push_back(UI::component::pobjectContext->container);
    // std::vector<UI::Primitive*> flatComponent = UI::component::pobjectContext->container->flattenTree();
    // for (UI::Primitive* _primitive : flatComponent) {
    //     // std::cout << "_primitive->id = " << _primitive->id << std::endl;
    //     UI::primitiveList.push_back(_primitive);
    // }


    // INITIALLY SELECTED POBJECT
    // ::PObject* house1_wo = ::PScene::getWorldObjectByName("house1_obj");
    // if (house1_wo == nullptr) {
    //     std::cout << "Error init ui. Unable to find house 1 obj." << std::endl;
    // }
    // // WORLD OBJECT COMPONENT 
    // // WorldObject* ground_01 = PScene::getWorldObjectByName("ground_01");
    // // if (ground_01 != nullptr) {
    // //     std::cout << "house1_wo->name = " << ground_01->name << std::endl;
    // // }

    // // UI::Primitive* worldObjectComponent_tree = UI::PObject::newComponent(house1_wo);
    // // primitiveTreeHeads.push_back(worldObjectComponent_tree);
    // // // Flatten component for easy traversal
    // // std::vector<UI::Primitive*> flatComponent = worldObjectComponent_tree->flattenTree();
    // // for (UI::Primitive* _primitive : flatComponent ){
    // //     std::cout << "_primitive->id = " << _primitive->id << std::endl;
    // //     primitiveList.push_back(_primitive);
    // // }


    // // PObject Context
    // UI::component::pobjectContext = new UI::component::PobjectComponent();
    // UI::component::pobjectContext->populateContext(house1_wo);
    // // UI::PObject::uiPObjectContext->newPObject(ground_01);
    // // add the uiPObject to the global ui lists
    // UI::primitiveTreeHeads.push_back(UI::component::pobjectContext->container);
    // std::vector<UI::Primitive*> flatComponent = UI::component::pobjectContext->container->flattenTree();
    // for (UI::Primitive* _primitive : flatComponent) {
    //     // std::cout << "_primitive->id = " << _primitive->id << std::endl;
    //     UI::primitiveList.push_back(_primitive);
    // }

    // // UiPScene Context
    // UI::component::psceneContext = new UI::component::PsceneComponent();
    // UI::component::psceneContext->populateContext(::PScene::getCurrentScene());
    // std::vector<UI::Primitive*> flatUiPScene = UI::component::psceneContext->container->flattenTree();
    // UI::primitiveTreeHeads.push_back(UI::component::psceneContext->container);
    // for (UI::Primitive* _primitive : flatUiPScene) {
    //     // std::cout << "_primitive->id = " << _primitive->id << std::endl;
    //     UI::primitiveList.push_back(_primitive);
    // }


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



}

void update(){
    glDisable(GL_DEPTH_TEST);

    primitive_editor->update_component();
    primitive_editor->render_component();

    for (UI::Primitive* primitiveTreeHead : UI::primitiveTreeHeads) {
        // Will remove transprancy during hover
        primitiveTreeHead->updateShaderMatrixesRecursively();
        // primitiveTreeHead->renderRecursive();
        primitiveTreeHead->render_recursive();
    }
}

void pointerPositionCallback(PInput::PointerPosition pointer_pos, PInput::PointerChange _pointer_change) {

    if(grabbed_primitive != nullptr){
        // std::cout << "Dragging " << grabbed_primitive->uiTransform.h_input << std::endl;
        grabbed_primitive->grabbed(_pointer_change.dx, _pointer_change.dy);
    }
        

    cursor_x = pointer_pos.x;
    // Invert Y direction
    // double viewport_height_double = (double)viewport_height;
    // cursor_y = -(pointer_pos.y - viewport_height_double);
    cursor_y = pointer_pos.y;

    // clear currently hovering primitive
    if (currentlyHoveredPrimitive != nullptr) {
        currentlyHoveredPrimitive->setState(PrimitiveState::Default);
        currentlyHoveredPrimitive = nullptr;
    }


    UiResult targetResult = primitive_editor->try_find_target_component(cursor_x, cursor_y);
    if (targetResult.success) {
        currentlyHoveredPrimitive = targetResult.primitive;
        targetResult.primitive->setState(PrimitiveState::Hover);
        // plog_info(::plib::LogScope::UI, "HOVERING PRIMITIVE EDITOR");
        // hoverResult.primitive->setState(PrimitiveState::Hover);
    }

    // setCurrentlyHoveredPrimitive();
    Primitive* targetedPrimitive = getTargetingPrimitive();
    if (targetedPrimitive == nullptr) {
        // std::cout << "No primitive targeted." << std::endl;
        return;
    }
    // targetedPrimitive->printId();


    // Update currently hovered primitive
    setCurrentlyHoveredPrimitive(targetedPrimitive);


}


void scrollyCallback(double y_change) {

    UiResult scrollTargetQuery = primitive_editor->try_find_target_component(cursor_x, cursor_y);

    if (scrollTargetQuery.success) {
        scrollTargetQuery.primitive->scroll(y_change);
    }
    
}

void leftReleaseCallback(PInput::PointerPosition _pointer_pos){
    std::cout << "LEFT RELEASE"  << std::endl;
    if (grabbed_primitive != nullptr)
        std::cout << "Released " << grabbed_primitive->uiTransform.h_input << std::endl;

    
    UiResult releaseTargetResult = primitive_editor->try_find_target_component(cursor_x, cursor_y);

    // We click if: 1) found primitive, 2) primitive is the same one as registered on left click
    bool click_confirmed = releaseTargetResult.success && releaseTargetResult.primitive == grabbed_primitive;
    if (click_confirmed) {
        releaseTargetResult.primitive->click_new();
    }
    
    grabbed_primitive = nullptr;
}

// Don't need cursor position update as it will stay current using pointer position callback
void leftClickCallback(PInput::PointerPosition _pointer_pos) {

    
    UiResult targetResult = primitive_editor->try_find_target_component(cursor_x, cursor_y);
    // if (targetResult.success && targetResult.primitive->isClickable) {
    //     targetResult.primitive->click_new();
    //     // targetResult.primitive->setState(PrimitiveState::Selected);
    //     // plog_info(::plib::LogScope::UI, "CLICLCICLK");
    // }
    if(targetResult.success){
        grabbed_primitive = targetResult.primitive;
        std::cout << "Grabbed " << grabbed_primitive->id << std::endl;
        
    }
    
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


    // // test-primitives
    // UI::Primitive* _primitive_root = new UI::Primitive();
    // _primitive_root->id = "root";
    // _primitive_root->uiTransform.vertRef = UI::VertRef::Top;
    // _primitive_root->initGraphics();
    // _primitive_root->setXrecursive(300);
    // _primitive_root->setYrecursive(100);
    // _primitive_root->fontSize = UI::FontSize::f15;
    // // _primitive_root->setString("I am root primitive!");
    // primitiveTreeHeads.push_back(_primitive_root);
    // primitiveList.push_back(_primitive_root);

    // _primitive_root->printId();

    // UI::Primitive* _primitive_child = new UI::Primitive();
    // primitiveList.push_back(_primitive_child);
    // _primitive_child->id = "1st child";
    // _primitive_root->appendChild(_primitive_child);
    // _primitive_child->isHoverable = true;
    // _primitive_child->uiTransform.vertRef = UI::VertRef::Top;
    // _primitive_child->initGraphics();
    // _primitive_child->fontSize = UI::FontSize::f15;
    // _primitive_child->setString("I am child!");
    // _primitive_child->setXrecursive(10);
    // _primitive_child->setYrecursive(10);

    // UI::Primitive* _primitive_grandchild = new UI::Primitive();
    // primitiveList.push_back(_primitive_grandchild);
    // _primitive_grandchild->id = "1st grandchild";
    // _primitive_child->appendChild(_primitive_grandchild);
    // _primitive_grandchild->isHoverable = true;
    // _primitive_grandchild->uiTransform.vertRef = UI::VertRef::Top;
    // _primitive_grandchild->initGraphics();
    // _primitive_grandchild->fontSize = UI::FontSize::f15;
    // _primitive_grandchild->setString("I am grandchild!");
    // _primitive_grandchild->setXrecursive(10);
    // _primitive_grandchild->setYrecursive(20);

    // UI::Primitive* _primitive_child_2 = new UI::Primitive();
    // primitiveList.push_back(_primitive_child_2);
    // _primitive_child_2->id = "2st child";
    // _primitive_root->appendChild(_primitive_child_2);
    // _primitive_child_2->uiTransform.vertRef = UI::VertRef::Top;
    // _primitive_child_2->initGraphics();
    // _primitive_child_2->fontSize = UI::FontSize::f15;
    // _primitive_child_2->setString("I am 2nd child!");
    // _primitive_child_2->setXrecursive(10);
    // _primitive_child_2->setYrecursive(60);

}


} // End UI namespace

