#include <iostream>

#include "lib/lib.hh"

#include "uic_primitive_color.hh"
#include "uic/uic_primitive_list.hh"


namespace UI::component {


UIC_PrimitiveList::UIC_PrimitiveList(::UI::Primitive& _primitive)
    :   boundObject     { _primitive },
        title           { Primitive("Primitive List") }
{
    set_w("180x");
    set_h("30x");
    set_color(active_pallete.base2);


    appendChild(&title);
    title.str_setFontSize(FontSize::f18);
    title.set_x("<10x");
    title.set_y("^5x");

    
}

void UIC_PrimitiveList::new_list_object(){
    // std::cout << "::::::::::::::"  << std::endl;
    
    UIC_PrimitiveColor new_color_editor = listObjects.emplace_back(UIC_PrimitiveColor(boundObject));
    appendChild(&new_color_editor);
    new_color_editor.children.pop_back();
    new_color_editor.children.pop_back();
    new_color_editor.children.pop_back();
    // new_color_editor.children.push_back(&new_color_editor.title);
    // new_color_editor.children.push_back(&new_color_editor.set_green_btn);
    // new_color_editor.children.push_back(&new_color_editor.set_red_btn);
    new_color_editor.appendChild(&new_color_editor.title);
    new_color_editor.appendChild(&new_color_editor.set_green_btn);
    new_color_editor.appendChild(&new_color_editor.set_red_btn);

    
    // new_color_editor.parent = this;
}

void UIC_PrimitiveList::render_component() {
    render();

    title.render();

    for(UIC_PrimitiveColor color_editor : listObjects){
        // color_editor.render_component();

        color_editor.children.pop_back();
        color_editor.children.pop_back();
        color_editor.children.pop_back();
        color_editor.appendChild(&color_editor.title);
        color_editor.appendChild(&color_editor.set_green_btn);
        color_editor.appendChild(&color_editor.set_red_btn);

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
