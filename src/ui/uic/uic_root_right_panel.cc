#include "glad/glad.h"

#include "lib/lib.hh"

#include "ui/ui_globals.hh"

#include "uic_root_right_panel.hh"


namespace UI::component {

UIC_Root_RightPanel_Resizer::UIC_Root_RightPanel_Resizer(){
    id = "UIC_Root_RightPanel_Resizer";

    set_w("5x");
    set_h("100%");

    set_x("<0x");
    set_y("_0x");

    set_color(black);
}
HoverEvent UIC_Root_RightPanel_Resizer::hover_enter(){
    set_state(PrimitiveState::Hover);
    return HoverEvent(true, PCursor::Hori, this);
}
HoverEvent UIC_Root_RightPanel_Resizer::hover_exit(){
    set_state(PrimitiveState::Default);
    return HoverEvent(true, PCursor::Default, this);
}
UiResult UIC_Root_RightPanel_Resizer::click(){
    return UiResult();
}
GrabState UIC_Root_RightPanel_Resizer::grab(){
    return GrabState(true, PCursor::Hori, this);
}

UIC_Root_RightPanel::UIC_Root_RightPanel()
    : uic_Root_RightPanel_Resizer { UIC_Root_RightPanel_Resizer() }
{
    set_color({0.0, 1.0, 0.0, 0.1});

    set_w("20%");
    set_h("100%o-30");

    set_x(">0x");
    set_y("_0x");

    appendChild(&uic_Root_RightPanel_Resizer);
}

void UIC_Root_RightPanel::update_component() {}


void UIC_Root_RightPanel::render_component(){

    if(!render_enabled)
        return;

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents
    uic_Root_RightPanel_Resizer.render();

    color_shader->stencil_disable();
}


UiResult UIC_Root_RightPanel::try_find_target_component(double x, double y) {

    if (!containsPoint(x, y))
        return UiResult();

    
    if (uic_Root_RightPanel_Resizer.containsPoint(x, y))
        return UiResult(true, CAction::None, &uic_Root_RightPanel_Resizer);
    
    return UiResult(true, CAction::None, this);
}


}
