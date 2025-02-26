#ifndef UIC_TOPBAR_GRID_TOGGLE_HH
#define UIC_TOPBAR_GRID_TOGGLE_HH


#include "ui/ui_primitive.hh"


namespace UI::component {


struct UIC_Topbar_GridToggle_LeftPanel : public ::UI::Primitive {
    UiResult click();
    UiResult scroll(double dy);
    UIC_Topbar_GridToggle_LeftPanel();
};
struct UIC_Topbar_GridToggle_RightPanel : public ::UI::Primitive {
    UiResult click();
    UIC_Topbar_GridToggle_RightPanel();
};
struct UIC_Topbar_GridToggle_Workbench : public ::UI::Primitive {
    UiResult click();
    UIC_Topbar_GridToggle_Workbench();
};


struct UIC_Topbar_GridToggle : public ::UI::Primitive {


    UIC_Topbar_GridToggle_LeftPanel  uic_Topbar_GridToggle_LeftPanel;
    UIC_Topbar_GridToggle_Workbench  uic_Topbar_GridToggle_Workbench;
    UIC_Topbar_GridToggle_RightPanel uic_Topbar_GridToggle_RightPanel;

    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_Topbar_GridToggle();
};




}


#endif