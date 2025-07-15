
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

#include "scene/mesh_line.hh"





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
    shader_color_light_inst.init();

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
set_camera(CameraObject& camera)
{
    camera.update_matrices();
    m4f4 cam_persp_mat    = camera.perspective.matrix;
    m4f4 cam_view_mat   = camera.view.matrix;

    // UNIFORMS
    program_model_texture.set_camera_view_projection(   cam_persp_mat,
                                                        cam_view_mat);

    program_vector.set_project_view_matrix( cam_persp_mat, 
                                            cam_view_mat);

    program_axes.set_uniforms(  m4f4(), 
                                cam_view_mat, 
                                cam_persp_mat);
    
    program_model_color.set_camera_uniforms(    cam_persp_mat, 
                                                cam_view_mat);

    program_mesh.set_camera_view_projection(    cam_persp_mat, 
                                                cam_view_mat);
    program_quad.set_camera_view_projection(    cam_persp_mat, 
                                                cam_view_mat);
    program_color_light.set_camera_view_perspective( cam_view_mat, 
                                                    cam_persp_mat          );
    shader_color_light_inst.set_camera_view_perspective(    cam_view_mat, 
                                                            cam_persp_mat   );
}

void RendererScene3D::set_lamps(std::vector<TagO> lamp_tags, Manager3D & manager3D)
{
    // LAMP
    for(auto lamp_tag : lamp_tags)
    {
        Object* lampo = manager3D.manager_o.get_object(lamp_tag);
            if(lampo == nullptr) continue;
        program_color_light.set_light_pos(lampo->pos);
        shader_color_light_inst.set_light_pos(lampo->pos);

        Lamp* lamp = manager3D.manager_p.find_lamp(lampo->tagp);
            if(lampo == nullptr) continue;
        program_color_light.set_light_color(lamp->light_color);
        shader_color_light_inst.set_light_color(lamp->light_color);
    }
}




void RendererScene3D::
render_scene(Scene3D& scene3D, Manager3D& manager_3D)
{
    glEnable(GL_DEPTH_TEST);
    // Defaults to fill. Context flag can overwrite?
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    set_camera(scene3D.camera);
    set_lamps(scene3D.lamp_tags, manager_3D);

    

    for(TagO tag : scene3D.lamp_tags)
        render_tag(tag, manager_3D);

    for(TagO tag : scene3D.quad_tags)
        render_tag(tag, manager_3D);
    
    for(TagO tag : scene3D.tagos)
        render_tag(tag, manager_3D);

    


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
    q_mesh.cube_centered();
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

    CameraObject& camera = manager_3D.window_scene->camera;
    camera.update_matrices();
    program_object_ids.set_camera_view_projection(  camera.perspective.matrix, 
                                                    camera.view.matrix);

    fb_object_ids.bind();

    fb_object_ids.clear_with({0.0f, 0.0f, 0.0f, 0.0f});


    for(TagO tago : scene.tagos)
    {
        Object* baseo = manager_3D.manager_o.get_object(tago);
        if(baseo == nullptr) continue;

        program_object_ids.render(  baseo->get_model_matrix(), 
                                    baseo->mesh, 
                                    baseo->tag.oid  );
    }
    for(TagO quad_tag : scene.quad_tags)
    {
        Object* obj = manager_3D.manager_o.get_object(quad_tag.oid);
        if(obj == nullptr) continue;

        program_object_ids.render(  obj->get_model_matrix(),
                                    obj->mesh,
                                    obj->tag.oid                );
    }
    for(TagO lamp_tag : scene.lamp_tags)
    {
        Object* obj = manager_3D.manager_o.get_object(lamp_tag.oid);
        if(obj == nullptr) continue;

        program_object_ids.render(  obj->get_model_matrix(),
                                    obj->mesh,
                                    obj->tag.oid                );
    }

    fb_object_ids.unbind(window_fb_size);
}



TagO RendererScene3D::
sample_oid_tag(const Scene3D& scene, const f2 cursor_pos_sane)
{
    fb_object_ids.bind();

    f4 vec4_color = fb_object_ids.sample_texture(cursor_pos_sane.to_i2());

    fb_object_ids.unbind(window_fb_size);


    OID sampled_oid = program_object_ids.vec4_to_oid(vec4_color);

    
    for(TagO tag : scene.tagos)
        if(tag.oid == sampled_oid)
            return tag;

    for(TagO tag : scene.quad_tags)
        if(tag.oid == sampled_oid)
            return tag;

    for(TagO tag : scene.lamp_tags)
        if(tag.oid == sampled_oid)
            return tag;

    return TagO();
}

bool RendererScene3D::
render_tag(TagO tag, Manager3D & manager_3D)
{
    Object* object = manager_3D.manager_o.get_object(tag);
        if(object == nullptr) return false;

    m4f4 model_matrix = object->get_model_matrix();

    // MESH - change color of active objects
    if(tag == manager_3D.state.selected.tag)
        program_mesh.render(object->get_model_matrix(), object->mesh, 0x00ff00ff);
    else if(tag == manager_3D.state.hovered.tag)
        program_mesh.render(object->get_model_matrix(), object->mesh, 0xff0000ff);


    if     (tag.is_quad())
    {
        Quad* quad = manager_3D.manager_q.find_quad_oid(tag.oid);
            if(quad == nullptr) return false;
        render_quad(*object, *quad, manager_3D);
        return true;
    }
    else if(tag.is_lamp())
    {
        Lamp* lamp = manager_3D.manager_p.find_lamp(object->tagp);
            if(lamp == nullptr) return false;
        render_lamp(*object, *lamp);
        return true;
    }



    

    if(object->rend_cxt.instance_ctx.enabled)
    {
        shader_color_light_inst.set_data(object->mesh);
        shader_color_light_inst.render_fill(object->rend_cxt.instance_ctx.instance_count);
    }
    else
    {
        switch (object->rend_cxt.shading)
        {

        case ObjectRenderContext::ColorLight:
            program_color_light.set_model_matrix(model_matrix);
            program_color_light.set_mesh(object->mesh);
            program_color_light.render_fill();
            break;

        case ObjectRenderContext::Wireframe:
            // program_color_light.set_model_matrix(model_matrix);
            // program_color_light.set_mesh(object->mesh);
            // program_color_light.render_lines();
            program_mesh.render(model_matrix, object->mesh);
            break;

        default:
            break;
        }
    }


    if(object->rend_cxt.normals)
    {
        for(size_t i=0; i<object->mesh.verts.size(); i++)
        {
            program_vector.set_color({0.0f, 0.0f, 0.0f});
            program_vector.render(  object->mesh.verts[i] + object->pos, 
                                    object->mesh.normals[i]           );
        }
    }

    if(object->pyh_tag.is_physics() && object->rend_cxt.physics)
    {
        Physics* physics = manager_3D.manager_p.find_physics(object->pyh_tag);
        if(physics != nullptr)
        {
            MeshLine linemesh;
            m4f4 mat;

            if(physics->is_aabb())
            {
                linemesh.aabb(physics->isector.aabb);
            }
            else if (physics->is_sphere())
            {
                linemesh.bounding_sphere(physics->isector.sphere.r);
                mat =   m4f4::translation(physics->p)
                        * m4f4::scale(physics->isector.sphere.r);
            }

            if(physics->isect_flag)
                program_mesh.render_linemesh(mat, linemesh, 0xab0fdbff);
            else
                program_mesh.render_linemesh(mat, linemesh, 0x000000ff);

        }
    }
    
    return true;
}

void RendererScene3D::
render_quad(Object& object, Quad& quad, Manager3D& manager_3D)
{
    m4f4 model_matrix = object.get_model_matrix();

    program_quad.render(model_matrix, object.mesh, quad.texture_id);
    program_axes.render(model_matrix);

    if(manager_3D.manager_q.state.is_capturing())
    {
        program_mesh.render(model_matrix, object.mesh, 0x0000ffff);
    }   
}


void RendererScene3D::
render_lamp(Object& object, Lamp& lamp)
{
    m4f4 model_matrix = object.get_model_matrix();

    program_mesh.set_color(lamp.light_color);
    program_mesh.render_filled(model_matrix, object.mesh);
}

void RendererScene3D::render_physics(Physics * physics, ManagerProperty & manager_p)
{

}





