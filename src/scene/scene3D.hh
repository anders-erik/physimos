
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "opengl/renderer_model_texture.hh"
#include "opengl/renderer_vector.hh"
#include "opengl/renderer_model_color.hh"

#include "math/vecmat.hh"
#include "window/auxwin.hh"

#include "camera.hh"

#include "renderer_axes.hh"

#include "scene_base.hh"

#include "scene/render3D/program_model_texture.hh"
#include "scene/render3D/program_model_color.hh"
#include "scene/render3D/program_axes.hh"

struct GLFWWindow;



class Scene3D : public SceneBase
{

public:

    model::ModelColor model;

    ProgramModelColor renderer_model_color;
    RenderContextModelColor model_render_context;
    opengl::RendererVector renderer_vector;


    ProgramModelTexture renderer_model_texture;

    model::ModelTexture ground;
    ProgramModelTextureContext ground_render_context;

    model::ModelTexture tube;
    ProgramModelTextureContext tube_render_context;


    ProgramAxes renderer_axes;

    CameraOrbital camera;
    bool camera_grabbed = false;


    Scene3D();


    void handle_input(window::InputEvent input_event);
    void handle_window_resize(window::WindowResizeEvent resize_event);

    void update();
    void render();
    
};

