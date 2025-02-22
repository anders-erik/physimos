
#include "uic_primitive_size.hh"
#include "ui/ui_texture.hh"

#include "lib/lib.hh"
#include <random>

namespace UI::component {

UIC_PrimitiveSize_drag::UIC_PrimitiveSize_drag() {
    set_w("20x");
    set_h("20x");
    set_texture(texture::get_icon(Icon::Resize));
}
UiResult UIC_PrimitiveSize_drag::grabbed(double dx, double dy) {
    UIC_PrimitiveSize* uic_PrimitiveSize = (UIC_PrimitiveSize*)this->parent;

    y_accum += dy;
    std::string new_h_input;
    int h_input = uic_PrimitiveSize->boundObject.uiTransform.h_input;

    if(y_accum > 3.0){
        y_accum = 0.0;
        new_h_input = std::to_string(h_input + 10) + "x";
        uic_PrimitiveSize->boundObject.set_h(new_h_input);
    }
    else if(y_accum < -3.0){
        y_accum = 0.0;
        new_h_input = std::to_string(h_input - 10) + "x";
        uic_PrimitiveSize->boundObject.set_h(new_h_input);
    }

    

    x_accum += dx;
    std::string new_w_input;
    int w_input = uic_PrimitiveSize->boundObject.uiTransform.w_input;

    if(x_accum > 3.0){
        x_accum = 0.0;
        new_w_input = std::to_string(w_input + 10) + "x";
        uic_PrimitiveSize->boundObject.set_w(new_w_input);
    }
    else if (x_accum < -3.0){
        x_accum = 0.0;
        new_w_input = std::to_string(w_input - 10) + "x";
        uic_PrimitiveSize->boundObject.set_w(new_w_input);
    }


    return UiResult(true, Action::None, this);
}



UIC_PrimitiveSize::UIC_PrimitiveSize(::UI::Primitive& _primitive)
    :   boundObject     { _primitive },
        title           { PrimitiveString("Size") },
        drag            { UIC_PrimitiveSize_drag() }
{
    set_w("180x");
    set_h("30x");
    set_color(active_pallete.base2);

    appendChild(&title);
    title.str_setFontSize(FontSize::f18);
    title.set_x("<10x");
    title.set_y("^5x");

    appendChild(&drag);
    drag.set_x("<80x");
    drag.set_y("^5x");
    
}

void UIC_PrimitiveSize::render_component() {
    render();

    title.render();
    drag.render();
}

UiResult UIC_PrimitiveSize::try_find_target_component(double x, double y) {
    // Return if point is not contain within the root component primitive
    if (!containsPoint(x, y))
        return UiResult();

    if (drag.containsPoint(x, y))
        return UiResult(true, Action::None, &drag);

    return UiResult(true, Action::None, this);

}



}
