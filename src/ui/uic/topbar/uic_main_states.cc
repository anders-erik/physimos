#include <iostream>
#include "glad/glad.h"

#include "lib/lib.hh"

#include "ui/ui_globals.hh"

#include "ui/uic/topbar/uic_main_states.hh"


namespace UI::component {

UIC_Topbar_MainStates_Scene3D::UIC_Topbar_MainStates_Scene3D() {
    id = "MainStates_Scene3D";
    set_w("40x");
    set_h("20x");
    
    str_setFontSize(FontSize::f18);
    str_setString("Scene 3D");
    set_color(active_pallete.base2);
}
UiResult UIC_Topbar_MainStates_Scene3D::click() {
    return UiResult(true, CAction::State_ToggleScene3D, this);
}


UIC_Topbar_MainStates_Canvas::UIC_Topbar_MainStates_Canvas() {
    id = "MainStates_Canvas";
    set_w("40x");
    set_h("20x");
    
    str_setFontSize(FontSize::f18);
    str_setString("Canvas");
    set_color(active_pallete.base2);
}
UiResult UIC_Topbar_MainStates_Canvas::click() {
    return UiResult(true, CAction::State_ToggleCanvas, this);
}


UIC_Topbar_MainStates_UIEditor::UIC_Topbar_MainStates_UIEditor() {
    id = "MainStates_UIEditor";
    set_w("40x");
    set_h("20x");

    str_setFontSize(FontSize::f18);
    str_setString("UI Editor");
    set_color(active_pallete.base2);
}
UiResult UIC_Topbar_MainStates_UIEditor::click() {
    std::cout << "RIGHT PANEL TOGGLE" << std::endl;
    return UiResult(true, CAction::State_ToggleUIEditor, this);
}





UIC_Topbar_MainStates::UIC_Topbar_MainStates()
    :   uic_Topbar_MainStates_Scene3D { UIC_Topbar_MainStates_Scene3D() },
        uic_Topbar_MainStates_Canvas {UIC_Topbar_MainStates_Canvas() },
        uic_Topbar_MainStates_UIEditor { UIC_Topbar_MainStates_UIEditor() }
{
    set_color({0.0, 0.0, 0.0, 0.8});

    set_w("350x");
    set_h("30x");

    set_x("|");
    set_y("~");

    appendChild(&uic_Topbar_MainStates_Scene3D);
    uic_Topbar_MainStates_Scene3D.set_x("<20x");
    uic_Topbar_MainStates_Scene3D.set_y("_5x");

    appendChild(&uic_Topbar_MainStates_Canvas);
    uic_Topbar_MainStates_Canvas.set_x("<120x");
    uic_Topbar_MainStates_Canvas.set_y("_5x");

    appendChild(&uic_Topbar_MainStates_UIEditor);
    uic_Topbar_MainStates_UIEditor.set_x("<220x");
    uic_Topbar_MainStates_UIEditor.set_y("_5x");

}

void UIC_Topbar_MainStates::update_component() {}


void UIC_Topbar_MainStates::render_component(){

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents
    uic_Topbar_MainStates_Scene3D.render();
    uic_Topbar_MainStates_Canvas.render();
    uic_Topbar_MainStates_UIEditor.render();

    color_shader->stencil_disable();
}


UiResult UIC_Topbar_MainStates::try_find_target_component(double x, double y) {

    if (!containsPoint(x, y))
        return UiResult();

    if (uic_Topbar_MainStates_Scene3D.containsPoint(x, y))
        return UiResult(true, CAction::None, &uic_Topbar_MainStates_Scene3D);
    
    if (uic_Topbar_MainStates_Canvas.containsPoint(x, y))
        return UiResult(true, CAction::None, &uic_Topbar_MainStates_Canvas);

    if (uic_Topbar_MainStates_UIEditor.containsPoint(x, y))
        return UiResult(true, CAction::None, &uic_Topbar_MainStates_UIEditor);

    
    return UiResult(true, CAction::None, this);
}


}
