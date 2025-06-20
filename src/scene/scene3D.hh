
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "opengl/renderer_model_texture.hh"
#include "opengl/renderer_vector.hh"
#include "opengl/renderer_model_color.hh"

#include "math/vecmat.hh"

#include "window/auxevent.hh"

#include "camera.hh"

#include "renderer_axes.hh"

#include "scene_base.hh"

#include "rend/scene3D/program_model_texture.hh"
#include "rend/scene3D/program_model_color.hh"
#include "rend/scene3D/program_axes.hh"

#include "scene/object_manager.hh"


class Scene3D : public SceneBase
{

public:

    std::vector<Object> objects;

    Object selected_object;
    Object hovered_object;
    Object capturing_quad;

    std::vector<model::ModelT> texture_models;
    std::vector<model::ModelColor> color_models;

    model::ModelT tube;


    CameraOrbital camera;
    bool camera_grabbed = false;


    Scene3D() = default;

    void push_object(Object new_object);
    std::vector<Object>& get_objects_mut();

    void push_texture_model(model::ModelT& new_model);
    void push_color_model(model::ModelColor& new_model);

    InputResponse handle_input(window::InputEvent input_event);
    void handle_window_resize(window::WindowResizeEvent resize_event);

    void update();
    
};

