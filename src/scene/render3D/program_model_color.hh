
#pragma once

#include <vector>
#include "math/vecmat.hh"
#include "math/transform.hh"

#include "model/model.hh"

#include "opengl/program.hh"

// #include "rubik.hh"








struct RenderContextModelColor {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
};


struct ProgramModelColor : protected opengl::Program
{
    opengl::ProgramName renderer_program = opengl::ProgramName::ModelColor;
    

    unsigned int transform_location;
    unsigned int view_location;
    unsigned int perspective_location;

    ProgramModelColor() : Program("3D/model_color") {};

    void init();
    void create_render_context(RenderContextModelColor& context, model::ModelColor& model);
    void set_camera_uniforms(m4f4 pers_mat, m4f4 view_mat);
    void render(const RenderContextModelColor& context, m4f4 matrix, int vertex_count);
};


