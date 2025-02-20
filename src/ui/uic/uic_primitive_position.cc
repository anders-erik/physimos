
#include "uic_primitive_position.hh"

#include "lib/lib.hh"
#include <random>

namespace UI::component {

UIC_PrimitivePosition_up::UIC_PrimitivePosition_up() {
    set_w("20x");
    set_h("20x");
    set_color(Colors::LightGray);
}
UiResult UIC_PrimitivePosition_up::click() {
    UIC_PrimitivePosition* uic_PrimitivePosition = (UIC_PrimitivePosition*)this->parent;

    int y_input = uic_PrimitivePosition->boundObject.uiTransform.y_input;
    // Unit y_unit = uic_PrimitivePosition->boundObject.uiTransform.y_unit;
    // VertRef y_ref = uic_PrimitivePosition->boundObject.uiTransform.vertRef;
    std::string new_input = "<" + std::to_string(y_input + 10) + "x";
    uic_PrimitivePosition->boundObject.set_y(new_input);

    return UiResult(true, Action::None, this);
}


UIC_PrimitivePosition_down::UIC_PrimitivePosition_down() {
    set_w("20x");
    set_h("20x");
    set_color(Colors::LightGray);
}

UiResult UIC_PrimitivePosition_down::click() {
    UIC_PrimitivePosition* uic_PrimitivePosition = (UIC_PrimitivePosition*)this->parent;

    int y_input = uic_PrimitivePosition->boundObject.uiTransform.y_input;
    std::string new_input = "<" + std::to_string(y_input - 10) + "x";
    uic_PrimitivePosition->boundObject.set_y(new_input);

    return UiResult(true, Action::None, this);
}

UIC_PrimitivePosition_scroll::UIC_PrimitivePosition_scroll() {
    set_w("20x");
    set_h("20x");
    set_color(Colors::LightGray);
}
UiResult UIC_PrimitivePosition_scroll::scroll(double y_change) {
    UIC_PrimitivePosition* uic_PrimitivePosition = (UIC_PrimitivePosition*)this->parent;

    std::string new_input;
    int y_input = uic_PrimitivePosition->boundObject.uiTransform.y_input;
    if(y_change > 0)
        new_input = "<" + std::to_string(y_input + 10) + "x";
    else
        new_input = "<" + std::to_string(y_input - 10) + "x";

    uic_PrimitivePosition->boundObject.set_y(new_input);

    return UiResult(true, Action::None, this);
}

UIC_PrimitivePosition_drag::UIC_PrimitivePosition_drag() {
    set_w("20x");
    set_h("20x");
    set_color(Colors::LightGray);
}
UiResult UIC_PrimitivePosition_drag::grabbed(double dx, double dy) {
    UIC_PrimitivePosition* uic_PrimitivePosition = (UIC_PrimitivePosition*)this->parent;

    y_accum += dy;
    std::string new_y_input;
    int y_input = uic_PrimitivePosition->boundObject.uiTransform.y_input;

    if(y_accum > 3.0){
        y_accum = 0.0;
        new_y_input = "_" + std::to_string(y_input + 10) + "x";
        uic_PrimitivePosition->boundObject.set_y(new_y_input);
    }
    else if(y_accum < -3.0){
        y_accum = 0.0;
        new_y_input = "_" + std::to_string(y_input - 10) + "x";
        uic_PrimitivePosition->boundObject.set_y(new_y_input);
    }

    

    x_accum += dx;
    std::string new_x_input;
    int x_input = uic_PrimitivePosition->boundObject.uiTransform.x_input;

    if(x_accum > 3.0){
        x_accum = 0.0;
        new_x_input = "<" + std::to_string(x_input + 10) + "x";
        uic_PrimitivePosition->boundObject.set_x(new_x_input);
    }
    else if (x_accum < -3.0){
        x_accum = 0.0;
        new_x_input = "<" + std::to_string(x_input - 10) + "x";
        uic_PrimitivePosition->boundObject.set_x(new_x_input);
    }


    return UiResult(true, Action::None, this);
}



UIC_PrimitivePosition::UIC_PrimitivePosition(::UI::Primitive& _primitive)
    :   boundObject     { _primitive },
        title           { PrimitiveString("Pos") },
        up_btn          { UIC_PrimitivePosition_up() },
        down_btn        { UIC_PrimitivePosition_down() },
        scroll          { UIC_PrimitivePosition_scroll() },
        drag            { UIC_PrimitivePosition_drag() }
{
    set_w("180x");
    set_h("30x");
    set_color(Colors::DarkGray);


    appendChild(&title);
    title.str_setFontSize(FontSize::f15);
    title.set_x("<10x");
    title.set_y("^5x");

    appendChild(&up_btn);
    up_btn.set_x("<60x");
    up_btn.set_y("^5x");

    appendChild(&down_btn);
    down_btn.set_x("<85x");
    down_btn.set_y("^5x");

    appendChild(&scroll);
    scroll.set_x("<110x");
    scroll.set_y("^5x");

    appendChild(&drag);
    drag.set_x("<135x");
    drag.set_y("^5x");
    
}

void UIC_PrimitivePosition::render_component() {
    render();

    title.render();
    up_btn.render();
    down_btn.render();
    scroll.render();
    drag.render();
}

UiResult UIC_PrimitivePosition::try_find_target_component(double x, double y) {
    // Return if point is not contain within the root component primitive
    if (!containsPoint(x, y))
        return UiResult();

    if (title.containsPoint(x, y))
        return UiResult(true, Action::None, &title);

    if (up_btn.containsPoint(x, y))
        return UiResult(true, Action::None, &up_btn);

    if (down_btn.containsPoint(x, y))
        return UiResult(true, Action::None, &down_btn);

    if (scroll.containsPoint(x, y))
        return UiResult(true, Action::None, &scroll);

    if (drag.containsPoint(x, y))
        return UiResult(true, Action::None, &drag);

    return UiResult(true, Action::None, this);

}



}
