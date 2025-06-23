
#pragma once

#include "window/auxevent.hh"

#include "scene/tago.hh"

#include "scene/ss_active_tags.hh"


struct Scene3D;
struct Manager3D;
struct InputResponse;



class SceneState
{

    InputResponse old_scene_handler(Scene3D& scene, window::InputEvent& event);

public:

    bool camera_grabbed = false;

    SS::ActiveTags active_tags;

    /** Returns true on successful grab. Updates state. */
    bool try_new_quad_grab(window::InputEvent& event);
    /** Returns true on released quad. Updates state. */
    bool try_release_quad(window::InputEvent& event);
    bool try_release_quad_esc(window::InputEvent& event);

    InputResponse handle_mouse(Manager3D& manager_3D, window::InputEvent& event);
    InputResponse handle_key(Manager3D& manager_3D, window::InputEvent& event);

    void handle_window(Manager3D& manager_3D, window::WindowResizeEvent& window_event);
};