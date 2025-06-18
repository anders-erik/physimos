
#include <iostream>
#include <cmath>

#include "glad/glad.h"

#include "math/vecmat.hh"

#include "opengl/texture.hh"

#include "scene/quadS2D.hh"
#include "scene/scene2D.hh"
#include "scene/manager.hh"

#include "renderer3D.hh"

#include "scene/object_manager.hh"







RendererScene3D::
RendererScene3D(f2 window_fb_size)
{
    this->window_fb_size = window_fb_size;

    program_vector.init();
    program_axes.init();
    program_model_texture.init();
    program_model_color.init();

    program_mesh.init();
    program_object_ids.init();

    fb_object_ids.reload((int)window_fb_size.x, (int) window_fb_size.y);
}



void RendererScene3D::
set_window_fb_size(window::WindowResizeEvent& window_resize_event)
{

    this->window_fb_size = window_resize_event.size_f;
    fb_object_ids.reload(window_resize_event.size_i.x, window_resize_event.size_i.y);
}


OID RendererScene3D::sample_object_id_fb(f2 cursor_pos_sane)
{
    fb_object_ids.bind();

    f4 vec4_color = fb_object_ids.sample_texture(cursor_pos_sane.to_i2());

    fb_object_ids.unbind(window_fb_size);

    return program_object_ids.vec4_to_oid(vec4_color);
}



void RendererScene3D::
render_scene_3d(SceneBase& scene_base)
{
    if(!scene_base.is_3D())
        throw std::runtime_error("Can only render 3d scene");
    Scene3D& scene3D = (Scene3D&) scene_base;

    fb_object_ids.bind();
    // fb_object_ids.clear_with({1.0f, 1.0f, 1.0f, 1.0f});
    fb_object_ids.clear_with({0.0f, 0.0f, 0.0f, 0.0f});
    fb_object_ids.unbind(window_fb_size);

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

    program_mesh.set_camera_view_projection(    scene3D.camera.perspective_mat, 
                                                scene3D.camera.view_mat);

    program_object_ids.set_camera_view_projection(  scene3D.camera.perspective_mat, 
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

    for(Object object : scene3D.objects)
    {
        if(object.type == Object::Mesh)
        {
            MeshO* mesho = ObjectManager::get_mesho(object.id);
            if(mesho == nullptr) continue;


            if(mesho->object.id == scene3D.selected_object.id)
                program_mesh.render(mesho->mesh, 0x0000ffff);
            else if(mesho->object.id == scene3D.hovered_object.id)
                program_mesh.render(mesho->mesh, 0x00ff00ff);
            else
                program_mesh.render(mesho->mesh, 0xffffffff);


            // OID OUTLINE
            fb_object_ids.bind();
            program_object_ids.render(mesho->mesh, mesho->object.id);
            fb_object_ids.unbind(window_fb_size);
        }
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


