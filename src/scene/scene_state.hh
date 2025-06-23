
#pragma once

#include "window/auxevent.hh"

#include "scene/tago.hh"

// #include 

struct Scene3D;
struct InputResponse;

class SceneState
{

    InputResponse old_scene_handler(Scene3D& scene, window::InputEvent& event);

public:

    bool camera_grabbed = false;

    TagO selected_tag;
    TagO hovered_tag;
    TagO capturing_quad_tag;

    InputResponse handle(Scene3D& scene, window::InputEvent& event);

    void handle_window(Scene3D& scene, window::WindowResizeEvent& window_event);
};