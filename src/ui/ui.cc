#include <iostream>

#include "glad/glad.h"  // glDisable(GL_DEPTH_TEST)
#include "lib/process.hh"
#include "lib/log.hh"

#include "ui.hh"

#include "ui/ui_globals.hh"
#include "ui/ui_texture.hh"
#include "ui/ui_shader_texture.hh"
#include "ui/ui_shader_color.hh"
#include "ui/ui_texture_string.hh"
#include "ui/ui_primitive.hh"

#include "ui/uic/topbar/uic_topbar.hh"
#include "ui/uic/uic_root_main_view.hh"
#include "ui/uic/uic_root_left_panel.hh"
#include "ui/uic/uic_root_right_panel.hh"
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


// TESTING PRIMITIVES

UI::Primitive* color_primtiive;
UI::Primitive* primitive_to_edit;

UI::component::UIC_PrimitiveEditor* primitive_editor;

UI::component::UIC_PrimitiveList* primitive_list;
UI::component::UIC_PrimitiveListEditor* primitive_list_editor;

// ROOT PRIMITIVES

UI::component::UIC_Root_Topbar*      topbar;
UI::component::UIC_Root_MainView*    main_view;
UI::component::UIC_Root_LeftPanel*   left_panel;
UI::component::UIC_Root_RightPanel*  right_panel;
UI::component::UIC_Root_Workbench*   workbench;

UiResult try_find_target(double x, double y){

    UiResult result;

    result = topbar->try_find_target_component(x, y);
    if (result.success)
        return result;

    result = workbench->try_find_target_component(x, y);
    if (result.success)
        return result;


    return UiResult();
}

UiResult try_find_target_old(double x, double y){

    UiResult result;

    result = primitive_editor->try_find_target_component(x, y);
    if(result.success)
        return result;

    result = primitive_list_editor->try_find_target_component(x, y);
    if(result.success)
        return result;

    result = primitive_list->try_find_target_component(x, y);
    if(result.success)
        return result;


    return UiResult();
}

void state_main_set(StateMain new_state_main){

    // Reset topbar buttons highlight
    topbar->main_states.uic_Topbar_MainStates_Scene3D.set_state(PrimitiveState::Default);
    topbar->main_states.uic_Topbar_MainStates_Canvas.set_state(PrimitiveState::Default);
    topbar->main_states.uic_Topbar_MainStates_UIEditor.set_state(PrimitiveState::Default);
    
    // Set new topbar button highlight
    switch (new_state_main){

        case StateMain::Scene3D :
            topbar->main_states.uic_Topbar_MainStates_Scene3D.set_state(PrimitiveState::Selected);
            break;
        case StateMain::Canvas :
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
    
    UI::texture::init_static_color_textures();
    UI::texture::init_static_icon_textures();

    shader::texture_shader.set_window_info(new_window.width, new_window.height, new_window.xscale, new_window.yscale);
    shader::texture_shader.compile_shader();
    shader::texture_shader.init();

    shader::color_shader.set_window_info(new_window.width, new_window.height, new_window.xscale, new_window.yscale);
    shader::color_shader.compile_shader();
    shader::color_shader.init();

    // characters-2.bmp character map
    UI::init_font();



    // COLOR PRIMITIVE
    color_primtiive = new UI::Primitive();
    color_primtiive->render_enabled = false;
    color_primtiive->has_texture = false;
    color_primtiive->set_color({0.5,0.5,1.0,1.0});
    color_primtiive->id = "color_primitive";
    color_primtiive->set_x("<300x");
    color_primtiive->set_y("_1%");
    color_primtiive->set_h("98%");
    color_primtiive->set_w("100x");

    
    // PRIMITIVE TO EDIT
    primitive_to_edit = new UI::Primitive();
    primitive_to_edit->id = "primitive_to_edit";
    // primitive_to_edit->set_color(active_pallete.base1);
    primitive_to_edit->set_x("<50%");
    primitive_to_edit->set_y("_50%");
    primitive_to_edit->set_h("200xo-10");
    primitive_to_edit->set_w("10%o9");

    primitive_editor = new UI::component::UIC_PrimitiveEditor(*primitive_to_edit);
    primitive_editor->id = "editor_id";
    primitive_editor->set_x("<74%");
    primitive_editor->set_y("^10%");
    primitive_editor->set_h("50%");
    primitive_editor->set_w("25%");
    primitive_editor->uiTransform.w_min = 100;
    primitive_editor->uiTransform.h_min = 100;
    primitive_editor->stencil_test = true;

    std::cout << "sizeof(primitive_editor = " << sizeof(*primitive_editor) << std::endl;



    // PRIMITIVE LIST
    primitive_list = new UI::component::UIC_PrimitiveList(*primitive_to_edit);
    primitive_list->id = "primitive_list";
    primitive_list->set_x("<50x");
    primitive_list->set_y("10x");
    primitive_list->set_h("200x");
    primitive_list->set_w("200x");

    // PRIMITIVE LIST EDITOR
    primitive_list_editor = new UI::component::UIC_PrimitiveListEditor(*primitive_list, *primitive_to_edit);
    primitive_list_editor->id = "primitive_list_editor";
    primitive_list_editor->set_x("<300x");
    primitive_list_editor->set_y("_20%");
    primitive_list_editor->set_h("100xo-10");
    primitive_list_editor->set_w("20%o9");

    topbar      = new UI::component::UIC_Root_Topbar();
    main_view   = new UI::component::UIC_Root_MainView();
    left_panel  = new UI::component::UIC_Root_LeftPanel();
    right_panel = new UI::component::UIC_Root_RightPanel();
    workbench   = new UI::component::UIC_Root_Workbench();
    
    
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


void render_main_view_components(){

    primitive_to_edit->render();
    color_primtiive->render();


    primitive_editor->update_component();
    primitive_editor->render_component();

    primitive_list->render_component();
    primitive_list_editor->render_component();

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

    viewport_width = physimos_window.width / physimos_window.xscale;
    viewport_height = physimos_window.height / physimos_window.yscale;
    
    // SHADER TRANSFORM
    shader::texture_shader.set_window_info(
        physimos_window.width,
        physimos_window.height,
        physimos_window.xscale,
        physimos_window.yscale
    );

    shader::color_shader.set_window_info(
        physimos_window.width, 
        physimos_window.height, 
        physimos_window.xscale, 
        physimos_window.yscale
    );

    // RELOAD ALL PRIMITIVES TO GET CORRECT DIMENSIONS
    primitive_editor->update_w_real_recursive();
    primitive_editor->update_x_real_recursive();

    primitive_editor->update_h_real_recursive();
    primitive_editor->update_y_real_recursive();

    // RELOAD ROOT COMPONENTS
    topbar->uiTransform.size_has_been_changed = true;
    main_view->uiTransform.size_has_been_changed = true;
    left_panel->uiTransform.size_has_been_changed = true;
    right_panel->uiTransform.size_has_been_changed = true;
    workbench->uiTransform.size_has_been_changed = true;
}




} // End UI namespace

