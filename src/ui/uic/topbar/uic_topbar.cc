#include "glad/glad.h"

#include "lib/lib.hh"

#include "ui/ui_globals.hh"

#include "uic_topbar.hh"


namespace UI::component {

UIC_Root_Topbar::UIC_Root_Topbar() 
    : grid_toggle { UIC_Topbar_GridToggle() }
{
    set_color({0.0, 0.0, 0.0, 0.3});

    set_w("100%");
    set_h("30x");

    set_x("<0x");
    set_y("^0x");

    appendChild(&grid_toggle);

}

void UIC_Root_Topbar::update_component() {}


void UIC_Root_Topbar::render_component(){

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents
    grid_toggle.render_component();

    color_shader->stencil_disable();
}


UiResult UIC_Root_Topbar::try_find_target_component(double x, double y) {

    if (!containsPoint(x, y))
        return UiResult();

    if (grid_toggle.containsPoint(x, y))
        return grid_toggle.try_find_target_component(x, y);
    
    return UiResult(true, CAction::None, this);
}


}
