#include <iostream>

#include "glad/glad.h"

#include "lib/lib.hh"

#include "ui/ui_globals.hh"

#include "uic_topbar.hh"


namespace UI::component {




UIC_Topbar_GridToggle::UIC_Topbar_GridToggle()
    :   left_panel  { UIC_Button() },
        workbench   { UIC_Button() },
        right_panel { UIC_Button() }
{
    set_color({0.0, 0.0, 0.0, 0.8});

    set_w("100x");
    set_h("30x");

    set_x(">0x");
    set_y("^0x");

    appendChild(&left_panel);
    left_panel.set_x("<10x");
    left_panel.set_y("_5x");
    left_panel.set_color({0.0, 1.0, 0.0, 1.0});
    left_panel.set_click_action(CAction::UI_ToggleLeftPanel);


    appendChild(&workbench);
    workbench.set_x("<40x");
    workbench.set_y("_5x");
    workbench.set_color({0.0, 0.0, 1.0, 1.0});
    workbench.set_click_action(CAction::UI_ToggleWorkbench);


    appendChild(&right_panel);
    right_panel.set_x("<70x");
    right_panel.set_y("_5x");
    right_panel.set_color({0.0, 1.0, 0.0, 1.0});
    right_panel.set_click_action(CAction::UI_ToggleRightPanel);

}

void UIC_Topbar_GridToggle::update_component() {}


void UIC_Topbar_GridToggle::render_component(){

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents
    left_panel.render();
    right_panel.render();
    workbench.render();

    color_shader->stencil_disable();
}


UiResult UIC_Topbar_GridToggle::try_find_target_component(double x, double y) {

    if (!containsPoint(x, y))
        return UiResult();

    if (left_panel.containsPoint(x, y))
        return UiResult(true, CAction::None, &left_panel);
    
    if (right_panel.containsPoint(x, y))
        return UiResult(true, CAction::None, &right_panel);

    if (workbench.containsPoint(x, y))
        return UiResult(true, CAction::None, &workbench);

    
    return UiResult(true, CAction::None, this);
}






UIC_Topbar_MainStates::UIC_Topbar_MainStates()
    :   scene_3d    { UIC_Button_Label("Scene 3D") },
        canvas      { UIC_Button_Label("Draw") },
        ui_editor   { UIC_Button_Label("UI Editor") }
{
    set_color({0.0, 0.0, 0.0, 0.8});

    set_w("380x");
    set_h("30x");

    set_x("|");
    set_y("~");


    appendChild(&scene_3d);
    scene_3d.id = "MainStates_Scene3D";
    scene_3d.set_x("<20x");
    scene_3d.set_y("_5x");
    scene_3d.set_w("100x");
    scene_3d.set_h("20x");
    scene_3d.set_color(black);

    scene_3d.set_click_action(CAction::State_ToggleScene3D);
    
    scene_3d.string_primitive.str_setFontSize(FontSize::f18);
    scene_3d.string_primitive.update_str("Scene 3D");
    scene_3d.string_primitive.set_x("<15x");
    


    appendChild(&canvas);
    canvas.set_x("<140x");
    canvas.set_y("_5x");
    canvas.id = "MainStates_Draw";
    canvas.set_w("100x");
    canvas.set_h("20x");
    canvas.set_color(black);

    canvas.set_click_action(CAction::State_ToggleCanvas);
    
    canvas.string_primitive.str_setFontSize(FontSize::f18);
    canvas.string_primitive.update_str("Draw");
    canvas.string_primitive.set_x("<35x");




    appendChild(&ui_editor);
    ui_editor.id = "MainStates_UIEditor";
    ui_editor.set_x("<260x");
    ui_editor.set_y("_5x");
    ui_editor.set_w("100x");
    ui_editor.set_h("20x");
    ui_editor.set_color(black);

    ui_editor.set_click_action(CAction::State_ToggleUIEditor);

    ui_editor.string_primitive.str_setFontSize(FontSize::f18);
    ui_editor.string_primitive.update_str("UI Editor");
    ui_editor.string_primitive.set_x("<10x");

}

void UIC_Topbar_MainStates::update_component() {}


void UIC_Topbar_MainStates::render_component(){

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents
    scene_3d.render_recursive();
    canvas.render_recursive();
    ui_editor.render_recursive();

    color_shader->stencil_disable();
}


UiResult UIC_Topbar_MainStates::try_find_target_component(double x, double y) {

    if (!containsPoint(x, y))
        return UiResult();

    if (scene_3d.containsPoint(x, y))
        return UiResult(true, CAction::None, &scene_3d);
    
    if (canvas.containsPoint(x, y))
        return UiResult(true, CAction::None, &canvas);

    if (ui_editor.containsPoint(x, y))
        return UiResult(true, CAction::None, &ui_editor);

    
    return UiResult(true, CAction::None, this);
}

UIC_Root_Topbar::UIC_Root_Topbar() 
    :   grid_toggle { UIC_Topbar_GridToggle() },
        main_states { UIC_Topbar_MainStates() }
{
    set_color({0.0, 0.0, 0.0, 0.2});

    set_w("100%");
    set_h("30x");

    set_x("<0x");
    set_y("^0x");

    appendChild(&grid_toggle);
    appendChild(&main_states);

}

void UIC_Root_Topbar::update_component() {}


void UIC_Root_Topbar::render_component(){

    color_shader->stencil_start_new_capture();

    render();

    color_shader->stencil_apply();

    // Contents
    grid_toggle.render_component();
    main_states.render_component();

    color_shader->stencil_disable();
}


UiResult UIC_Root_Topbar::try_find_target_component(double x, double y) {

    if(!render_enabled)
        return UiResult();

    if (!containsPoint(x, y))
        return UiResult();

    if (grid_toggle.containsPoint(x, y))
        return grid_toggle.try_find_target_component(x, y);
    
    if (main_states.containsPoint(x, y))
        return main_states.try_find_target_component(x, y);
    
    return UiResult(true, CAction::None, this);
}


}
