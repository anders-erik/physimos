#include "glad/glad.h"

#include "lib/lib.hh"

#include "ui/ui_globals.hh"

#include "uic_main_view_draw.hh"


namespace UI::component {


UIC_MainView_Draw_CursorPos::UIC_MainView_Draw_CursorPos(){

    id = "Main_view_draw_cursor_pos";
    // set_color({0.0, 1.0, 0.0, 1.0});
    // set_color(active_pallete.detail2);

    set_w("100%");
    set_h("200x");

    set_x("<0x");
    set_y("_0x");

}

void UIC_MainView_Draw_CursorPos::update_component(){

}
void UIC_MainView_Draw_CursorPos::render_component(){

}

UiResult UIC_MainView_Draw_CursorPos::try_find_target_component(double x, double y){
    return UiResult();
}


UIC_MainView_Draw::UIC_MainView_Draw()
    : uic_MainView_Draw_CursorPos { UIC_MainView_Draw_CursorPos() }
{
    id = "Main_view_draw";
    set_color({0.0, 1.0, 0.0, 0.0});
    // set_color(active_pallete.detail2);
    render_enabled = false;

    set_w("100%");
    set_h("100%");

    set_x("<0x");
    set_y("_0x");

    appendChild(&uic_MainView_Draw_CursorPos);

}

void UIC_MainView_Draw::update_component() {}


void UIC_MainView_Draw::render_component(){

    if(!render_enabled)
        return;

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents
    uic_MainView_Draw_CursorPos.render_component();

    color_shader->stencil_disable();
}


UiResult UIC_MainView_Draw::try_find_target_component(double x, double y) {

    if(!render_enabled)
        return UiResult();

    if (!containsPoint(x, y))
        return UiResult();

    if (uic_MainView_Draw_CursorPos.containsPoint(x, y))
        return uic_MainView_Draw_CursorPos.try_find_target_component(x, y);
    
    // if (primitive_to_edit.containsPoint(x, y))
    //     return UiResult(true, CAction::None, &primitive_to_edit);

    // if (primitive_list->containsPoint(x, y))
    //     return primitive_list->try_find_target_component(x, y);
    
    return UiResult(true, CAction::None, this);
}


}
