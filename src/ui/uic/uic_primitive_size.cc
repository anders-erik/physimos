
#include "uic_primitive_size.hh"
#include "ui/ui_texture.hh"

#include "lib/lib.hh"
#include <random>

namespace UI::component {

UIC_PrimitiveSize_dec_width::UIC_PrimitiveSize_dec_width() {
    set_w("20x");
    set_h("20x");
    set_texture(texture::get_icon(Icon::Left));
}
UiResult UIC_PrimitiveSize_dec_width::click() {
    UIC_PrimitiveSize* uic_PrimitiveSize = (UIC_PrimitiveSize*)this->parent;
    uic_PrimitiveSize->boundObject.dec_w();
    return UiResult(true, CAction::None, this);
}


UIC_PrimitiveSize_inc_width::UIC_PrimitiveSize_inc_width() {
    set_w("20x");
    set_h("20x");
    set_texture(texture::get_icon(Icon::Right));
}
UiResult UIC_PrimitiveSize_inc_width::click() {
    UIC_PrimitiveSize* uic_PrimitiveSize = (UIC_PrimitiveSize*)this->parent;
    uic_PrimitiveSize->boundObject.inc_w();
    return UiResult(true, CAction::None, this);
}

UIC_PrimitiveSize_dec_height::UIC_PrimitiveSize_dec_height() {
    set_w("20x");
    set_h("20x");
    set_texture(texture::get_icon(Icon::Down));
}
UiResult UIC_PrimitiveSize_dec_height::click() {
    UIC_PrimitiveSize* uic_PrimitiveSize = (UIC_PrimitiveSize*)this->parent;
    uic_PrimitiveSize->boundObject.dec_h();
    return UiResult(true, CAction::None, this);
}

UIC_PrimitiveSize_inc_height::UIC_PrimitiveSize_inc_height() {
    set_w("20x");
    set_h("20x");
    set_texture(texture::get_icon(Icon::Up));
}
UiResult UIC_PrimitiveSize_inc_height::click() {
    UIC_PrimitiveSize* uic_PrimitiveSize = (UIC_PrimitiveSize*)this->parent;
    uic_PrimitiveSize->boundObject.inc_h();
    return UiResult(true, CAction::None, this);
}





UIC_PrimitiveSize_drag::UIC_PrimitiveSize_drag() {
    set_w("20x");
    set_h("20x");
    set_texture(texture::get_icon(Icon::Resize));
}
UiResult UIC_PrimitiveSize_drag::grabbed(double dx, double dy) {
    UIC_PrimitiveSize* uic_PrimitiveSize = (UIC_PrimitiveSize*)this->parent;

    y_accum += dy;
    x_accum += dx;


    if(y_accum > 3.0){
        y_accum = 0.0;
        uic_PrimitiveSize->boundObject.inc_h();
    }
    else if(y_accum < -3.0){
        y_accum = 0.0;
        uic_PrimitiveSize->boundObject.dec_h();
    }


    if(x_accum > 3.0){
        x_accum = 0.0;
        uic_PrimitiveSize->boundObject.inc_w();
    }
    else if (x_accum < -3.0){
        x_accum = 0.0;
        uic_PrimitiveSize->boundObject.dec_w();
    }


    return UiResult(true, CAction::None, this);
}



UIC_PrimitiveSize::UIC_PrimitiveSize(::UI::Primitive& _primitive)
    :   boundObject     { _primitive },
        title           { Primitive("Size") },
        dec_width       { UIC_PrimitiveSize_dec_width() },
        inc_width       { UIC_PrimitiveSize_inc_width() },
        dec_height      { UIC_PrimitiveSize_dec_height() },
        inc_height      { UIC_PrimitiveSize_inc_height() },
        drag            { UIC_PrimitiveSize_drag() }
{
    set_w("96%");
    set_h("90x");
    set_color(active_pallete.base2);

    appendChild(&title);
    title.str_setFontSize(FontSize::f24);
    title.set_x("<5x");
    title.set_y("^0x");

    appendChild(&dec_width);
    dec_width.set_x("<70x");
    dec_width.set_y("^35x");

    appendChild(&inc_width);
    inc_width.set_x("<130x");
    inc_width.set_y("^35x");

    appendChild(&dec_height);
    dec_height.set_x("<100x");
    dec_height.set_y("^65x");

    appendChild(&inc_height);
    inc_height.set_x("<100x");
    inc_height.set_y("^5x");

    appendChild(&drag);
    drag.set_x("<100x");
    drag.set_y("^35x");
    
}

void UIC_PrimitiveSize::render_component() {
    render();

    title.render();

    dec_width.render();
    inc_width.render();
    dec_height.render();
    inc_height.render();

    drag.render();
}

UiResult UIC_PrimitiveSize::try_find_target_component(double x, double y) {
    // Return if point is not contain within the root component primitive
    if (!containsPoint(x, y))
        return UiResult();

    if (dec_width.containsPoint(x, y))
        return UiResult(true, CAction::None, &dec_width);

    if (inc_width.containsPoint(x, y))
        return UiResult(true, CAction::None, &inc_width);

    if (dec_height.containsPoint(x, y))
        return UiResult(true, CAction::None, &dec_height);

    if (inc_height.containsPoint(x, y))
        return UiResult(true, CAction::None, &inc_height);
    
    if (drag.containsPoint(x, y))
        return UiResult(true, CAction::None, &drag);

    return UiResult(true, CAction::None, this);

}



}
