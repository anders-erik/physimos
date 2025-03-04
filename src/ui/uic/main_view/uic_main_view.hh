#ifndef UIC_MAIN_VIEW_HH
#define UIC_MAIN_VIEW_HH


#include "ui/ui_primitive.hh"

#include "ui/uic/main_view/uic_main_view_ui_editor.hh"
#include "ui/uic/main_view/uic_main_view_draw.hh"


namespace UI::component {




struct UIC_Root_MainView : public ::UI::Primitive {

    StateMain current_state_main;
    void set_current_state(StateMain new_state);

    UIC_MainView_UiEditor   uic_MainView_UiEditor;
    UIC_MainView_Draw       uic_MainView_Draw;


    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_Root_MainView();
};




}


#endif