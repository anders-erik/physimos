#ifndef UIC_TOPBAR_GRID_TOGGLE_HH
#define UIC_TOPBAR_GRID_TOGGLE_HH


#include "ui/ui_primitive.hh"


namespace UI::component {

// class Component_PrimitiveEditor;


struct UIC_Topbar_GridToggle : public ::UI::Primitive {

    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_Topbar_GridToggle();
};




}


#endif