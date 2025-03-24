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



void CameraOrbital::set_triplet(float _rho, float _theta, float _phi){

    rho = _rho;
    theta = _theta;

    phi = _phi < 0.0f ? 0.0f : _phi;
    phi = phi > 3.14f ? 3.14f : phi;

    transform.pos.x = rho * cosf(theta) * sinf(phi);
    transform.pos.y = rho * sinf(theta) * sinf(phi);
    transform.pos.z = rho * cosf(phi);

    transform.rot.x = phi;
    transform.rot.z = 1.57f + theta; // Offset by 90 deg to align camera with initial theta
}

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


    perspective_mat = indentity;
    perspective_mat.perspective(
                        camera.fov, 
                        camera.width, 
                        camera.height, 
                        camera.zn,
                        camera.zf
    );

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
    // renderer_model.create_render_context(_cube.c_xp.model);
    // renderer_model.create_render_context(_cube.c0.model);
    // renderer_model.create_render_context(_cube.c1.model);
    

    return true;
}

void scene_handle_input(InputState input_state){
    

    // ORBIT CONTROLS

    // Spherical Coords

    float rho_factor = -0.2f;
    float theta_scale = -0.008f;
    float phi_scale = 0.010f;

    float _rho      = camera.rho   + camera.rho  * rho_factor * input_state.scroll_delta;
    float _theta    = camera.theta + theta_scale * (float) input_state.mouse.middle_delta_accum.x;
    float _phi      = camera.phi   + phi_scale   * (float) input_state.mouse.middle_delta_accum.y;

    camera.set_triplet(_rho, _theta, _phi);


    
    if(input_state.up){

    }
    if(input_state.down){

    }
    if(input_state.left){

    }
    if(input_state.right){

    }
    
    
    // PRINTS
    if(input_state.p){
        // std::cout << "camera.transform.rot.x = " << camera.transform.rot.x << std::endl;
        // std::cout << "camera.transform.rot.y = " << camera.transform.rot.y << std::endl;
        // std::cout << "camera.transform.rot.z = " << camera.transform.rot.z << std::endl;
        // std::cout << "xy_cam_norm = " << xy_cam_norm << std::endl;
        // std::cout << "camera.rho    = " << camera.rho << std::endl;
        // std::cout << "camera.theta  = " << camera.theta << std::endl;
        // std::cout << "camera.phi    = " << camera.phi << std::endl;
    }

}

void scene_update(Cube& _cube){

    // if(_cube.animator.is_animating)
        // _cube.update_animator();
    // else
        // cube
    // _cube.update_cubies();
    
    // _cube.c_xp.model.set_transform_matrix();
    // _cube.c_xp.tr

    // _cube.c0.model.set_transform_matrix();
    // _cube.c1.model.set_transform_matrix();

    // camera
    camera.set_triplet(camera.rho, camera.theta, camera.phi);
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
    // renderer_model.render(_cube.c_xp.model);
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
