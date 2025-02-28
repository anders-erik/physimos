#ifndef UIC_RIGHT_PANEL_UI_EDITOR_HH
#define UIC_RIGHT_PANEL_UI_EDITOR_HH


#include "ui/ui_primitive.hh"



#include "ui/uic/uic_primitive_editor.hh"
#include "ui/uic/uic_primitive_list_editor.hh"
#include "ui/uic/uic_primitive_list.hh"


namespace UI::component {





struct UIC_RightPanel_UiEditor : public ::UI::Primitive {

    // Primitive               primitive_to_edit;

    // UIC_PrimitiveList       uic_PrimitiveList;
    UIC_PrimitiveListEditor uic_PrimitiveListEditor;

    UIC_PrimitiveEditor     uic_PrimitiveEditor;
    

    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_RightPanel_UiEditor();
};




}


#endif