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

/** Initializes manager and creates the root scene. The root scene is returned. */
scene::Scene2D* init(f2 window_size);

const scene::Scene2D& get_root_scene();

/** After init(), this call should never fail. */
[[nodiscard]] scene::Scene2D& get_root_scene_mut();

/** Tries to find scene with matching scene_id in storgage.
    Resutnrs `nullptr` if no match is found. */
[[nodiscard]] scene::Scene2D* try_find_scene(size_t id);

/** Copies the scene into storage
    Returns the scene id for later queries. */
size_t push_scene(scene::Scene2D& _scene);




/** 
    Queries (to be) window scene for matching subscenes.
    If match found that subscene is the new target; else the window scene;
    This query updates the current cursor position and the scenes window box.
 */
void update_current_target(window::CursorPosition& _cursor_pos);
void clear_cursor_highlighting();
bool is_grabbing_cursor();
scene::Scene2D* get_current_target();


void event_scroll(window::InputEvent& event);
void event_move(window::InputEvent& event);
void event_mouse_button(window::InputEvent& event);
void event_keystroke(window::InputEvent& event);
void event_window_resize(window::InputEvent& event);


};