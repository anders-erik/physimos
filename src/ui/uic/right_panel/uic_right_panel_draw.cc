#include "glad/glad.h"

#include "lib/lib.hh"

#include "ui/ui_globals.hh"

#include "ui/uic/right_panel/uic_right_panel_draw.hh"
#include "ui/uic/draw/uic_draw_transform.hh"


namespace UI::component {



UIC_RightPanel_Draw::UIC_RightPanel_Draw()
    :   transform   { UIC_Draw_Transform() },
        brush       { UIC_Draw_Brush() }
{
    id = "UIC_RightPanel_Draw";
    // set_color({0.5, 0.5, 0.5, 0.5});
    // set_color(active_pallete.base2);

    set_w("100%o-5");
    set_h("500x");

    set_x(">0x");
    set_y("_0x");

    // appendChild(&uic_Root_RightPanel_Resizer);
    appendChild(&transform);
    appendChild(&brush);
    // uic_PrimitiveEditor.set_y("^5x");

}

void UIC_RightPanel_Draw::update_component() {
    // transform.update_component();
}


void UIC_RightPanel_Draw::render_component(){

    if(!render_enabled)
        return;

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents
    // uic_Root_RightPanel_Resizer.render();
    transform.update_component();
    transform.render_component();
    brush.update_component();
    brush.render_component();

    color_shader->stencil_disable();
}


UiResult UIC_RightPanel_Draw::try_find_target_component(double x, double y) {

    if(!render_enabled)
        return UiResult();

    if (!containsPoint(x, y))
        return UiResult();

    if (transform.containsPoint(x, y))
        return transform.try_find_target_component(x, y);

    if (brush.containsPoint(x, y))
        return brush.try_find_target_component(x, y);

    // if (uic_PrimitiveListEditor.containsPoint(x, y))
    //     return uic_PrimitiveListEditor.try_find_target_component(x, y);
    
    // if (uic_Root_RightPanel_Resizer.containsPoint(x, y))
    //     return UiResult(true, CAction::None, &uic_Root_RightPanel_Resizer);
    
    return UiResult(true, CAction::None, this);
}


}
