
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "math/vecmat.hh"
#include "renderer_model.hh"
#include "window/auxwin.hh"

#include "camera.hh"

#include "renderer_axes.hh"


struct GLFWWindow;

namespace xpeditor {


class Scene {

public:

    RendererModel renderer_model;
    RenderContextModel model_render_context;

    RendererAxes renderer_axes;
    Model model;

    CameraOrbital camera;
    bool camera_grabbed = false;

    Scene();


    void handle_input(window::InputEvent input_event);

    void update();
    void render();
    
};


}