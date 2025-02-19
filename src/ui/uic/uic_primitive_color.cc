
#include "uic_primitive_color.hh"

#include "lib/lib.hh"
#include <random>

namespace UI::component {

UIC_PrimitiveColor_SetBlack::UIC_PrimitiveColor_SetBlack() {
    setHeight(20);
    isClickable = true;

    initGraphics();
    // setDefaultColor({ 0,0,0, 0 });
    setState(PrimitiveState::Selected);
}
UiResult UIC_PrimitiveColor_SetBlack::click_new(){
    plib::plog_info(plib::LogScope::UI, "AAAAAAAAAAAAA");
    UIC_PrimitiveColor* uic_PrimitiveColor = (UIC_PrimitiveColor*)this->parent;
    // uic_PrimitiveColor->boundObject.setDefaultColor({0,0,0,0});
    int rand_height = std::rand() % 300;
    uic_PrimitiveColor->boundObject.setHeight(rand_height);
    // set_color(Colors::Green);
    uic_PrimitiveColor->boundObject.set_color(Colors::LightGreen);
    // uic_PrimitiveColor->boundObject.set_x("<500x");
    return UiResult(true, Action::None, this);
}

UIC_PrimitiveColor::UIC_PrimitiveColor(::UI::Primitive& _primitive)
    :   boundObject     { _primitive },
        title           { PrimitiveString("Primitive Color") },
        set_black_btn   { UIC_PrimitiveColor_SetBlack () }
{
    setHeight(50);

    initGraphics();

    appendChild(&title);
    title.str_setFontSize(FontSize::f15);
    title.set_y("^10x");
    // title.setDefaultColor();

    appendChild(&set_black_btn);
    set_black_btn.set_y("^30x");
    
}

void UIC_PrimitiveColor::render_component() {
    render();

    title.render();
    set_black_btn.render();
}

UiResult UIC_PrimitiveColor::try_find_target_component(double x, double y) {
    // Return if point is not contain within the root component primitive
    if (!containsPoint(x, y))
        return UiResult();

    if (title.containsPoint(x, y))
        return UiResult(true, Action::None, &title);

    if (set_black_btn.containsPoint(x, y))
        return UiResult(true, Action::None, &set_black_btn);

    return UiResult(true, Action::None, this);

}



}
