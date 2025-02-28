#include "glad/glad.h"

#include "lib/lib.hh"

#include "ui/ui_globals.hh"

#include "uic_main_view_ui_editor.hh"


namespace UI::component {

Primitive primitive_to_edit;
UIC_PrimitiveList* primitive_list;


UIC_MainView_UiEditor::UIC_MainView_UiEditor()
    // : uic_Root_RightPanel_Resizer { UIC_Root_RightPanel_Resizer() }
    // :   primitive_to_edit {Primitive()}
{
    id = "Main_view_ui_editor";
    // set_color({0.0, 1.0, 0.0, 1.0});
    // set_color(active_pallete.detail2);

    set_w("100%");
    set_h("100%");

    set_x("<0x");
    set_y("_0x");

    // appendChild(&uic_Root_RightPanel_Resizer);
    // appendChild(&uic_PrimitiveEditor);

    primitive_to_edit = Primitive();

    appendChild(&primitive_to_edit);

    primitive_to_edit.set_color(black);
    primitive_to_edit.set_w("30x");
    primitive_to_edit.set_h("30x");
    primitive_to_edit.set_x("<30x");
    primitive_to_edit.set_y("_30x");

    primitive_list = new UIC_PrimitiveList(primitive_to_edit);
    appendChild(primitive_list);
    primitive_list->set_x(">0x");
    primitive_list->set_y("_200x");
    primitive_list->set_h("200x");
}

void UIC_MainView_UiEditor::update_component() {}


void UIC_MainView_UiEditor::render_component(){

    if(!render_enabled)
        return;

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents
    // uic_Root_RightPanel_Resizer.render();
    // uic_PrimitiveEditor.render_component();
    primitive_to_edit.render();
    primitive_list->render_component();

    color_shader->stencil_disable();
}


UiResult UIC_MainView_UiEditor::try_find_target_component(double x, double y) {

    if(!render_enabled)
        return UiResult();

    if (!containsPoint(x, y))
        return UiResult();

    // if (uic_PrimitiveEditor.containsPoint(x, y))
    //     return uic_PrimitiveEditor.try_find_target_component(x, y);
    
    if (primitive_to_edit.containsPoint(x, y))
        return UiResult(true, CAction::None, &primitive_to_edit);

    if (primitive_list->containsPoint(x, y))
        return primitive_list->try_find_target_component(x, y);
    
    return UiResult(true, CAction::None, this);
}


}
