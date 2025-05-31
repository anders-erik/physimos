
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "opengl/renderer2D.hh"
#include "opengl/texture.hh"

#include "window/auxwin.hh"

#include "math/vecmat.hh"
#include "math/shape.hh"

#include "scene/scene2D.hh"
#include "scene/camera2D.hh"
#include "scene/shapeS2D.hh"

#include "phont/phont.hh"

struct GLFWWindow;



namespace scene {

/** 
    A Scene2D and QuadS2D pair.
    Primarily resitricted interface using indrection to the scene and quad.
*/
class SubScene2D {

    Scene2D scene;
    QuadS2D quad;

public:

    SubScene2D(f2 framebuffer_size) : scene {Scene2D(framebuffer_size)} {};

    QuadS2D& get_quad();

    // QUAD CALLS
    void set_box(f2 pos, f2 size);
    bool contains_cursor(f2 pos_scene);
    f2 get_normalized_from_point(f2 pos_scene);
    m3f3 get_matrix();
    opengl::RenderContextQuadS2D& get_rendering_context();
    void set_texture_id(unsigned int id);

    // SCENE CALLS
    ShapeS2D& add_shape(Shape& shape);
    void add_quad(scene::QuadS2D& quad);
    void update();
    unsigned int render();
    void handle_scroll(float delta);
    void handle_pointer_move(scene::PointerMovement2D pointer_movement);
    void handle_pointer_click(PointerClick2D pointer_click);
};

}