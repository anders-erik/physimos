
#pragma once

#include <vector>
#include "math/math.hh"

#include "opengl/program.hh"

// #include "rubik.hh"

namespace xpeditor {


struct Transform {
    f3 pos;
    f3 rot;
    m4f4 matrix;
    m4f4 matrix_anim;
};

struct TriangleFaceIndeces {
    int v0;
    int v1;
    int v2;
};

typedef struct Vertex {
    f3 pos;
    f3 color;
    f3 normal;

    Vertex(f3 pos) : pos {pos}, color {f3()}, normal {f3()} {};
    Vertex() : pos {f3()}, color {f3()}, normal {f3()} {};
} Vertex;

typedef struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<TriangleFaceIndeces> faces;
} Mesh;


struct RenderContextModel {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
};


struct Model {
    Transform transform;

    Mesh mesh;

    RenderContextModel rendering_context;

    void set_transform_matrix();
    void set_transform_matrix_anim();
    void set_base_color(f3 color);
};


struct RendererModel {
    opengl::Programs renderer_program = opengl::Programs::Model;
    

    unsigned int transform_location;
    unsigned int view_location;
    unsigned int perspective_location;

    void init();
    void create_render_context(Model& model);
    void set_camera_uniforms(m4f4 view_mat, m4f4 pers_mat);
    void render(Model& model);
};





void model_add_cube_mesh(Mesh& mesh);
void model_add_facelet(Mesh& mesh, f3 color, Axis axis);


}