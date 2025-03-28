
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "opengl/renderer_model_texture.hh"

#include "math/vecmat.hh"
#include "renderer_model.hh"
#include "window/auxwin.hh"

#include "camera.hh"

#include "renderer_axes.hh"


struct GLFWWindow;

namespace xpeditor {


class Scene {

public:
    ModelColor model;
    RendererModel renderer_model;
    RenderContextModel model_render_context;

    ModelTexture ground;
    opengl::RendererModelTextureContext ground_render_context;

    ModelTexture tube;
    opengl::RendererModelTextureContext tube_render_context;

    opengl::RendererModelTexture renderer_model_texture;

    RendererAxes renderer_axes;

    CameraOrbital camera;
    bool camera_grabbed = false;

    Scene();


    void handle_input(window::InputEvent input_event);

    void update();
    void render();
    
};


}