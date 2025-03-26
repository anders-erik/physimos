
#pragma once

#include <array>

#include "math/math.hh"

namespace phont {

struct Quad;
struct QuadRenderer;
struct QuadRenderContext;


struct Vertex {
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

struct Quad {
    std::array<phont::Vertex, 4> verts;
    std::array<i3, 2> faces;

    phont::QuadRenderContext render_context;

    Quad(float window_width, float window_height);
};


struct QuadRenderer {
    unsigned int program;

    void create_context(phont::Quad& quad);

    void render_context(QuadRenderContext context);

    QuadRenderer();
};



void set_texture_checker(unsigned int& texture_id);

unsigned int get_texture_F();


}
