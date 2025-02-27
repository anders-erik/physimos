#include <iostream>
#include "glad/glad.h"

#include "lib/lib.hh"

#include "ui/ui_globals.hh"

#include "uic_root_workbench.hh"


namespace UI::component {


UIC_Root_Workbench_Resizer::UIC_Root_Workbench_Resizer(){
    id = "UIC_Root_Workbench_Resizer";

    set_w("100%");
    set_h("10x");

    set_x("<0x");
    set_y("^0x");

    set_color(active_pallete.base2);
}
UiResult UIC_Root_Workbench_Resizer::click(){
    std::cout << "CLICK RESIZE WORKBENCH" << std::endl;
    
    return UiResult(true, CAction::None, this);
}
GrabState UIC_Root_Workbench_Resizer::grab(){
    std::cout << "GRABBED RESIZE WORKBENCH" << std::endl;

    return GrabState(true, PCursor::Vert, this);
}
UiResult UIC_Root_Workbench_Resizer::hover_enter(){
    set_state(PrimitiveState::Hover);
    return UiResult(true, CAction::None, this);
}
UiResult UIC_Root_Workbench_Resizer::hover_exit(){
    set_state(PrimitiveState::Default);
    return UiResult(true, CAction::None, this);
}

UIC_Root_Workbench::UIC_Root_Workbench() 
    : uic_Root_Workbench_Resizer { UIC_Root_Workbench_Resizer() }
{
    id = "UIC_Root_Workbench_Resizer";
    set_color({0.0, 0.0, 1.0, 0.1});

    set_w("60%");
    set_h("20%");

    set_x("<20%");
    set_y("_0x");

    appendChild(&uic_Root_Workbench_Resizer);

}

void UIC_Root_Workbench::update_component() {}


void UIC_Root_Workbench::render_component(){

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents
    uic_Root_Workbench_Resizer.render();

    color_shader->stencil_disable();
}


UiResult UIC_Root_Workbench::try_find_target_component(double x, double y) {

    if (!containsPoint(x, y))
        return UiResult();

    if (uic_Root_Workbench_Resizer.containsPoint(x, y))
        return UiResult(true, CAction::None, &uic_Root_Workbench_Resizer);
    
    return UiResult(true, CAction::None, this);
}


}
