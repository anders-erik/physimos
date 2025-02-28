#ifndef UIC_TOPBAR_MAIN_STATES_HH
#define UIC_TOPBAR_MAIN_STATES_HH


#include "ui/ui_primitive.hh"


namespace UI::component {


struct UIC_Topbar_MainStates_Scene3D : public ::UI::Primitive {
    Primitive string_primitive;
    UiResult click();
    UIC_Topbar_MainStates_Scene3D();
};
struct UIC_Topbar_MainStates_Canvas : public ::UI::Primitive {
    Primitive string_primitive;
    UiResult click();
    UIC_Topbar_MainStates_Canvas();
};
struct UIC_Topbar_MainStates_UIEditor : public ::UI::Primitive {
    Primitive string_primitive;
    UiResult click();
    UIC_Topbar_MainStates_UIEditor();
};


struct UIC_Topbar_MainStates : public ::UI::Primitive {


    UIC_Topbar_MainStates_Scene3D   uic_Topbar_MainStates_Scene3D;
    UIC_Topbar_MainStates_Canvas    uic_Topbar_MainStates_Canvas;
    UIC_Topbar_MainStates_UIEditor  uic_Topbar_MainStates_UIEditor;

    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_Topbar_MainStates();
};




}


#endif