#include "glad/glad.h"

#include "lib/lib.hh"

#include "ui/ui_globals.hh"

#include "uic_main_view.hh"


namespace UI::component {

UIC_Root_MainView::UIC_Root_MainView()
    : uic_MainView_UiEditor { UIC_MainView_UiEditor() }
{
    id = "UIC_Root_MainView";
    // set_color({1.0, 0.0, 0.0, 0.3});
    set_color({1.0, 0.0, 0.0, 0.0});

    // set_w("20%");
    // set_h("100%o-30");

    set_x(">0x");
    set_y("_0x");

    appendChild(&uic_MainView_UiEditor);
}

void UIC_Root_MainView::set_current_state(StateMain new_state){
    current_state_main = new_state;
}

void UIC_Root_MainView::update_component() {}


void UIC_Root_MainView::render_component(){

    if(!render_enabled)
        return;

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents
    switch (current_state_main){

    case StateMain::UIEditor :
        uic_MainView_UiEditor.render_component();
        break;
    
    default:
        break;
    }
    

    color_shader->stencil_disable();
}


UiResult UIC_Root_MainView::try_find_target_component(double x, double y) {

    if(!render_enabled || !containsPoint(x, y))
        return UiResult();

    
    // if (uic_Root_RightPanel_Resizer.containsPoint(x, y))
    //     return UiResult(true, CAction::None, &uic_Root_RightPanel_Resizer);

    
    if (current_state_main == StateMain::UIEditor)
        return uic_MainView_UiEditor.try_find_target_component(x, y);
    
    return UiResult(false, CAction::None, this);
}


}
