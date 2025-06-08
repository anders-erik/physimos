#pragma once


#include <vector>

#include "math/vecmat.hh"

#include "log.hh"
#include "print.hh"
#include "opt.hh"
#include "pair.hh"



// Forward declares
namespace scene {
    struct Scene2D;
}
namespace window {
    struct InputEvent;
    struct CursorPosition;
}



/** Scene Owner and manager module. */
namespace ManagerScene {

size_t new_unique_id();

scene::Scene2D* try_find_scene(size_t id);

/** Initializes manager and creates the root scene. The root scene is returned. */
scene::Scene2D* init(f2 window_size);


scene::Scene2D* new_scene(scene::Scene2D& _scene);
scene::Scene2D* new_scene(f2 framebuffer_size);

[[nodiscard]] scene::Scene2D* get_root_scene();
[[nodiscard]] scene::Scene2D* get_scene(size_t id);

/** 
    Queries all scenes, starting with root (eventually window scene) and sets the scene which the cursor is currently targeting.
    This query updates the current cursor position and the scenes window box.
 */
void update_current_target(window::CursorPosition& _cursor_pos);
void clear_current_target();
bool has_grabbed_target();
scene::Scene2D* get_current_target();

void event_scroll(window::InputEvent& event);
void event_move(window::InputEvent& event);
void event_mouse_button(window::InputEvent& event);
void event_keystroke(window::InputEvent& event);
void event_window_resize(window::InputEvent& event);


};