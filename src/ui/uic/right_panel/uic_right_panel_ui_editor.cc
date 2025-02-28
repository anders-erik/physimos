#include "glad/glad.h"

#include "lib/lib.hh"

#include "ui/ui_globals.hh"

#include "uic_right_panel_ui_editor.hh"

#include "ui/uic/main_view/uic_main_view_ui_editor.hh"

namespace UI::component {

// UIC_PrimitiveList       uic_PrimitiveList;
//     UIC_PrimitiveListEditor uic_PrimitiveListEditor;
UIC_RightPanel_UiEditor::UIC_RightPanel_UiEditor()
    // : uic_Root_RightPanel_Resizer { UIC_Root_RightPanel_Resizer() }
    // :   primitive_to_edit {Primitive()},
    // :   uic_PrimitiveList { UIC_PrimitiveList(primitive_to_edit) },
    :   uic_PrimitiveListEditor { UIC_PrimitiveListEditor(*primitive_list, primitive_to_edit) },
        uic_PrimitiveEditor {UIC_PrimitiveEditor(primitive_to_edit)}
{
    id = "UIC_RightPanel_UiEditor";
    // set_color({0.0, 1.0, 0.0, 1.0});
    set_color(active_pallete.base2);

    set_w("100%o-5");
    set_h("100%");

    set_x(">0x");
    set_y("_0x");

    // appendChild(&uic_Root_RightPanel_Resizer);
    appendChild(&uic_PrimitiveEditor);
    uic_PrimitiveEditor.set_y("^5x");

    appendChild(&uic_PrimitiveListEditor);
    uic_PrimitiveListEditor.set_h("20%");
    uic_PrimitiveListEditor.set_y("_100x");
}

void UIC_RightPanel_UiEditor::update_component() {}


void UIC_RightPanel_UiEditor::render_component(){

    if(!render_enabled)
        return;

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents
    // uic_Root_RightPanel_Resizer.render();
    uic_PrimitiveEditor.render_component();
    uic_PrimitiveListEditor.render_component();

    color_shader->stencil_disable();
}


UiResult UIC_RightPanel_UiEditor::try_find_target_component(double x, double y) {

    if(!render_enabled)
        return UiResult();

    if (!containsPoint(x, y))
        return UiResult();

    if (uic_PrimitiveEditor.containsPoint(x, y))
        return uic_PrimitiveEditor.try_find_target_component(x, y);

    if (uic_PrimitiveListEditor.containsPoint(x, y))
        return uic_PrimitiveListEditor.try_find_target_component(x, y);
    
    // if (uic_Root_RightPanel_Resizer.containsPoint(x, y))
    //     return UiResult(true, CAction::None, &uic_Root_RightPanel_Resizer);
    
    return UiResult(true, CAction::None, this);
}


}
