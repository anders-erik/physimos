
#pragma once

#include "scene/tago.hh"

#include "scene/ss_hovered.hh"
#include "scene/ss_selected.hh"
#include "scene/ss_key_state.hh"
#include "scene/ss_camera.hh"


namespace window    {   
                        struct InputEvent;
                        struct WindowResizeEvent;   
                    }

struct Scene3D;
struct Manager3D;
struct InputResponse;



class SceneState
{

public:

    bool cursor_grab = false;

    SS::Hovered     hovered;
    SS::Selected    selected;
    SS::KeyState    keys;

    CameraState     camera;


    bool try_change_selected(window::InputEvent& event);
    bool try_move_camera(window::InputEvent& event);
    bool try_update_selected(window::InputEvent& event);

    InputResponse handle_user_input(Manager3D& manager_3D, window::InputEvent& event, TagO sampled_tag);

    void handle_window(Manager3D& manager_3D, window::WindowResizeEvent& window_event);

    void try_print(window::InputEvent& event);
    void print_selected_state();
    void print_active_tags();
};