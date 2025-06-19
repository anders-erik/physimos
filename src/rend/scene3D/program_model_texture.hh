
#pragma once

#include "math/vecmat.hh"
#include "model/model.hh"
#include "opengl/texture.hh"



struct ProgramModelTextureContext {
    unsigned int program;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int texture;
};



struct ProgramModelTexture : protected opengl::Program
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;


    ProgramModelTexture() : Program("3D/model_texture") {};


    void create_model_rendering_context(    model::MeshT& mesh, 
                                            ProgramModelTextureContext& context, 
                                            opengl::Textures texture_enum);

    void render_model_rendering_context(    model::MeshT& mesh, 
                                            ProgramModelTextureContext& context, 
                                            m4f4 model_matrix);

    void set_camera_view_projection(m4f4 persective_mat, m4f4 view_mat);

    void init();
    void render(model::MeshT& mesh, m4f4 model_matrix);

};



