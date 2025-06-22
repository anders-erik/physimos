
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

#include "sid.hh"



struct GLFWWindow;



class SceneModelEditor
{

public:
    model::ModelColor model;
    opengl::RendererModelColor renderer_model_color;
    opengl::RenderContextModelColor model_render_context;
    opengl::RendererVector renderer_vector;

    model::ModelT ground;
    opengl::RendererModelTextureContext ground_render_context;

    model::ModelT tube;
    opengl::RendererModelTextureContext tube_render_context;

    opengl::RendererModelTexture renderer_model_texture;

    opengl::RendererAxes renderer_axes;

    CameraOrbital camera;
    bool camera_grabbed = false;

    SceneModelEditor();


    void handle_input(window::InputEvent input_event);
    void handle_window_resize(window::WindowResizeEvent resize_event);

    void update();
    void render();
    
};

