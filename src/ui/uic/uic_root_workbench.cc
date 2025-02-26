#include "glad/glad.h"

#include "lib/lib.hh"

#include "ui/ui_globals.hh"

#include "uic_root_workbench.hh"


namespace UI::component {

UIC_Root_Workbench::UIC_Root_Workbench() {
    set_color({0.0, 0.0, 1.0, 0.1});

    set_w("60%");
    set_h("20%");

    set_x("<20%");
    set_y("_0x");

}

void UIC_Root_Workbench::update_component() {}


void UIC_Root_Workbench::render_component(){

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents

    color_shader->stencil_disable();
}


UiResult UIC_Root_Workbench::try_find_target_component(double x, double y) {

    if (!containsPoint(x, y))
        return UiResult();
    
    return UiResult(true, Action::None, this);
}


}
