#pragma once

#include "window/auxevent.hh"

#include "rend/scene3D/renderer3D.hh"
#include "scene/scene_state.hh"
#include "scene/object_manager.hh"
#include "scene/scene3D.hh"

#include "scene2D/manager_2D.hh"


class Manager3D
{
    bool init_flag = false;

    SID sid_3D_counter = 2; // Global index count
    SID new_sid_3D();

public:

    RendererScene3D renderer_3D;

    SceneState state;

    Scene3D root_scene;
    Scene3D* window_scene = nullptr;

    ManagerO manager_o;

    std::list<Scene3D> scenes; // list of non-root 3D scenes

    /** Initializes manager and creates the root scene. The root scene is returned. */
    Scene3D& init(f2 window_scene_f);

    /** Also samples the  */
    void render_window_scene(i2 window_scene_i, window::CursorPosition cursor_pos);

};

