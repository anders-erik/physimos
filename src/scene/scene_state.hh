
#pragma once

#include "window/auxevent.hh"

#include "scene/tago.hh"

#include "scene/ss_active_tags.hh"
#include "scene/ss_key_state.hh"


struct Scene3D;
struct Manager3D;
struct InputResponse;



class SceneState
{

    InputResponse send_to_current_state(Scene3D& scene, window::InputEvent& event);

public:

    bool cursor_grab = false;

    SS::ActiveTags  active_tags;
    SS::KeyState    keys;


    bool try_peel_state(window::InputEvent& event);
    bool try_clear_state(window::InputEvent& event);

    bool try_build_state(window::InputEvent & event);


    InputResponse handle_user_input(Manager3D& manager_3D, window::InputEvent& event, TagO sampled_tag);

    void handle_window(Manager3D& manager_3D, window::WindowResizeEvent& window_event);


    void print_key_state();
    void print_active_tags();
};