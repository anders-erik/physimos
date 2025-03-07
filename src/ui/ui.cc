#include <iostream>

#include "glad/glad.h"  // glDisable(GL_DEPTH_TEST)
#include "lib/process.hh"
#include "lib/log.hh"

#include "ui.hh"

#include "ui/ui_globals.hh"
#include "ui/ui_texture.hh"
#include "ui/ui_shader_texture.hh"
#include "ui/ui_shader_color.hh"
#include "ui/ui_font.hh"
#include "ui/ui_primitive.hh"

#include "ui/uic/topbar/uic_topbar.hh"
#include "ui/uic/main_view/uic_main_view.hh"
#include "ui/uic/right_panel/uic_right_panel.hh"
#include "ui/uic/uic_root_left_panel.hh"
#include "ui/uic/uic_root_workbench.hh"

#include "ui/uic/uic_primitive_editor.hh"
#include "ui/uic/uic_primitive_list.hh"
#include "ui/uic/uic_primitive_list_editor.hh"


#include "Input.hpp"
#include "Windowing.hpp"


#include "PSO_util.hpp"




namespace UI {


/** Returned primitive from finding primitive target during left click. Is reset to nullptr on left release. */
Primitive* grabbed_primitive = nullptr;

/** Current cursor position recieved in the input callback function. */
double cursor_x = 0.0;
/** Current cursor position recieved in the input callback function. y = 0 at the bottom of window.  */
double cursor_y = 0.0;

Primitive* currentlyHoveredPrimitive = nullptr;



// ROOT PRIMITIVES

UI::component::UIC_Root_Topbar*      topbar;
UI::component::UIC_Root_MainView*    main_view;
UI::component::UIC_Root_LeftPanel*   left_panel;
UI::component::UIC_Root_RightPanel*  right_panel;
UI::component::UIC_Root_Workbench*   workbench;

UiResult try_find_target(double x, double y){

    UiResult result;

    
    result = main_view->try_find_target_component(x, y);
    if (result.success)
        return result;

    result = topbar->try_find_target_component(x, y);
    if (result.success)
        return result;

    result = left_panel->try_find_target_component(x, y);
    if (result.success)
        return result;
    
    result = right_panel->try_find_target_component(x, y);
    if (result.success)
        return result;
    
    result = workbench->try_find_target_component(x, y);
    if (result.success)
        return result;


    return UiResult();
}


void state_main_set(StateMain new_state_main){

    main_view->set_current_state(new_state_main);
    right_panel->set_current_state(new_state_main);

    // TOPBAR BUTTONS

    // Reset topbar buttons highlight
    topbar->main_states.uic_Topbar_MainStates_Scene3D.set_state(PrimitiveState::Default);
    topbar->main_states.uic_Topbar_MainStates_Canvas.set_state(PrimitiveState::Default);
    topbar->main_states.uic_Topbar_MainStates_UIEditor.set_state(PrimitiveState::Default);
    
    // Set new topbar button highlight
    switch (new_state_main){

        case StateMain::Scene3D :
            topbar->main_states.uic_Topbar_MainStates_Scene3D.set_state(PrimitiveState::Selected);
            break;
        case StateMain::Draw :
            topbar->main_states.uic_Topbar_MainStates_Canvas.set_state(PrimitiveState::Selected);
            break;
        case StateMain::UIEditor :
            topbar->main_states.uic_Topbar_MainStates_UIEditor.set_state(PrimitiveState::Selected);
            break;
    
    default:
        break;
    }
}

void init(){

    PhysWin new_window = get_initial_physimos_window();
    viewport_width = new_window.width / new_window.xscale;
    viewport_height = new_window.height / new_window.yscale;

    // subscribeWindowChange_ui(callback_window_change); // WINDOWING.CPP


    // characters-2.bmp character map
    UI::font::init_font();
    
    UI::texture::init_static_color_textures();
    UI::texture::init_static_icon_textures();

    shader::texture_shader.set_window_info(new_window.width, new_window.height, new_window.xscale, new_window.yscale);
    shader::texture_shader.compile_shader();
    shader::texture_shader.init();

    shader::color_shader.set_window_info(new_window.width, new_window.height, new_window.xscale, new_window.yscale);
    shader::color_shader.compile_shader();
    shader::color_shader.init();

    shader::string_shader.set_window_info(new_window.width, new_window.height, new_window.xscale, new_window.yscale);
    shader::string_shader.init();
    // shader::string_shader.set_bitmap(get_font_bitmap());
    shader::string_shader.set_texture(font::get_font_texture());


    topbar      = new UI::component::UIC_Root_Topbar();
    main_view   = new UI::component::UIC_Root_MainView();
    left_panel  = new UI::component::UIC_Root_LeftPanel();
    right_panel = new UI::component::UIC_Root_RightPanel();
    workbench   = new UI::component::UIC_Root_Workbench();
    


    // REMOVING THESE CAUSES A LINKING ERROR I"VE NOT BEEN ABLE TO RESOLVE...!!
    // OLD PRIMITIVES FOR DEV
    component::UIC_PrimitiveList* primitive_list;
    Primitive* primitive_to_edit = new Primitive();
    component::UIC_PrimitiveEditor* primitive_editor = new component::UIC_PrimitiveEditor(*primitive_to_edit);
    component::UIC_PrimitiveListEditor* primitive_list_editor = new component::UIC_PrimitiveListEditor(*primitive_list, *primitive_to_edit);
}

void set_ui_views(ViewportViewSizes view_sizes, ViewportViewVisibility visibility){

    if(visibility.left_panel)
        left_panel->render_enabled = true;
    else 
        left_panel->render_enabled = false;

    if(visibility.workbench)
        workbench->render_enabled = true;
    else 
        workbench->render_enabled = false;

    if(visibility.right_panel)
        right_panel->render_enabled = true;
    else 
        right_panel->render_enabled = false;


    topbar->set_w("100%");
    topbar->set_h(std::to_string(view_sizes.topbar_h) + "x");
    topbar->set_x("<0x");
    topbar->set_y("^0x");

    left_panel->set_w(std::to_string(view_sizes.left_panel_w) + "x");
    left_panel->set_h("100%o-" + std::to_string(view_sizes.topbar_h));
    left_panel->set_x("<0x");
    left_panel->set_y("_0x");

    right_panel->set_w(std::to_string(view_sizes.right_panel_w) + "x");
    right_panel->set_h("100%o-" + std::to_string(view_sizes.topbar_h));
    right_panel->set_x(">0x");
    right_panel->set_y("_0x");

    workbench->set_w(std::to_string(view_sizes.workbench.w) + "x");
    workbench->set_h(std::to_string(view_sizes.workbench.h) + "x");
    workbench->set_x("<" + std::to_string(view_sizes.workbench.x) + "x");
    workbench->set_y("_0x");

    main_view->set_w(std::to_string(view_sizes.main_view.w) + "x");
    main_view->set_h(std::to_string(view_sizes.main_view.h) + "x");
    main_view->set_x("<" + std::to_string(view_sizes.main_view.x) + "x");
    main_view->set_y("_" + std::to_string(view_sizes.main_view.y) + "x");

    
}



void update(){
    glDisable(GL_DEPTH_TEST);

    topbar->render_component();
    main_view->render_component();
    left_panel->render_component();
    right_panel->render_component();
    workbench->render_component();
    
}



void update_window(PhysWin physimos_window) {

    viewport_width = physimos_window.logical.w;
    viewport_height = physimos_window.logical.h;
    
    // SHADER TRANSFORM
    shader::texture_shader.set_window_info(
        physimos_window.raw.w,
        physimos_window.raw.h,
        physimos_window.xscale,
        physimos_window.yscale
    );

    shader::color_shader.set_window_info(
        physimos_window.raw.w, 
        physimos_window.raw.h, 
        physimos_window.xscale, 
        physimos_window.yscale
    );

    shader::string_shader.set_window_info(
        physimos_window.raw.w, 
        physimos_window.raw.h, 
        physimos_window.xscale, 
        physimos_window.yscale
    );

    // RELOAD ROOT COMPONENTS
    topbar->uiTransform.size_has_been_changed = true;
    main_view->uiTransform.size_has_been_changed = true;
    left_panel->uiTransform.size_has_been_changed = true;
    right_panel->uiTransform.size_has_been_changed = true;
    workbench->uiTransform.size_has_been_changed = true;
}




} // End UI namespace

