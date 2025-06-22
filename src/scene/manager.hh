#pragma once


#include <vector>
#include <list>

#include "math/vecmat.hh"

#include "log.hh"
#include "print.hh"
#include "opt.hh"
#include "pair.hh"

#include "window/auxevent.hh"

#include "scene3D.hh"

#include "scene2D/manager_2D.hh"


struct Manager2D;


namespace window {
    struct InputEvent;
    struct CursorPosition;
}




/** Scene Owner and manager module. */
namespace ManagerScene 
{


SID new_sid_3D();

/** Initializes manager and creates the root scene. The root scene is returned. */
Scene3D& init(f2 window_size);

/** Immutable root scene. */
[[nodiscard]] const Scene3D& get_root_scene();
/** Mutable root scene. */
[[nodiscard]] Scene3D& get_root_scene_mut();

/** Immutable window scene. */
[[nodiscard]] const Scene3D& get_window_scene();
/** Mutable window scene */
[[nodiscard]] Scene3D& get_window_scene_mut();


/** Relays an event to the window scene */
InputResponse event_send_window_scene(window::InputEvent& event);

void event_window_resize(window::WindowResizeEvent& window_resize);


[[nodiscard]] Manager2D& get_manager_2D();

};