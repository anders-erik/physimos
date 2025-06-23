
#include <iostream>
#include <cmath>

#include "glad/glad.h"

#include "math/vecmat.hh"
#include "math/quarternion.hh"

#include "opengl/texture.hh"

#include "scene2D/quadS2D.hh"
#include "scene2D/scene2D.hh"
#include "scene/manager_3D.hh"

#include "renderer3D.hh"

#include "scene/scene_state.hh"
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

    program_quad.init();

    fb_object_ids.reload((int)window_fb_size.x, (int) window_fb_size.y);
}



void RendererScene3D::
set_window_fb_size(window::WindowResizeEvent& window_resize_event)
{

    this->window_fb_size = window_resize_event.size_f;
    fb_object_ids.reload(window_resize_event.size_i.x, window_resize_event.size_i.y);
}




void RendererScene3D::
render_scene_3d(Scene3D& scene3D, SceneState& state)
{

    

    glEnable(GL_DEPTH_TEST);
    // Defaults to fill. Context flag can overwrite?
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    scene3D.camera.update();
    // scene3D.camera.print();


    // UNIFORMS
    program_model_texture.set_camera_view_projection(   scene3D.camera.perspective.matrix,
                                                        scene3D.camera.view.matrix);

    program_vector.set_project_view_matrix( scene3D.camera.perspective.matrix, 
                                            scene3D.camera.view.matrix);

    program_axes.set_uniforms(  m4f4(), 
                                scene3D.camera.view.matrix, 
                                scene3D.camera.perspective.matrix);
    
    program_model_color.set_camera_uniforms(    scene3D.camera.perspective.matrix, 
                                                scene3D.camera.view.matrix);

    program_mesh.set_camera_view_projection(    scene3D.camera.perspective.matrix, 
                                                scene3D.camera.view.matrix);
    program_quad.set_camera_view_projection(    scene3D.camera.perspective.matrix, 
                                                scene3D.camera.view.matrix);


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

    for(TagO tago : scene3D.tagos)
    {
        if(tago.type == TagO::Base)
        {
            Object* base = ObjectManager::get_object(tago);
            if(base == nullptr) continue;

            if(base->tag.oid == state.capturing_quad_tag.oid)
                program_mesh.render(base->mesh, 0x0000ffff);
            else if(base->tag.oid == state.selected_tag.oid)
                program_mesh.render(base->mesh, 0x00ff00ff);
            else if(base->tag.oid == state.hovered_tag.oid)
                program_mesh.render(base->mesh, 0xff0000ff);
            else
                program_mesh.render(base->mesh, 0xffffffff);
        }
        else if (tago.type == TagO::Quad)
        {
            SQuadO* quado = ObjectManager::get_squado(tago);
            if(quado == nullptr) continue;

            m4f4 translation_matrix = m4f4::translation(quado->object.pos);

            if(quado->object.tag.oid == state.capturing_quad_tag.oid)
                program_mesh.render(translation_matrix, quado->object.mesh, 0x0000ffff);
            else if(quado->object.tag.oid == state.selected_tag.oid)
                program_mesh.render(translation_matrix, quado->object.mesh, 0x00ff00ff);
            else if(quado->object.tag.oid == state.hovered_tag.oid)
                program_mesh.render(translation_matrix, quado->object.mesh, 0xff0000ff);
            else
                program_mesh.render(translation_matrix, quado->object.mesh, 0xffffffff);

            program_quad.render(translation_matrix, quado->object.mesh, quado->squad.texture_id);
            // program_quad.render(m4f4(), 0);
            // program_quad.render(quado->mesh, quado->texture.text_coords);
            // program_quad.render(quado->mesh);
            
        }
    }


    // AXES
    program_axes.render();


    // VECTOR
    program_vector.set_color({1.0f, 1.0f, 1.0f});
    program_vector.render(  {0.0f, 0.0f, 0.0f}, 
                            {0.0f, 2.0f, 2.0f});

    // QUARTERNION TESTS
    f3 rot_axis = {0.5f, 0.0f, 1.0f};
    program_vector.set_color({1.0f, 0.0f, 1.0f});
    Quarternion::rotate_f3( scene3D.quarternion_vector_100,
                            rot_axis,
                            0.01);
    program_vector.render(  {0.0f, 0.0f, 0.0f},
                            scene3D.quarternion_vector_100);
    program_vector.render(  {0.0f, 0.0f, 0.0f},
                            scene3D.quarternion_vector_111);

    program_vector.set_color({0.0f, 0.0f, 0.0f});
    program_vector.render(  {0.0f, 0.0f, 0.0f},
                            rot_axis);



}

void RendererScene3D::render_object_ids(Scene3D & scene)
{
    program_object_ids.set_camera_view_projection(  scene.camera.perspective.matrix, 
                                                    scene.camera.view.matrix);

    fb_object_ids.bind();

    fb_object_ids.clear_with({0.0f, 0.0f, 0.0f, 0.0f});
    
    for(TagO tago : scene.tagos)
    {
        if(tago.type == TagO::Base)
        {
            Object* baseo = ObjectManager::get_object(tago);
            if(baseo == nullptr) continue;

            program_object_ids.render(m4f4(), baseo->mesh, baseo->tag.oid);
        }
        else if (tago.type == TagO::Quad)
        {
            SQuadO* quado = ObjectManager::get_squado(tago);
            if(quado == nullptr) continue;

            program_object_ids.render(  m4f4::translation(quado->object.pos), 
                                        quado->object.mesh, 
                                        quado->object.tag.oid                   );
        }
    }

    fb_object_ids.unbind(window_fb_size);
}



Object* RendererScene3D::sample_object_in_fb(f2 cursor_pos_sane)
{
    fb_object_ids.bind();

    f4 vec4_color = fb_object_ids.sample_texture(cursor_pos_sane.to_i2());

    fb_object_ids.unbind(window_fb_size);

    OID oid = program_object_ids.vec4_to_oid(vec4_color);

    return ObjectManager::get_object(oid);
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


