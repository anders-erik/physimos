
#include <iostream>
#include <cmath>

#include "glad/glad.h"

#include "math/vecmat.hh"
#include "math/quarternion.hh"

#include "opengl/texture.hh"

#include "scene2D/quadS2D.hh"
#include "scene2D/scene2D.hh"
#include "scene/manager_3D.hh"
#include "scene/manager_object.hh"

#include "renderer3D.hh"

#include "scene/scene_state.hh"





void RendererScene3D::
init(f2 window_fb_size)
{
    this->window_fb_size = window_fb_size;

    program_vector.init();
    program_axes.init();
    program_model_texture.init();
    program_model_color.init();

    program_mesh.init();
    program_object_ids.init();

    program_color_light.init();

    program_quad.init();

    fb_object_ids.reload((int)window_fb_size.x, (int) window_fb_size.y);
}


void RendererScene3D::
bind_window_fb(i2 window_scene_i)
{
    glBindFramebuffer(  GL_FRAMEBUFFER, 
                        0               );

    glViewport( 0,
                0,
                window_scene_i.x,
                window_scene_i.y    );

    if(window_scene_i != fb_object_ids.size)
    {
        fb_object_ids.reload(   window_scene_i.x, 
                                window_scene_i.y    );
    }
}


void RendererScene3D::
set_window_fb_size(window::WindowResizeEvent& window_resize_event)
{
    this->window_fb_size = window_resize_event.size_f;
    fb_object_ids.reload(window_resize_event.size_i.x, window_resize_event.size_i.y);
}




void RendererScene3D::
render_scene_3d(Scene3D& scene3D, Manager3D& manager_3D)
{
    ManagerObject& man_o = manager_3D.manager_o;
    const SceneState& state = manager_3D.state;
    

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
    program_color_light.set_camera_view_perspective( scene3D.camera.view.matrix, 
                                                    scene3D.camera.perspective.matrix          );

    // LAMP
    for(auto tago : scene3D.tagos)
    {
        if(tago.is_lamp())
        {
            Object* lampo = man_o.get_object(tago);
            if(lampo == nullptr) continue;
            program_color_light.set_light_pos(lampo->pos);

            Lamp* lamp = manager_3D.manager_p.find_lamp(lampo->tagp);
            if(lampo == nullptr) continue;
            program_color_light.set_light_color(lamp->light_color);
        }
    }


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
    for(model::VertexT& vertex : scene3D.tube.mesh.vertices)
    {
        program_vector.render(vertex.normal, vertex.pos);
    }


    for(TagO tag : scene3D.tagos)
    {
        Object* base = man_o.get_object(tag);
        if(base == nullptr) continue;

        m4f4 scale_matrix       = m4f4::scale(base->scale);
        m4f4 translation_matrix = m4f4::translation(base->pos);
        m4f4 rotation_matrix    = base->rot.matrix();
        m4f4 model_matrix       = translation_matrix * rotation_matrix * scale_matrix;


        // MESH - change color of active objects
        if(tag == state.selected.tag)
            program_mesh.render(model_matrix, base->mesh, 0x00ff00ff);
        else if(tag == state.hovered.tag)
            program_mesh.render(model_matrix, base->mesh, 0xff0000ff);


        // QUAD
        if (tag.type == TagO::Quad)
        {
            Quad* quad = manager_3D.manager_q.find_quad_oid(tag.oid);
            if(quad == nullptr) continue;

            program_quad.render(model_matrix, base->mesh, quad->texture_id);
            program_axes.render(model_matrix);

            if(manager_3D.manager_q.state.is_capturing())
            {
                program_mesh.render(model_matrix, base->mesh, 0x0000ffff);
            }
        }
        else if(tag.is_lamp())
        {
            Lamp* lamp = manager_3D.manager_p.find_lamp(base->tagp);
            if(lamp == nullptr) continue;

            program_mesh.set_color(lamp->light_color);
            program_mesh.render_filled(model_matrix, base->mesh);
        }
        else if(base->mesh.normals.size() > 0) // Draw meshes with normals as color models + normal vectors
        {
            program_color_light.render(model_matrix, base->mesh);
            // Draw normals if selected
            if(tag == state.selected.tag)
            {
                for(size_t i=0; i<base->mesh.verts.size(); i++)
                {
                    program_vector.set_color({0.0f, 0.0f, 0.0f});
                    program_vector.render(  base->mesh.verts[i] + base->pos, 
                                            base->mesh.normals[i]           );
                }
            }
        }
        else
        {
            program_mesh.render(model_matrix, base->mesh);
        }

    }
    


    // AXES
    program_axes.render(m4f4());


    // VECTOR
    program_vector.set_color({1.0f, 1.0f, 1.0f});
    program_vector.render(  {0.0f, 0.0f, 0.0f}, 
                            {0.0f, 2.0f, 2.0f});

    // QUARTERNION TESTS
    f3 rot_axis = {0.5f, 0.0f, 1.0f};
    Quarternion::rotate_f3( scene3D.q_vec_100,
                            rot_axis,
                            0.01f);
    program_vector.set_color({1.0f, 0.0f, 0.0f});
    program_vector.render(  {0.0f, 0.0f, 0.0f},
                            scene3D.q_vec_100);
    Quarternion::rotate_f3( scene3D.q_vec_010,
                            rot_axis,
                            0.01f);
    program_vector.set_color({0.0f, 1.0f, 0.0f});
    program_vector.render(  {0.0f, 0.0f, 0.0f},
                            scene3D.q_vec_010);
    Quarternion::rotate_f3( scene3D.q_vec_001,
                            rot_axis,
                            0.01f);
    program_vector.set_color({0.0f, 0.0f, 1.0f});
    program_vector.render(  {0.0f, 0.0f, 0.0f},
                            scene3D.q_vec_001);

    Mesh q_mesh;
    q_mesh.cube();
    q_mesh.scale({0.2, 0.2, 0.02});
    scene3D.q_1000.rotate(f3::Z(), 0.01f);
    // scene3D.q_1000 = Quarternion::rotate_quart(scene3D.q_1000, {0.0f, f3::Y()}, 0.1);
    // scene3D.q_1000.print();
    program_mesh.render(     scene3D.q_1000.matrix()
                            ,q_mesh 
                            ,0xff00ffff                  );

    // program_vector.set_color({1.0f, 0.0f, 1.0f});
    // program_vector.render(  {0.0f, 0.0f, 0.0f},
    //                         scene3D.q_1000_vec);

    program_vector.set_color({0.0f, 0.0f, 0.0f});
    program_vector.render(  {0.0f, 0.0f, 0.0f},
                            rot_axis);
}



void RendererScene3D::
render_object_ids(Scene3D & scene, Manager3D& manager_3D)
{
    program_object_ids.use();

    program_object_ids.set_camera_view_projection(  scene.camera.perspective.matrix, 
                                                    scene.camera.view.matrix);

    fb_object_ids.bind();

    fb_object_ids.clear_with({0.0f, 0.0f, 0.0f, 0.0f});
    
    for(TagO tago : scene.tagos)
    {
        Object* baseo = manager_3D.manager_o.get_object(tago);
        if(baseo == nullptr) continue;

        m4f4 scale_matrix       = m4f4::scale(baseo->scale);
        m4f4 translation_matrix = m4f4::translation(baseo->pos);
        m4f4 rotation_matrix    = baseo->rot.matrix();
        m4f4 model_matrix       = translation_matrix * rotation_matrix * scale_matrix;

        program_object_ids.render(  model_matrix, 
                                    baseo->mesh, 
                                    baseo->tag.oid  );
    }

    fb_object_ids.unbind(window_fb_size);
}



TagO RendererScene3D::
sample_oid_tag(const std::vector<TagO>& scene_tags, const f2 cursor_pos_sane)
{
    fb_object_ids.bind();

    f4 vec4_color = fb_object_ids.sample_texture(cursor_pos_sane.to_i2());

    fb_object_ids.unbind(window_fb_size);


    OID sampled_oid = program_object_ids.vec4_to_oid(vec4_color);

    
    for(TagO tag : scene_tags)
    {
        if(tag.oid == sampled_oid)
            return tag;
    }

    return TagO();
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


