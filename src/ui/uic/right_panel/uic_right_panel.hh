#ifndef UIC_RIGHT_PANEL_HH
#define UIC_RIGHT_PANEL_HH


#include "ui/ui_primitive.hh"

#include "ui/uic/right_panel/uic_right_panel_ui_editor.hh"
#include "ui/uic/right_panel/uic_right_panel_draw.hh"


namespace UI::component {



struct UIC_Root_RightPanel_Resizer : public ::UI::Primitive {
    virtual HoverEvent hover_enter();
    virtual HoverEvent hover_exit();
    virtual UiResult click();
    virtual GrabState grab();
    
    UIC_Root_RightPanel_Resizer();
};



struct UIC_Root_RightPanel : public ::UI::Primitive {

    UIC_Root_RightPanel_Resizer uic_Root_RightPanel_Resizer;

    UIC_RightPanel_UiEditor uic_RightPanel_UiEditor;
    UIC_RightPanel_Draw uic_RightPanel_Draw;

    StateMain current_state_main;
    void set_current_state(StateMain new_state);

    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_Root_RightPanel();
};




}


#endif