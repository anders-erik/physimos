
#pragma once

#include <array>

#include "math/vecmat.hh"
#include "math/transform.hh"

#include "opengl/program.hh"


namespace opengl {


struct VertexQuad2D {
    f3 pos;
    f2 tex;
};

struct Quad2DRenderContext {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int texture;

    int element_count;

    void delete_contents();
};



struct Quad2D {
    std::array<VertexQuad2D, 6> verts;

    Transform2D transform_2d;

    Quad2DRenderContext render_context;

    Quad2D();

    void set_dims(float window_width, float window_height, float width_pixels, float height_pixels);

    static std::array<VertexQuad2D, 6> generate_quad();

};


class Quad2DRenderer {
    opengl::Programs shader = opengl::Programs::Texture2D;
    unsigned int program; // opengl id for renderer

public:

    void create_context(Quad2D& quad);

    void activate();
    void set_camera(m3f3 camera);
    void set_model(m3f3 model_mat);

    void set_model_camera(m3f3 model_mat, m3f3 camera);
    void render(Quad2DRenderContext context);
    void render_multisample_texture(Quad2DRenderContext context);
    

    Quad2DRenderer();
};


}