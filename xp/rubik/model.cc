
#include "math.hh"
#include "model.hh"

#include "glad/glad.h"
#include <iostream>

namespace xprubik {



void RendererModel::init(){
    // gpu_build_vert_frag_program(shader_enum);

    unsigned int program = gpu_get_program(Shader::Model);

    transform_location = glGetUniformLocation(program, "transform");
    view_location = glGetUniformLocation(program, "view");
    perspective_location = glGetUniformLocation(program, "perspective");


}
void RendererModel::create_render_context(Model& model){
    
    gpu_use_program(renderer_program);

    // NEW OBJECTS
    glGenVertexArrays(1, &model.rendering_context.vao);
    glGenBuffers(1, &model.rendering_context.vbo);
    glGenBuffers(1, &model.rendering_context.ebo);

    glBindVertexArray(model.rendering_context.vao);

    glBindBuffer(GL_ARRAY_BUFFER, model.rendering_context.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * model.mesh.vertices.size(), model.mesh.vertices.data(), GL_STATIC_DRAW);

    // std::cout <<  model.mesh.vertices[1].pos.x << std::endl;
    

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.rendering_context.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TriangleFaceIndeces)*model.mesh.faces.size(), model.mesh.faces.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0 );
    glEnableVertexAttribArray(0);

}

void RendererModel::set_camera_uniforms(m4f4 view_mat, m4f4 pers_mat){

    gpu_use_program(Shader::Model);

    // unsigned int program = gpu_get_program(Shader::Model);

    // view_location = glGetUniformLocation(program, "view");
    // perspective_location = glGetUniformLocation(program, "perspective");

    glUniformMatrix4fv(view_location, 1, GL_TRUE, (float *) &view_mat);
    glUniformMatrix4fv(perspective_location, 1, GL_TRUE, (float *) &pers_mat);


}

void RendererModel::render(Model& model){
    
    gpu_use_program(renderer_program);

    glBindVertexArray(model.rendering_context.vao);

    glUniformMatrix4fv(transform_location, 1, GL_TRUE, (float*) &model.transform.matrix);


    glDrawElements(GL_TRIANGLES, model.mesh.faces.size()*3, GL_UNSIGNED_INT, 0);
    

}


void model_add_cube_mesh(Mesh& mesh){

    float cube_size = 1.0f;
    float cube_half = cube_size / 2.0f;


    // for(int crossec_i = 0; crossec_i < 2; crossec_i++){
    //     for(int vert_i = 0; vert_i < 4; vert_i++){


        
    //     }
    // }


    Vertex v00;
    v00.pos.x = -cube_half;
    v00.pos.y = -cube_half;
    v00.pos.z = -cube_half;
    mesh.vertices.push_back(v00);

    Vertex v01;
    v01.pos.x =  cube_half;
    v01.pos.y = -cube_half;
    v01.pos.z = -cube_half;
    mesh.vertices.push_back(v01);

    Vertex v02;
    v02.pos.x =  cube_half;
    v02.pos.y =  cube_half;
    v02.pos.z = -cube_half;
    mesh.vertices.push_back(v02);

    Vertex v03;
    v03.pos.x = -cube_half;
    v03.pos.y =  cube_half;
    v03.pos.z = -cube_half;
    mesh.vertices.push_back(v03);




    Vertex v10;
    v10.pos.x = -cube_half;
    v10.pos.y = -cube_half;
    v10.pos.z =  cube_half;
    mesh.vertices.push_back(v10);

    Vertex v11;
    v11.pos.x =  cube_half;
    v11.pos.y = -cube_half;
    v11.pos.z =  cube_half;
    mesh.vertices.push_back(v11);

    Vertex v12;
    v12.pos.x =  cube_half;
    v12.pos.y =  cube_half;
    v12.pos.z =  cube_half;
    mesh.vertices.push_back(v12);

    Vertex v13;
    v13.pos.x = -cube_half;
    v13.pos.y =  cube_half;
    v13.pos.z =  cube_half;
    mesh.vertices.push_back(v13);


    // front
    mesh.faces.emplace_back(TriangleFaceIndeces{0, 1, 2});
    mesh.faces.emplace_back(TriangleFaceIndeces{2, 3, 0});

    // back
    mesh.faces.emplace_back(TriangleFaceIndeces{4, 5, 6});
    mesh.faces.emplace_back(TriangleFaceIndeces{6, 7, 4});

}



}
