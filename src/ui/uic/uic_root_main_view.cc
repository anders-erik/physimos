#include "glad/glad.h"

#include "lib/lib.hh"

#include "ui/ui_globals.hh"

#include "uic_root_main_view.hh"


namespace UI::component {

UIC_Root_MainView::UIC_Root_MainView() {
    set_color({1.0, 0.0, 0.0, 0.1});

    // set_w("60%");
    // set_h("80%o-30");

    set_x("<20%");
    set_y("_20%");

}

void UIC_Root_MainView::update_component() {}


void UIC_Root_MainView::render_component(){

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents

    color_shader->stencil_disable();
}


UiResult UIC_Root_MainView::try_find_target_component(double x, double y) {

    if (!containsPoint(x, y))
        return UiResult();
    
    return UiResult(true, CAction::None, this);
}


}
