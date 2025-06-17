
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

    scene3D.camera.set_matrices();

    // UNIFORMS
    program_model_texture.set_camera_view_projection(scene3D.camera.perspective_mat, scene3D.camera.view_mat);
    program_vector.set_project_view_matrix(scene3D.camera.perspective_mat, scene3D.camera.view_mat);
    program_axes.set_uniforms(m4f4(), scene3D.camera.view_mat, scene3D.camera.perspective_mat);
    program_model_color.set_camera_uniforms(scene3D.camera.perspective_mat, scene3D.camera.view_mat);


    // GROUND
    scene3D.ground.transform.set_matrix_model();
    program_model_texture.render_model_rendering_context(scene3D.ground.mesh, scene3D.ground_render_context, scene3D.ground.transform.matrix);

    // TUBE
    scene3D.tube.transform.set_matrix_model();
    program_model_texture.render_model_rendering_context(scene3D.tube.mesh, scene3D.tube_render_context, scene3D.tube.transform.matrix);

    for(model::VertexTexture vertex : scene3D.tube.mesh.vertices){
        program_vector.render(vertex.normal, vertex.pos);
    }


    // COLOR CUBE
    scene3D.model.transform.set_matrix_model();
    program_model_color.render(scene3D.model_render_context, scene3D.model.transform.matrix, scene3D.model.mesh.faces.size()*3);

    // VECTOR
    program_vector.render({0.0f, 2.0f, 2.0f}, {0.0f, 0.0f, 0.0f});

    // AXES
    program_axes.render();

}
