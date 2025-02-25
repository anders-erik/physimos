#include <iostream>
#include <memory>
#include <cstring>

#include "lib/lib.hh"

#include "uic_primitive_color.hh"
#include "uic/uic_primitive_list.hh"


namespace UI::component {

// UIC_PrimitiveColor* uic_PrimitiveColorPool = nullptr;


// UIC_PrimitiveColor* uic_store(Primitive *_parent, ::UI::Primitive& _primitive){
//     uic_PrimitiveColorPool = (UIC_PrimitiveColor*) std::malloc(sizeof(UIC_PrimitiveColor));

//     UIC_PrimitiveColor new_list_item = UIC_PrimitiveColor(_parent, _primitive);

//     std::memcpy(uic_PrimitiveColorPool, &new_list_item, sizeof(UIC_PrimitiveColor));

//     UIC_PrimitiveColor* new_list_location = uic_PrimitiveColorPool;

//     new_list_location->children.pop_back();
//     new_list_location->children.pop_back();
//     new_list_location->children.pop_back();
//     new_list_location->appendChild(&new_list_location->title);
//     new_list_location->appendChild(&new_list_location->set_green_btn);
//     new_list_location->appendChild(&new_list_location->set_red_btn);


//     return uic_PrimitiveColorPool;
// }

// UIC_PrimitiveColor color_editor_1; //  =  UIC_PrimitiveColor(this, boundObject);


UIC_PrimitiveList::UIC_PrimitiveList(::UI::Primitive& _primitive)
    :   boundObject     { _primitive },
        title           { Primitive("Primitive List") }
        // color_editor_1  { UIC_PrimitiveColor(this, boundObject) }
{
    set_w("180x");
    set_h("30x");
    set_color(active_pallete.base2);


    appendChild(&title);
    title.str_setFontSize(FontSize::f18);
    title.set_x("<10x");
    title.set_y("^5x");

    listObjects.reserve(10);
}

void UIC_PrimitiveList::new_list_object(){
    // std::cout << "::::::::::::::"  << std::endl;
    
    // UIC_PrimitiveColor new_color_editor = listObjects.emplace_back(UIC_PrimitiveColor(this, boundObject));
    

    // Scope creation to destroy children pointers, and then reloads the pointers after temporary scope ends.
    {
        UIC_PrimitiveColor new_color_editor = listObjects.emplace_back(UIC_PrimitiveColor(this, boundObject));
    }


    listObjects[listObjects.size() -1].set_color(active_pallete.base1);
    listObjects[listObjects.size() -1].parent = this;

    listObjects[listObjects.size() -1].children.pop_back();
    listObjects[listObjects.size() -1].children.pop_back();
    listObjects[listObjects.size() -1].children.pop_back();
    listObjects[listObjects.size() -1].children.push_back(&listObjects[listObjects.size() -1].title);
    listObjects[listObjects.size() -1].children.push_back(&listObjects[listObjects.size() -1].set_green_btn);
    listObjects[listObjects.size() -1].children.push_back(&listObjects[listObjects.size() -1].set_red_btn);

    listObjects[listObjects.size() -1].title.parent = &listObjects[listObjects.size() -1];
    listObjects[listObjects.size() -1].set_red_btn.parent = &listObjects[listObjects.size() - 1];
    listObjects[listObjects.size() -1].set_green_btn.parent = &listObjects[listObjects.size() - 1];

    // listObjects[listObjects.size() -1].set_w("500x");
    // listObjects[listObjects.size() -1].set_x("<100x");

    int new_y_from_top = list_item_stride_px * listObjects.size();

    listObjects[listObjects.size() -1].set_y("^" + std::to_string(new_y_from_top) + "x");
    // listObjects[listObjects.size() -1].uiTransform.size_has_been_changed = true;


    this->uiTransform.size_has_been_changed = true;

    // listObjects[listObjects.size() -1].title.set_x("<10x");
    // listObjects[listObjects.size() -1].title.set_y("_100x");
    // listObjects[listObjects.size() -1].set_red_btn.set_x("<10x");
    // listObjects[listObjects.size() -1].set_red_btn.set_y("_100x");
    // listObjects[listObjects.size() -1].set_y("_100x");
    
    // UIC_PrimitiveColor* new_color =  uic_store(this, boundObject);
    // listObjectPtnrs.push_back(uic_store(this, boundObject));

    // appendChild(&new_color_editor);

    // new_color_editor.children.pop_back();
    // new_color_editor.children.pop_back();
    // new_color_editor.children.pop_back();
    // new_color_editor.children.push_back(&new_color_editor.title);
    // new_color_editor.children.push_back(&new_color_editor.set_green_btn);
    // new_color_editor.children.push_back(&new_color_editor.set_red_btn);
    // new_color_editor.appendChild(&new_color_editor.title);
    // new_color_editor.appendChild(&new_color_editor.set_green_btn);
    // new_color_editor.appendChild(&new_color_editor.set_red_btn);

    
    // new_color_editor.parent = this;
}

void UIC_PrimitiveList::render_component() {
    render();
    title.render();


    // color_editor_1.render_component();

    for(UIC_PrimitiveColor& color_editor : listObjects){
        // color_editor.render_component();

        // color_editor.children.pop_back();
        // color_editor.children.pop_back();
        // color_editor.children.pop_back();
        // color_editor.appendChild(&color_editor.title);
        // color_editor.appendChild(&color_editor.set_green_btn);
        // color_editor.appendChild(&color_editor.set_red_btn);

        color_editor.render_component();
        // color_editor.render();
        int x = 0;
    }

}

UiResult UIC_PrimitiveList::try_find_target_component(double x, double y) {
    // Return if point is not contain within the root component primitive
    if (!containsPoint(x, y))
        return UiResult();

    if (title.containsPoint(x, y))
        return UiResult(true, Action::None, &title);


    return UiResult(true, Action::None, this);

}



}
