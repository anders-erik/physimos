
#include <iostream>
#include <cmath>

#include "glad/glad.h"

#include "math/vecmat.hh"

#include "opengl/texture.hh"

#include "scene/quadS2D.hh"
#include "scene/scene2D.hh"
#include "scene/manager.hh"

#include "renderer3D.hh"








RendererScene3D::RendererScene3D()
{
    program_vector.init();
    program_axes.init();
    program_model_texture.init();
    program_model_color.init();
}

void RendererScene3D::render_scene_3d(SceneBase& scene_base)
{
    if(!scene_base.is_3d())
        throw std::runtime_error("Can only render 3d scene");
    Scene3D& scene3D = (Scene3D&) scene_base;

    glEnable(GL_DEPTH_TEST);
    // Defaults to fill. Context flag can overwrite?
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    scene3D.camera.set_matrices();

    // UNIFORMS
    program_model_texture.set_camera_view_projection(   scene3D.camera.perspective_mat,
                                                        scene3D.camera.view_mat);

    program_vector.set_project_view_matrix( scene3D.camera.perspective_mat, 
                                            scene3D.camera.view_mat);

    program_axes.set_uniforms(  m4f4(), 
                                scene3D.camera.view_mat, 
                                scene3D.camera.perspective_mat);
    
    program_model_color.set_camera_uniforms(    scene3D.camera.perspective_mat, 
                                                scene3D.camera.view_mat);

    // TEXTURE MODELS
    for(auto& model : scene3D.texture_models)
    {
        render_texure_model(model);
    }

    // COLOR MODELS
    for(auto& model : scene3D.color_models)
    {
        render_color_model(model);
    }

    // Tube normals
    for(model::VertexT vertex : scene3D.tube.mesh.vertices)
    {
        program_vector.render(vertex.normal, vertex.pos);
    }



    // VECTOR
    program_vector.render(  {0.0f, 0.0f, 0.0f}, 
                            {0.0f, 2.0f, 2.0f});

    // AXES
    program_axes.render();

}



void RendererScene3D::
render_texure_model(model::ModelT model_texture)
{
    ProgramModelTextureContext new_context;

    program_model_texture.create_model_rendering_context(   model_texture.mesh,   
                                                            new_context,
                                                            model_texture.texture);

    model_texture.transform.set_matrix_model();

    program_model_texture.render_model_rendering_context(   model_texture.mesh,
                                                            new_context,
                                                            model_texture.transform.matrix);
}

void RendererScene3D::
render_color_model(model::ModelColor model)
{
    model.transform.set_matrix_model();

    RenderContextModelColor new_context;

    program_model_color.create_render_context(new_context, model);

    program_model_color.render( new_context, 
                                model.transform.matrix, 
                                model.mesh.faces.size()*3);
}


