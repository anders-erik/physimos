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
    class QuadManager;
}
using namespace scene;

namespace window {
    struct InputEvent;
    struct CursorPosition;
}



/** Scene Owner and manager module. */
namespace ManagerScene 
{


size_t new_unique_id();

/** Initializes manager and creates the root scene. The root scene is returned. */
scene::Scene2D* init(f2 window_size);

/** Immutable root scene. */
[[nodiscard]] const scene::Scene2D& get_root_scene();
/** After init(), this call should never fail. */
[[nodiscard]] scene::Scene2D& get_root_scene_mut();

/** Immutable window scene. */
[[nodiscard]] const scene::Scene2D& get_window_scene();
/** Mutable window scene */
[[nodiscard]] scene::Scene2D& get_window_scene_mut();

/** Find the scene which the cursor is targeting. 
    Defaults to window scene if cursor scene not found. */
[[nodiscard]] scene::Scene2D& get_cursor_scene_mut();


/** Tries to find scene with matching scene_id in storage.
    Returns `nullptr` if no match is found. */
[[nodiscard]] scene::Scene2D* search_scene_storage(size_t id);

/** Copies the scene into storage
    Returns the scene id for later queries. */
size_t push_scene(scene::Scene2D& _scene);


scene::QuadManager& get_quad_manager();


/** 
    Queries window scene for matching subscenes, and set it up for event handling.
    If match, then that subscene is the new target. If no match then window scene is target.
 */
void requery_cursor_target(window::CursorPosition& _cursor_pos);

/** Clear current cursor scene from hovers, and set cursor scene to be window, then clear window. */
void clear_cursor_hovers();

bool is_grabbing_cursor();



void event_scroll(window::InputEvent& event);
void event_move(window::InputEvent& event);
void event_mouse_button(window::InputEvent& event);
void event_keystroke(window::InputEvent& event);

/** Confirmed targeting scene. */
void events_user_all(window::InputEvent& event);

void event_window_resize(window::InputEvent& event);


};