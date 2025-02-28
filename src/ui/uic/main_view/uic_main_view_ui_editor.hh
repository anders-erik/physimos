#ifndef UIC_MAIN_VIEW_UI_EDITOR_HH
#define UIC_MAIN_VIEW_UI_EDITOR_HH


#include "ui/ui_primitive.hh"
#include "ui/uic/uic_primitive_list.hh"


namespace UI::component {


extern Primitive primitive_to_edit;
extern UIC_PrimitiveList* primitive_list;


struct UIC_MainView_UiEditor : public ::UI::Primitive {

    // Primitive primitive_to_edit;


    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_MainView_UiEditor();
};




}


#endif