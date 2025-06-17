#pragma once


#include <vector>

#include "math/vecmat.hh"

#include "log.hh"
#include "print.hh"
#include "opt.hh"
#include "pair.hh"

#include "window/auxevent.hh"

#include "scene3D.hh"


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


struct InputStateSceneManager : InputState 
{
    enum Type {
        Scene2D,
        Scene3D,
        Subwin,
        NONE,
    } type;

    void update(Type new_type, InputResponse new_response)
    {
        type = new_type;
        last_response = new_response;

        if(is_all_release())
            type = NONE;
    }


    bool is_subwin() {return type == Subwin ? true : false;};
    bool is_scene2D() {return type == Scene2D ? true : false;};

};



/** Scene Owner and manager module. */
namespace ManagerScene 
{


SceneID new_unique_id();

/** Initializes manager and creates the root scene. The root scene is returned. */
Scene3D& init(f2 window_size);

/** Immutable root scene. */
[[nodiscard]] const Scene3D& get_root_scene();
/** After init(), this call should never fail. */
[[nodiscard]] Scene3D& get_root_scene_mut();

/** Immutable window scene. */
[[nodiscard]] const SceneBase& get_window_scene();
/** Mutable window scene */
[[nodiscard]] SceneBase& get_window_scene_mut();


/** Tries to find scene with matching scene_id in storage.
    Returns `nullptr` if no match is found. */
[[nodiscard]] SceneBase* search_scene_storage(SceneID id);


/** Copies the scene into storage
    Returns the scene id for later queries. */
SceneID push_scene2D(scene::Scene2D& _scene);



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
InputResponse events_user_all(window::InputEvent& event);

void event_window_resize(window::WindowResizeEvent& window_resize);


};