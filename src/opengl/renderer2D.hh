
#pragma once

#include <array>

#include "math/vecmat.hh"
#include "math/transform.hh"
#include "math/shape.hh"

#include "opengl/program.hh"


namespace opengl {


struct Vertex2DT {
    f3 pos;
    f2 tex;
};

struct ShapeS2DRenderContext {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int texture;

    int element_count;

    void delete_contents();
};




class Scene2DRenderer {
    opengl::ProgramName shader = opengl::ProgramName::Texture2D;
    unsigned int program; // opengl id for renderer

public:
    Scene2DRenderer();

    void create_context_quad_t(ShapeS2DRenderContext& render_context, std::array<Vertex2DT, 6> verts);
    void create_shape_context_t(ShapeS2DRenderContext& render_context, std::vector<Vertex2DT> verts);
    void create_wire_quad_context_t(ShapeS2DRenderContext& render_context, std::array<Vertex2DT, 8> verts);

    void activate();
    void set_camera(m3f3 camera);
    void set_model(m3f3 model_mat);

    void render_quad(ShapeS2DRenderContext context);
    void render_point(ShapeS2DRenderContext context);
    void render_line(ShapeS2DRenderContext context);
    void render_frame(ShapeS2DRenderContext frame_context);
    void render_multisample_texture(ShapeS2DRenderContext context);
    
};


}