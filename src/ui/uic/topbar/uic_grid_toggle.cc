#include <iostream>
#include "glad/glad.h"

#include "lib/lib.hh"

#include "ui/ui_globals.hh"

#include "ui/uic/topbar/uic_grid_toggle.hh"


namespace UI::component {

UIC_Topbar_GridToggle_LeftPanel::UIC_Topbar_GridToggle_LeftPanel() {
    id = "GridToggle_LeftPanel";
    set_w("20x");
    set_h("20x");
    set_color({1.0, 0.0, 0.0, 1.0});
}
UiResult UIC_Topbar_GridToggle_LeftPanel::click() {
    std::cout << "LEFT PANEL TOGGLE" << std::endl;
    return UiResult(true, CAction::UI_ToggleLeftPanel, this);
}


UIC_Topbar_GridToggle_Workbench::UIC_Topbar_GridToggle_Workbench() {
    id = "GridToggle_Workbench";
    set_w("20x");
    set_h("20x");
    set_color({0.0, 1.0, 0.0, 1.0});
}
UiResult UIC_Topbar_GridToggle_Workbench::click() {
    std::cout << "WORKBENCH TOGGLE" << std::endl;
    return UiResult(true, CAction::UI_ToggleWorkbench, this);
}


UIC_Topbar_GridToggle_RightPanel::UIC_Topbar_GridToggle_RightPanel() {
    id = "GridToggle_RightPanel";
    set_w("20x");
    set_h("20x");
    set_color({0.0, 0.0, 1.0, 1.0});
}
UiResult UIC_Topbar_GridToggle_RightPanel::click() {
    std::cout << "RIGHT PANEL TOGGLE" << std::endl;
    return UiResult(true, CAction::UI_ToggleRightPanel, this);
}





UIC_Topbar_GridToggle::UIC_Topbar_GridToggle()
    :   uic_Topbar_GridToggle_LeftPanel { UIC_Topbar_GridToggle_LeftPanel() },
        uic_Topbar_GridToggle_Workbench {UIC_Topbar_GridToggle_Workbench() },
        uic_Topbar_GridToggle_RightPanel { UIC_Topbar_GridToggle_RightPanel() }
{
    set_color({0.0, 0.0, 0.0, 0.8});

    set_w("100x");
    set_h("30x");

    set_x(">0x");
    set_y("^0x");

    appendChild(&uic_Topbar_GridToggle_LeftPanel);
    uic_Topbar_GridToggle_LeftPanel.set_x("<10x");
    uic_Topbar_GridToggle_LeftPanel.set_y("_5x");

    appendChild(&uic_Topbar_GridToggle_Workbench);
    uic_Topbar_GridToggle_Workbench.set_x("<40x");
    uic_Topbar_GridToggle_Workbench.set_y("_5x");


    appendChild(&uic_Topbar_GridToggle_RightPanel);
    uic_Topbar_GridToggle_RightPanel.set_x("<70x");
    uic_Topbar_GridToggle_RightPanel.set_y("_5x");

}

void UIC_Topbar_GridToggle::update_component() {}


void UIC_Topbar_GridToggle::render_component(){

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents
    uic_Topbar_GridToggle_LeftPanel.render();
    uic_Topbar_GridToggle_RightPanel.render();
    uic_Topbar_GridToggle_Workbench.render();

    color_shader->stencil_disable();
}


UiResult UIC_Topbar_GridToggle::try_find_target_component(double x, double y) {

    if (!containsPoint(x, y))
        return UiResult();

    if (uic_Topbar_GridToggle_LeftPanel.containsPoint(x, y))
        return UiResult(true, CAction::None, &uic_Topbar_GridToggle_LeftPanel);
    
    if (uic_Topbar_GridToggle_RightPanel.containsPoint(x, y))
        return UiResult(true, CAction::None, &uic_Topbar_GridToggle_RightPanel);

    if (uic_Topbar_GridToggle_Workbench.containsPoint(x, y))
        return UiResult(true, CAction::None, &uic_Topbar_GridToggle_Workbench);

    
    return UiResult(true, CAction::None, this);
}


}
