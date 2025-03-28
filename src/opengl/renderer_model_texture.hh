
#pragma once

#include "math/vecmat.hh"
#include "model/model.hh"
#include "opengl/texture.hh"

namespace opengl {


struct RendererModelTextureContext {
    unsigned int program;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int texture;
};



typedef struct RendererModelTexture {
    unsigned int program;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    void create_model_rendering_context(xpeditor::MeshTexture& mesh, RendererModelTextureContext& context, Texture texture_enum);
    void render_model_rendering_context(xpeditor::MeshTexture& mesh, RendererModelTextureContext& context, m4f4 model_matrix);

    void set_camera_view_projection(m4f4 persective_mat, m4f4 view_mat);

    void init();
    void render(xpeditor::MeshTexture& mesh, m4f4 model_matrix);
} RendererModelTexture;



}