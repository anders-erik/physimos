#include "glad/glad.h"

#include "lib/lib.hh"

#include "ui/ui_globals.hh"

#include "uic_main_view.hh"


namespace UI::component {

UIC_Root_MainView::UIC_Root_MainView()
    : uic_MainView_UiEditor { UIC_MainView_UiEditor() }
{
    id = "UIC_Root_RightPanel";
    set_color({0.0, 1.0, 0.0, 0.1});

    set_w("20%");
    set_h("100%o-30");

    set_x(">0x");
    set_y("_0x");

    appendChild(&uic_MainView_UiEditor);
}

void UIC_Root_MainView::update_component() {}


void UIC_Root_MainView::render_component(){

    if(!render_enabled)
        return;

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents
    uic_MainView_UiEditor.render_component();

    color_shader->stencil_disable();
}


UiResult UIC_Root_MainView::try_find_target_component(double x, double y) {

    if(!render_enabled)
        return UiResult();

    if (!containsPoint(x, y))
        return UiResult();

    
    // if (uic_Root_RightPanel_Resizer.containsPoint(x, y))
    //     return UiResult(true, CAction::None, &uic_Root_RightPanel_Resizer);

    if (uic_MainView_UiEditor.containsPoint(x, y))
        return uic_MainView_UiEditor.try_find_target_component(x, y);
    
    return UiResult(true, CAction::None, this);
}


}
