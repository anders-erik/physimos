
#pragma once

#include <vector>
#include "math.hh"

#include "gpu.hh"

namespace xpcube {


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
    Mesh mesh;

    RenderContextModel rendering_context;
};


struct RendererModel {
    Shader renderer_program = Shader::Model;

    void init();
    void create_render_context(Model& model);
    void render(Model& model);
};






bool scene_init();


}