
#pragma once

#include <array>
#include <vector>

#include "math/vecmat.hh"
#include "math/transform.hh"
#include "opengl/texture.hh"


namespace opengl {

// struct Quad;
// struct QuadRenderer;
// struct QuadRenderContext;


struct VertexQuad {
    f3 pos;
    f2 tex;
};

struct QuadRenderContext {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int texture;

    int element_count;

    void delete_contents();
};

struct Quad2D {
    std::array<VertexQuad, 6> verts;

    Transform2D transform_2d;

    QuadRenderContext render_context;

    Quad2D(float window_width, float window_height);

    void set_dims(float window_width, float window_height, float width_pixels, float height_pixels);

    static std::array<VertexQuad, 6> generate_quad();

};


struct QuadRenderer {
    unsigned int program;

    void create_context(Quad2D& quad);

    void set_model_camera(m3f3 model_mat, m3f3 camera);
    void render(QuadRenderContext context);

    QuadRenderer();
};


}


namespace scene {

/** Captures [-1, 1]x[-1,1]x[-1, -1] by default. */
struct Camera2D {
    Transform2D transform_2d;

    void zoom(float zoom_scale);
};

class Scene2D {
public:
    Camera2D camera_2d;

    Scene2D() {};
};

};

namespace phont {



struct GlyphMesh {
    std::vector<f3> verts;
    std::vector<i3> faces;
};




void set_texture_checker(unsigned int& texture_id);

opengl::TextureFrameBuffer get_texture_F();
void get_mesh_F(GlyphMesh& mesh);


}
