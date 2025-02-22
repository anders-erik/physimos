
#include "uic_primitive_color.hh"

#include "lib/lib.hh"
#include <random>

namespace UI::component {

UIC_PrimitiveColor_SetGreen::UIC_PrimitiveColor_SetGreen() {
    set_w("20x");
    set_h("20x");
    // set_color_texture(ColorTexture::Green);
    set_color({0.0, 1.0, 0.0, 1.0});
}
UiResult UIC_PrimitiveColor_SetGreen::grabbed(double dx, double dy) {
    // UIC_PrimitiveColor* uic_PrimitiveColor = (UIC_PrimitiveColor*)this->parent;

    std::cout << "DRAGGING GREEN COLOR"  << std::endl;

    return UiResult(true, Action::None, this);
}
UiResult UIC_PrimitiveColor_SetGreen::scroll(double y_change){
    std::cout << "SCROLL GREEN COLOR = : " << y_change << std::endl;
    return UiResult(true, Action::None, this);
}
UiResult UIC_PrimitiveColor_SetGreen::hover_enter() {
    // set_state(PrimitiveState::Hover);
    // darkness_shift = 0.2f;
    set_darkness_shift(0.2f);
    return UiResult(true, Action::None, this);
}
UiResult UIC_PrimitiveColor_SetGreen::hover_exit() {
    // set_state(PrimitiveState::Default);
    // darkness_shift = 0.0f;
    reset_darkness_shift();
    return UiResult(true, Action::None, this);
}
UiResult UIC_PrimitiveColor_SetGreen::click(){
    UIC_PrimitiveColor* uic_PrimitiveColor = (UIC_PrimitiveColor*)this->parent;
    
    // int rand_height = std::rand() % 300;
    // uic_PrimitiveColor->boundObject.set_h(std::to_string(rand_height) + "x");

    // uic_PrimitiveColor->boundObject.set_color_texture(ColorTexture::Green);
    uic_PrimitiveColor->boundObject.set_color({0.0, 1.0, 0.0, 1.0});
    return UiResult(true, Action::None, this);
}


UIC_PrimitiveColor_SetRed::UIC_PrimitiveColor_SetRed() {
    set_w("20x");
    set_h("20x");
    // set_color_texture(ColorTexture::Red);
    set_color({1.0, 0.0, 0.0, 1.0});
}
UiResult UIC_PrimitiveColor_SetRed::click() {
    UIC_PrimitiveColor* uic_PrimitiveColor = (UIC_PrimitiveColor*)this->parent;

    // int rand_width = std::rand() % 300;
    // uic_PrimitiveColor->boundObject.set_w(std::to_string(rand_width) + "x");

    // uic_PrimitiveColor->boundObject.set_color_texture(ColorTexture::Red);
    uic_PrimitiveColor->boundObject.set_color({1.0, 0.0, 0.0, 1.0});
    return UiResult(true, Action::None, this);
}

UIC_PrimitiveColor::UIC_PrimitiveColor(::UI::Primitive& _primitive)
    :   boundObject     { _primitive },
        title           { PrimitiveString("Color") },
        set_green_btn   { UIC_PrimitiveColor_SetGreen   () },
        set_red_btn     { UIC_PrimitiveColor_SetRed     () }
{
    set_w("180x");
    set_h("30x");
    // set_color_texture(ColorTexture::DarkGray);
    set_color(active_pallete.base2);


    appendChild(&title);
    title.str_setFontSize(FontSize::f24);
    title.set_x("<10x");
    title.set_y("^5x");

    appendChild(&set_green_btn);
    set_green_btn.set_x("<120x");
    set_green_btn.set_y("^5x");

    appendChild(&set_red_btn);
    set_red_btn.set_x("<150x");
    set_red_btn.set_y("^5x");
    
}

void UIC_PrimitiveColor::render_component() {
    render();

    title.render();
    set_green_btn.render();
    set_red_btn.render();
}

UiResult UIC_PrimitiveColor::try_find_target_component(double x, double y) {
    // Return if point is not contain within the root component primitive
    if (!containsPoint(x, y))
        return UiResult();

    if (title.containsPoint(x, y))
        return UiResult(true, Action::None, &title);

    if (set_green_btn.containsPoint(x, y))
        return UiResult(true, Action::None, &set_green_btn);

    if (set_red_btn.containsPoint(x, y))
        return UiResult(true, Action::None, &set_red_btn);

    return UiResult(true, Action::None, this);

}



}
