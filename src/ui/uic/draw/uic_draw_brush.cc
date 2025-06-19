#include <iostream>

#include "uic/draw/uic_draw_brush.hh"

#include "ui/ui_texture.hh"

#include "lib/lib.hh"
#include "rend/bitmap.hh"
#include <random>

namespace UI::component {

  

UIC_Draw_Brush_SetGreen::UIC_Draw_Brush_SetGreen() {
    id = "prim_color_set_green";
    set_w("20x");
    set_h("20x");
    set_color({0.0, 1.0, 0.0, 1.0});
    scrollable = true;
}
UiResult UIC_Draw_Brush_SetGreen::click(){
    UIC_Draw_Brush* uic_Draw_Brush = (UIC_Draw_Brush*)this->parent;
    uic_Draw_Brush->boundObject.color = pixel_color_green;
    return UiResult(true, CAction::None, this);
}


UIC_Draw_Brush_SetRed::UIC_Draw_Brush_SetRed() {
    id = "prim_color_set_red";
    set_w("20x");
    set_h("20x");
    set_color({1.0, 0.0, 0.0, 1.0});
}
UiResult UIC_Draw_Brush_SetRed::click() {
    UIC_Draw_Brush* uic_Draw_Brush = (UIC_Draw_Brush*)this->parent;
    uic_Draw_Brush->boundObject.color = pixel_color_red;
    return UiResult(true, CAction::None, this);
}

UIC_Draw_Brush_DecSize::UIC_Draw_Brush_DecSize() {
    id = "UIC_Draw_Brush_DecSize";
    set_w("20x");
    set_h("20x");
    // set_color({1.0, 0.0, 0.0, 1.0});
    set_texture(texture::get_icon(Icon::Left));
}
UiResult UIC_Draw_Brush_DecSize::click() {
    UIC_Draw_Brush* uic_Draw_Brush = (UIC_Draw_Brush*)this->parent;
    uic_Draw_Brush->boundObject.size--;
    if( uic_Draw_Brush->boundObject.size <= 0)
        uic_Draw_Brush->boundObject.size = 1;
    return UiResult(true, CAction::None, this);
}

UIC_Draw_Brush_IncSize::UIC_Draw_Brush_IncSize() {
    id = "UIC_Draw_Brush_IncSize";
    set_w("20x");
    set_h("20x");
    // set_color({1.0, 0.0, 0.0, 1.0});

    set_texture(texture::get_icon(Icon::Right));
    
}
UiResult UIC_Draw_Brush_IncSize::click() {
    UIC_Draw_Brush* uic_Draw_Brush = (UIC_Draw_Brush*)this->parent;
    uic_Draw_Brush->boundObject.size++;
    return UiResult(true, CAction::None, this);
}

UIC_Draw_Brush_SizeLabel::UIC_Draw_Brush_SizeLabel() {
    id = "UIC_Draw_Brush_SizeLabel";
    set_color(transparent);
    str_setFontSize(FontSize::f18);
    str_setString("size_label");
}
void UIC_Draw_Brush_SizeLabel::update_component(){
    UIC_Draw_Brush* uic_Draw_Brush = (UIC_Draw_Brush*)this->parent;
    update_str_int(uic_Draw_Brush->boundObject.size);
}



UIC_Draw_Brush::UIC_Draw_Brush()
    :   boundObject     { draw::ui_get_brush() },
        title           { Primitive("Color") },
        set_green_btn   { UIC_Draw_Brush_SetGreen   () },
        set_red_btn     { UIC_Draw_Brush_SetRed     () },
        dec_size        { UIC_Draw_Brush_DecSize    () },
        inc_size        { UIC_Draw_Brush_IncSize    () },
        size_label      { UIC_Draw_Brush_SizeLabel  () }
{
    id = "UIC_Draw_Brush";
    set_w("96%");
    set_x("<2%");

    set_h("60x");
    set_y("^30x");

    // set_color_texture(ColorTexture::DarkGray);
    set_color(active_pallete.base2);


    appendChild(&title);
    title.str_setFontSize(FontSize::f32);
    title.set_x("<10x");
    title.set_y("^5x");

    appendChild(&set_green_btn);
    set_green_btn.set_x("<120x");
    set_green_btn.set_y("^5x");

    appendChild(&set_red_btn);
    set_red_btn.set_x("<150x");
    set_red_btn.set_y("^5x");

    appendChild(&dec_size);
    dec_size.set_x("<120x");
    dec_size.set_y("^30x");

    appendChild(&inc_size);
    inc_size.set_x("<180x");
    inc_size.set_y("^30x");

    appendChild(&size_label);
    size_label.set_x("<150x");
    size_label.set_y("^30x");
    
}

void UIC_Draw_Brush::update_component(){
    size_label.update_component();
}

void UIC_Draw_Brush::render_component() {
    render();

    title.render();

    set_green_btn.render();
    set_red_btn.render();

    dec_size.render();
    inc_size.render();
    size_label.render();
}

UiResult UIC_Draw_Brush::try_find_target_component(double x, double y) {
    // Return if point is not contain within the root component primitive
    if (!containsPoint(x, y))
        return UiResult();

    if (title.containsPoint(x, y))
        return UiResult(true, CAction::None, &title);

    if (set_green_btn.containsPoint(x, y))
        return UiResult(true, CAction::None, &set_green_btn);

    if (set_red_btn.containsPoint(x, y))
        return UiResult(true, CAction::None, &set_red_btn);

    if (dec_size.containsPoint(x, y))
        return UiResult(true, CAction::None, &dec_size);

    if (inc_size.containsPoint(x, y))
        return UiResult(true, CAction::None, &inc_size);

    return UiResult(true, CAction::None, this);

}



}
