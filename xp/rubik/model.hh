
#pragma once

#include <vector>
#include "math.hh"

#include "gpu.hh"



namespace xprubik {


struct Transform {
    f3 pos;
    f3 rot;
    m4f4 matrix;
};

struct TriangleFaceIndeces {
    int v0;
    int v1;
    int v2;
};

typedef struct Vertex {
    f3 pos;

    Vertex(f3 pos) : pos {pos} {};
    Vertex() : pos {f3()} {};
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
};


struct RendererModel {
    Shader renderer_program = Shader::Model;

    unsigned int transform_location;
    unsigned int view_location;
    unsigned int perspective_location;

    void init();
    void create_render_context(Model& model);
    void set_camera_uniforms(m4f4 view_mat, m4f4 pers_mat);
    void render(Model& model);
};






bool scene_init();


}