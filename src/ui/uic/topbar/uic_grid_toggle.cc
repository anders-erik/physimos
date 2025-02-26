#include "glad/glad.h"

#include "lib/lib.hh"

#include "ui/ui_globals.hh"

#include "ui/uic/topbar/uic_grid_toggle.hh"


namespace UI::component {

UIC_Topbar_GridToggle::UIC_Topbar_GridToggle() {
    set_color({0.0, 0.0, 0.0, 0.8});

    set_w("100x");
    set_h("30x");

    set_x(">0x");
    set_y("^0x");

}

void UIC_Topbar_GridToggle::update_component() {}


void UIC_Topbar_GridToggle::render_component(){

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents

    color_shader->stencil_disable();
}


UiResult UIC_Topbar_GridToggle::try_find_target_component(double x, double y) {

    if (!containsPoint(x, y))
        return UiResult();
    
    return UiResult(true, CAction::None, this);
}


}
