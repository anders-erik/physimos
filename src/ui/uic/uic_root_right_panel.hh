#ifndef UIC_ROOT_RIGHT_PANEL_HH
#define UIC_ROOT_RIGHT_PANEL_HH


#include "ui/ui_primitive.hh"


namespace UI::component {

// class Component_PrimitiveEditor;


struct UIC_Root_RightPanel : public ::UI::Primitive {

    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_Root_RightPanel();
};




}


#endif