#ifndef UIC_TOPBAR_HH
#define UIC_TOPBAR_HH


#include "ui/ui_primitive.hh"

#include "ui/uic/topbar/uic_grid_toggle.hh"
#include "ui/uic/topbar/uic_main_states.hh"


namespace UI::component {

// class Component_PrimitiveEditor;


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