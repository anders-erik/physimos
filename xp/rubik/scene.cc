#include "glad/glad.h"
#include "scene.hh"

#include "model.hh"
#include "math.hh"

// #include "rubik.hh"

#include <iostream>
#include <cmath>

namespace xprubik {

RendererModel renderer_model;
RendererAxes renderer_axes;

CameraOrbital camera;

// Cube cube;


void CameraOrbital::set_matrices(){
    m4f4 indentity;

    view_mat = indentity;

    view_mat.rotate_x(-camera.transform.rot.x);
    view_mat.rotate_y(-camera.transform.rot.y);
    view_mat.rotate_z(camera.transform.rot.z);

    f3 negative_camera_pos;
    negative_camera_pos.x = -camera.transform.pos.x;
    negative_camera_pos.y = -camera.transform.pos.y;
    negative_camera_pos.z = -camera.transform.pos.z;
    view_mat.translate(negative_camera_pos);

    m4f4 sanity_mat;
    sanity_mat.x.x = 0.0f;
    sanity_mat.x.z = -1.0f;
    sanity_mat.y.y = 0.0f;
    sanity_mat.y.x = -1.0f;
    sanity_mat.z.z = 0.0f;
    sanity_mat.z.y = 1.0f;
    // mat_mul(view_mat, sanity_mat);

    
    perspective_mat = indentity;
    perspective_mat.perspective(
                        camera.fov, 
                        camera.width, 
                        camera.height, 
                        camera.zn,
                        camera.zf
    );

}


void CameraOrbital::orbit_z(float orbit_z_delta){

    // Z ORBIT -- GOOD
    m4f4 rot_z_mat = m4f4_create_rotation_z(orbit_z_delta);
    transform.pos.matmul(rot_z_mat);
    transform.rot.z -= orbit_z_delta;

}
void CameraOrbital::orbit_xy(float xy_delta){

    float xy_cam_norm_2 = transform.pos.x*transform.pos.x + transform.pos.y*transform.pos.y;
    float xy_cam_norm = sqrtf(xy_cam_norm_2);
    float x_cam_norm = transform.pos.x / xy_cam_norm;
    float y_cam_norm = transform.pos.y / xy_cam_norm;

    // Lock the orbit when going very close to vertical
    if( xy_cam_norm > 0.1f ){
        m4f4 rot_x_mat = m4f4_create_rotation_x( y_cam_norm * -xy_delta);
        m4f4 rot_y_mat = m4f4_create_rotation_y( x_cam_norm * xy_delta);
        transform.pos.matmul(rot_y_mat);
        transform.pos.matmul(rot_x_mat);
        // LOOK AT
        transform.rot.x += xy_delta;
    }

}
void CameraOrbital::zoom(float zoom_delta){

    transform.pos.x -= transform.pos.x * 0.2 * zoom_delta;
    transform.pos.y -= transform.pos.y * 0.2 * zoom_delta;
    transform.pos.z -= transform.pos.z * 0.2 * zoom_delta;

}


void scene_set_viewport_dims(int _width, int _height){
    // std::cout << _width << _height << std::endl;
    camera.width = (float) _width;
    camera.height = (float) _height;
}

bool scene_init(Cube& _cube){

    renderer_model.init();
    renderer_axes.init();

    // camera.transform.rot.z = 0.5;
    camera.transform.rot.x =  1.3;
    camera.transform.rot.y =  0.0;
    camera.transform.rot.z = -0.75;

    camera.transform.pos.x = -4.0;
    camera.transform.pos.y = -4.0;
    camera.transform.pos.z = 2.0f;

    for(Cubie& c : _cube.cubies){
        // c.model.transform.rot.x += animator.animation_angle_step;
        renderer_model.create_render_context(c.model);
    }
    renderer_model.create_render_context(_cube.c_xp.model);
    // renderer_model.create_render_context(_cube.c0.model);
    // renderer_model.create_render_context(_cube.c1.model);
    

    return true;
}

void scene_handle_input(InputState input_state){
    


    // ORBIT CONTROLS


    // camera.transform.rot.x -= pan_rot_scale * (float) input_state.mouse.middle_delta_accum.y;
    // camera.transform.rot.z += pan_rot_scale * (float) input_state.mouse.middle_delta_accum.x;
    // float vert_mult = 1.41;
    // float vert_mult = 2.0f;

    // I've been playing around with values/+/- configurations here
    // float angle = atan2f(camera.transform.pos.y, camera.transform.pos.x);

 
    float orbit_xy_scale = 0.004f;
    float orbit_z_scale = 0.008f;

    camera.orbit_xy(orbit_xy_scale * (float) input_state.mouse.middle_delta_accum.y);
    camera.orbit_z( orbit_z_scale * (float) input_state.mouse.middle_delta_accum.x);
    camera.zoom(input_state.scroll_delta);
    

    // PRINTS
    if(input_state.p){
        // std::cout << "camera.transform.rot.x = " << camera.transform.rot.x << std::endl;
        // std::cout << "camera.transform.rot.y = " << camera.transform.rot.y << std::endl;
        // std::cout << "camera.transform.rot.z = " << camera.transform.rot.z << std::endl;
        // std::cout << "xy_cam_norm = " << xy_cam_norm << std::endl;
        
    }

    // ARROW LOOK AROUND
    // if(input_state.up){
    //     camera.transform.rot.x += 0.02f;
    // }
    // if(input_state.down){
    //     camera.transform.rot.x -= 0.02f;
    // }
    // if(input_state.left)
    //     camera.transform.rot.z += 0.02;
    // if(input_state.right)
    //     camera.transform.rot.z -= 0.02;
    

}

void scene_update(Cube& _cube){

    _cube.update_animator();

    // cube
    _cube.update_cubies();
    
    // _cube.c_xp.model.set_transform_matrix();
    // _cube.c_xp.tr

    // _cube.c0.model.set_transform_matrix();
    // _cube.c1.model.set_transform_matrix();

    // camera
    camera.set_matrices();
    renderer_model.set_camera_uniforms(camera.view_mat, camera.perspective_mat);

    m4f4 indentity;
    renderer_axes.set_uniforms(indentity, camera.view_mat, camera.perspective_mat);
    // renderer_axes.set_uniforms(_cube.c0.model.transform.matrix, camera.view_mat, camera.perspective_mat);


}

void scene_render(Cube& _cube){


    m4f4 identity;
    renderer_axes.set_uniforms(identity, camera.view_mat, camera.perspective_mat);
    renderer_axes.render();
    renderer_axes.set_uniforms(_cube.cubies[0].model.transform.matrix, camera.view_mat, camera.perspective_mat);
    renderer_axes.render();

    for(Cubie& c : _cube.cubies){
        renderer_model.render(c.model);
    }
    renderer_model.render(_cube.c_xp.model);
}




void RendererAxes::init(){

    unsigned int program = gpu_get_program(Shader::Axes);
    glUseProgram(program);

    transform_location = glGetUniformLocation(program, "transform");
    view_location = glGetUniformLocation(program, "view");
    perspective_location = glGetUniformLocation(program, "perspective");


    AxesVertex vertices[6];

    vertices[0].pos.x = 0.0f;
    vertices[0].color.x = 1.0f;

    vertices[1].pos.x = 1.0f;
    vertices[1].color.x = 1.0f;

    vertices[2].pos.y = 0.0f;
    vertices[2].color.y = 1.0f;

    vertices[3].pos.y = 1.0f;
    vertices[3].color.y = 1.0f;

    vertices[4].pos.z = 0.0f;
    vertices[4].color.z = 1.0f;

    vertices[5].pos.z = 1.0f;
    vertices[5].color.z = 1.0f;

    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(AxesVertex) * 6, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AxesVertex), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AxesVertex), (void*) sizeof(f3));
    glEnableVertexAttribArray(1);

}
void RendererAxes::set_uniforms(m4f4 transform_mat, m4f4 view_mat, m4f4 pers_mat){

    gpu_use_program(Shader::Axes);

    glUniformMatrix4fv(transform_location, 1, GL_TRUE, (float*) &transform_mat);
    glUniformMatrix4fv(view_location, 1, GL_TRUE, (float*) &view_mat);
    glUniformMatrix4fv(perspective_location, 1, GL_TRUE, (float*) &pers_mat);

}
void RendererAxes::render(){

    gpu_use_program(Shader::Axes);

    glBindVertexArray(vao);

    glDrawArrays(GL_LINES, 0, 6);
}

}
