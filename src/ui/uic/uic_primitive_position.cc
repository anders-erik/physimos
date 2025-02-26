
#include "uic_primitive_position.hh"

#include "lib/lib.hh"
#include "ui/ui_texture.hh"
#include <random>

namespace UI::component {

UIC_PrimitivePosition_up::UIC_PrimitivePosition_up() {
    set_w("20x");
    set_h("20x");
    set_texture(texture::get_icon(Icon::Up));
}
UiResult UIC_PrimitivePosition_up::click() {
    UIC_PrimitivePosition* uic_PrimitivePosition = (UIC_PrimitivePosition*)this->parent;
    uic_PrimitivePosition->boundObject.inc_y();
    return UiResult(true, Action::None, this);
}


UIC_PrimitivePosition_down::UIC_PrimitivePosition_down() {
    set_w("20x");
    set_h("20x");
    set_texture(texture::get_icon(Icon::Down));
}
UiResult UIC_PrimitivePosition_down::click() {
    UIC_PrimitivePosition* uic_PrimitivePosition = (UIC_PrimitivePosition*)this->parent;
    uic_PrimitivePosition->boundObject.dec_y();
    return UiResult(true, Action::None, this);
}

UIC_PrimitivePosition_left::UIC_PrimitivePosition_left() {
    set_w("20x");
    set_h("20x");
    set_texture(texture::get_icon(Icon::Left));
}
UiResult UIC_PrimitivePosition_left::click() {
    UIC_PrimitivePosition* uic_PrimitivePosition = (UIC_PrimitivePosition*)this->parent;
    uic_PrimitivePosition->boundObject.dec_x();
    return UiResult(true, Action::None, this);
}

UIC_PrimitivePosition_right::UIC_PrimitivePosition_right() {
    set_w("20x");
    set_h("20x");
    set_texture(texture::get_icon(Icon::Right));
}
UiResult UIC_PrimitivePosition_right::click() {
    UIC_PrimitivePosition* uic_PrimitivePosition = (UIC_PrimitivePosition*)this->parent;
    uic_PrimitivePosition->boundObject.inc_x();
    return UiResult(true, Action::None, this);
}





UIC_PrimitivePosition_scroll::UIC_PrimitivePosition_scroll() {
    set_w("20x");
    set_h("20x");
    set_texture(texture::get_icon(Icon::ScrollVert));
    scrollable = true;
}
UiResult UIC_PrimitivePosition_scroll::scroll(double y_change) {
    UIC_PrimitivePosition* uic_PrimitivePosition = (UIC_PrimitivePosition*)this->parent;

    if(y_change > 0)
        uic_PrimitivePosition->boundObject.inc_y();
    else
        uic_PrimitivePosition->boundObject.dec_y();

    return UiResult(true, Action::None, this);
}

UIC_PrimitivePosition_drag::UIC_PrimitivePosition_drag() {
    set_w("20x");
    set_h("20x");
    set_texture(texture::get_icon(Icon::Pan));
}
UiResult UIC_PrimitivePosition_drag::grabbed(double dx, double dy) {
    UIC_PrimitivePosition* uic_PrimitivePosition = (UIC_PrimitivePosition*)this->parent;

    y_accum += dy;
    x_accum += dx;

    if(y_accum > 3.0){
        y_accum = 0.0;
        uic_PrimitivePosition->boundObject.inc_y();
    }
    else if(y_accum < -3.0){
        y_accum = 0.0;
        uic_PrimitivePosition->boundObject.dec_y();
    }

    if(x_accum > 3.0){
        x_accum = 0.0;
        uic_PrimitivePosition->boundObject.inc_x();
    }
    else if (x_accum < -3.0){
        x_accum = 0.0;
        uic_PrimitivePosition->boundObject.dec_x();
    }

    return UiResult(true, Action::None, this);
}



UIC_PrimitivePosition::UIC_PrimitivePosition(::UI::Primitive& _primitive)
    :   boundObject     { _primitive },
        title           { Primitive("Pos") },
        up_btn          { UIC_PrimitivePosition_up() },
        down_btn        { UIC_PrimitivePosition_down() },
        left_btn        { UIC_PrimitivePosition_left() },
        right_btn       { UIC_PrimitivePosition_right() },
        scroll          { UIC_PrimitivePosition_scroll() },
        drag            { UIC_PrimitivePosition_drag() }
{
    set_w("96%");
    set_x("<2%");
    set_h("90x");
    set_color(active_pallete.base2);


    appendChild(&title);
    title.str_setFontSize(FontSize::f24);
    title.set_x("<5x");
    title.set_y("^0x");


    appendChild(&up_btn);
    up_btn.set_x("<100x");
    up_btn.set_y("^5x");

    appendChild(&down_btn);
    down_btn.set_x("<100x");
    down_btn.set_y("^65x");

    appendChild(&left_btn);
    left_btn.set_x("<70x");
    left_btn.set_y("^35x");

    appendChild(&right_btn);
    right_btn.set_x("<130x");
    right_btn.set_y("^35x");



    appendChild(&scroll);
    scroll.set_x("<130x");
    scroll.set_y("^5x");

    appendChild(&drag);
    drag.set_x("<100x");
    drag.set_y("^35x");
    
}

void UIC_PrimitivePosition::render_component() {
    render();

    title.render();

    up_btn.render();
    down_btn.render();
    left_btn.render();
    right_btn.render();

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

    if (left_btn.containsPoint(x, y))
        return UiResult(true, Action::None, &left_btn);

    if (right_btn.containsPoint(x, y))
        return UiResult(true, Action::None, &right_btn);


    if (scroll.containsPoint(x, y))
        return UiResult(true, Action::None, &scroll);

    if (drag.containsPoint(x, y))
        return UiResult(true, Action::None, &drag);

    return UiResult(true, Action::None, this);

}



}
