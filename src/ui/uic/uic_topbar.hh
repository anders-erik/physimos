#ifndef UIC_TOPBAR_HH
#define UIC_TOPBAR_HH


#include "ui/ui_primitive.hh"
#include "ui/uic/uic_button.hh"



namespace UI::component {



struct UIC_Topbar_GridToggle : public ::UI::Primitive {

    UIC_Button  left_panel;
    UIC_Button  workbench;
    UIC_Button  right_panel;

    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_Topbar_GridToggle();
};


struct UIC_Topbar_MainStates : public ::UI::Primitive {

    UIC_Button_Label   scene_3d;
    UIC_Button_Label    canvas;
    UIC_Button_Label  ui_editor;

    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_Topbar_MainStates();
};



struct UIC_Root_Topbar : public ::UI::Primitive {

    UIC_Topbar_GridToggle grid_toggle;
    UIC_Topbar_MainStates main_states;

    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_Root_Topbar();
};




}


#endif