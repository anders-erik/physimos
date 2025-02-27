#ifndef UIC_ROOT_WORKBENCH_HH
#define UIC_ROOT_WORKBENCH_HH


#include "ui/ui_primitive.hh"


namespace UI::component {

// class Component_PrimitiveEditor;

struct UIC_Root_Workbench_Resizer : public ::UI::Primitive {
    virtual UiResult hover_enter();
    virtual UiResult hover_exit();
    virtual UiResult click();
    virtual GrabState grab();
    
    UIC_Root_Workbench_Resizer();
};


struct UIC_Root_Workbench : public ::UI::Primitive {

    UIC_Root_Workbench_Resizer uic_Root_Workbench_Resizer;

    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_Root_Workbench();
};




}


#endif