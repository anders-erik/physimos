#include "glad/glad.h"

#include "lib/lib.hh"

#include "ui/ui_globals.hh"

#include "uic_root_topbar.hh"


namespace UI::component {

UIC_Root_Topbar::UIC_Root_Topbar() {
    set_color({0.0, 0.0, 0.0, 0.3});

    set_w("100%");
    set_h("30x");

    set_x("<0x");
    set_y("^0x");

}

void UIC_Root_Topbar::update_component() {}


void UIC_Root_Topbar::render_component(){

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents

    color_shader->stencil_disable();
}


UiResult UIC_Root_Topbar::try_find_target_component(double x, double y) {

    if (!containsPoint(x, y))
        return UiResult();
    
    return UiResult(true, Action::None, this);
}


}
