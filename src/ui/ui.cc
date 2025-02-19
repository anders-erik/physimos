#include "glad/glad.h"  // glDisable(GL_DEPTH_TEST)
#include "log.hh"

#include "ui.hh"

#include "ui/ui_globals.hh"
#include "ui/ui_texture.hh"
#include "ui/ui_shader.hh"
#include "ui/font.hh"
#include "ui/ui_primitive.hh"

#include "ui/uic/uic_primitive_editor.hh"


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

UI::Primitive* primitive_to_edit;
UI::component::UIC_PrimitiveEditor* primitive_editor;



void init(){

    PhysWin new_window = get_initial_physimos_window();
    subscribeWindowChange_ui(callback_window_change); // WINDOWING.CPP
    
    UI::texture::init_static_textures();

    shader::texture_shader.set_window_info(new_window.width, new_window.height, new_window.xscale, new_window.yscale);
    shader::texture_shader.compile_shader();
    shader::texture_shader.init();

    // characters-2.bmp character map
    UI::loadFont();


    // Subscribe to cursor position from input library
    ::PInput::subscribe_pointer_position_ui(callback_pointer_position);
    ::PInput::subscribe_mouse_left_down_ui(callback_left_down);
    ::PInput::subscribe_mouse_left_release_ui(callback_left_release);
    ::PInput::subscribe_mouse_scroll_y_ui(callback_scroll_y);

    
    // PRIMITIVE EDITOR COMPONENT
    primitive_to_edit = new UI::Primitive();
    primitive_to_edit->set_color(Colors::DarkGray);
    primitive_to_edit->id = "primitive_to_test";
    primitive_to_edit->set_x("<500x");
    primitive_to_edit->set_y("_400x");
    primitive_to_edit->set_h("200x");
    primitive_to_edit->set_w("200x");


    primitive_editor = new UI::component::UIC_PrimitiveEditor(*primitive_to_edit);
    primitive_to_edit->id = "primitive_editor";
    primitive_editor->set_x("<800x");
    primitive_editor->set_y("_200x");
    primitive_editor->set_h("400x");
    primitive_editor->set_w("200x");

}

void update(){
    glDisable(GL_DEPTH_TEST);

    primitive_to_edit->render();

    primitive_editor->update_component();
    primitive_editor->render_component();

}

void callback_pointer_position(PInput::PointerPosition pointer_pos, PInput::PointerChange _pointer_change) {

    // UI STATE
    cursor_x = pointer_pos.x;
    cursor_y = pointer_pos.y;

    // DRAG
    if(grabbed_primitive != nullptr){
        grabbed_primitive->grabbed(_pointer_change.dx, _pointer_change.dy);
    }
        
    // RESET HOVER
    if (currentlyHoveredPrimitive != nullptr) {
        currentlyHoveredPrimitive->hover_exit();
        currentlyHoveredPrimitive = nullptr;
    }

    // TRY NEW HOVER
    UiResult targetResult = primitive_editor->try_find_target_component(cursor_x, cursor_y);
    if (targetResult.success) {
        currentlyHoveredPrimitive = targetResult.primitive;
        targetResult.primitive->hover_enter();
    }

}


void callback_scroll_y(double y_change) {

    // TRY SCROLL PRIMITIVE
    UiResult scrollTargetQuery = primitive_editor->try_find_target_component(cursor_x, cursor_y);
    if (scrollTargetQuery.success) {
        scrollTargetQuery.primitive->scroll(y_change);
    }
    
}

void callback_left_release(PInput::PointerPosition _pointer_pos){

    // We are only interested in release behavior if we targeted a primitive on left click
    if (grabbed_primitive == nullptr)
        return;
    

    // TRIGGER CLICK
    UiResult releaseTargetResult = primitive_editor->try_find_target_component(cursor_x, cursor_y);
    // We click if: 1) released on primitive, 2) primitive is the same one as registered on left down
    bool click_confirmed = releaseTargetResult.success && releaseTargetResult.primitive == grabbed_primitive;
    if (click_confirmed) {
        plog_info(::plib::LogScope::UI, "Released on " + releaseTargetResult.primitive->id);
        releaseTargetResult.primitive->click();
    }
    
    // NEVER PERSIST GRAB ON RELEASE
    grabbed_primitive = nullptr;
}

void callback_left_down(PInput::PointerPosition _pointer_pos) {

    // REGISTER FOR CLICK ON RELEASE
    UiResult clickTargetResult = primitive_editor->try_find_target_component(cursor_x, cursor_y);
    if(clickTargetResult.success){
        grabbed_primitive = clickTargetResult.primitive;
        plog_info(::plib::LogScope::UI, "Grabbed : " + grabbed_primitive->id);
    }

}

void callback_window_change(PhysWin physimos_window) {
    
    // SHADER TRANSFORM
    shader::texture_shader.set_window_info(
        physimos_window.width,
        physimos_window.height,
        physimos_window.xscale,
        physimos_window.yscale
    );
}




} // End UI namespace

