
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "math/math.hh"
#include "model_editor.hh"
#include "window/auxwin.hh"

#include "camera.hh"

#include "renderer_axes.hh"


struct GLFWWindow;

namespace xpeditor {


class Scene {

public:

    RendererModel renderer_model;
    RendererAxes renderer_axes;
    Model model;

    CameraOrbital camera;
    bool camera_grabbed = false;

    Scene();


    void handle_input(window::InputEvent input_event);

    void update();
    void render();

    void set_viewport_dims(int _width, int _height);
    
};


}