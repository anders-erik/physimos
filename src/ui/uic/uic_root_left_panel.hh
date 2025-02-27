#ifndef UIC_ROOT_LEFT_PANEL_HH
#define UIC_ROOT_LEFT_PANEL_HH


#include "ui/ui_primitive.hh"


namespace UI::component {


struct UIC_Root_LeftPanel_Resizer : public ::UI::Primitive {
    virtual HoverEvent hover_enter();
    virtual HoverEvent hover_exit();
    virtual UiResult click();
    virtual GrabState grab();
    
    UIC_Root_LeftPanel_Resizer();
};


struct UIC_Root_LeftPanel : public ::UI::Primitive {

    UIC_Root_LeftPanel_Resizer uic_Root_LeftPanel_Resizer;

    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_Root_LeftPanel();
};




}


#endif