
#pragma once

#include <vector>
#include "math/vecmat.hh"
#include "math/transform.hh"

#include "model/model.hh"

#include "opengl/program.hh"

// #include "rubik.hh"

namespace xpeditor {






struct RenderContextModel {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
};


struct RendererModel {
    opengl::Programs renderer_program = opengl::Programs::ModelColor;
    

    unsigned int transform_location;
    unsigned int view_location;
    unsigned int perspective_location;

    void init();
    void create_render_context(RenderContextModel& context, ModelColor& model);
    void set_camera_uniforms(m4f4 view_mat, m4f4 pers_mat);
    void render(const RenderContextModel& context, m4f4 matrix, int vertex_count);
};



}