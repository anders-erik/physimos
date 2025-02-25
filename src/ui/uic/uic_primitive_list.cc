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

    // WHEN THE NUBMER OF OBJECT EXCEED THIS, ALL POINTERS NEED TO BE RELOADED!
    // Unsure of when
    // listObjects.reserve(4);
}

void UIC_PrimitiveList::new_list_object(){

    // {
    UIC_PrimitiveColor& new_color_editor = listObjects.emplace_back(UIC_PrimitiveColor(this, boundObject));
    // }
    // UIC_PrimitiveColor& new_color_editor = listObjects[listObjects.size()-1];

    new_color_editor.set_color(active_pallete.base1);

    // Make sure all pointers are up to date after 
    new_color_editor.reload_pointers(this);

    // Place
    int new_y_from_top = list_item_stride_px * listObjects.size();
    new_color_editor.set_y("^" + std::to_string(new_y_from_top) + "x");

    // this->children.push_back(&new_color_editor);
    this->uiTransform.size_has_been_changed = true;

}

void UIC_PrimitiveList::del_list_object(){

    if(listObjects.size() > 0)
        listObjects.pop_back();
    
}

void UIC_PrimitiveList::render_component() {
    render();

    title.render();

    for(UIC_PrimitiveColor& color_editor : listObjects){

        // color_editor.render_component();
        color_editor.render_component();

    }

}

UiResult UIC_PrimitiveList::try_find_target_component(double x, double y) {
    // Return if point is not contain within the root component primitive
    if (!containsPoint(x, y))
        return UiResult();

    if (title.containsPoint(x, y))
        return UiResult(true, Action::None, &title);

    for(UIC_PrimitiveColor& color_editor : listObjects){

        if (color_editor.containsPoint(x, y))
            return color_editor.try_find_target_component(x, y);

    }


    return UiResult(true, Action::None, this);

}



}
