
#include "math.hh"
#include "model.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

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

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(f3)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(f3)*2));
    glEnableVertexAttribArray(2);

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


void Model::set_transform_matrix(){
    m4f4 identity;

    transform.matrix = identity;

    transform.matrix.translate(transform.pos);
    
    // Intrinsic rotations work??
    // transform.matrix.rotate_x(transform.rot.x);
    // transform.matrix.rotate_y(transform.rot.y);
    // transform.matrix.rotate_z(transform.rot.z);

    // mat_mul(transform.matrix, );

}
void Model::set_transform_matrix_anim(){
    m4f4 identity;

    

    // transform.matrix = identity;

    // transform.matrix.rotate_z(transform.rot.z);
    // transform.matrix.rotate_y(transform.rot.y);
    // transform.matrix.rotate_x(transform.rot.x);

    // // transform.matrix.translate(transform.pos);

    // transform.matrix = transform.matrix;

}

void Model::set_base_color(f3 color){

    for(Vertex& vert : mesh.vertices){
        vert.color.x = color.x;
        vert.color.y = color.y;
        vert.color.z = color.z;
    }
}

void model_add_facelet(Mesh& mesh, f3 color, Axis axis){

    float cube_size = 1.0f;
    float cube_half = cube_size / 2.0f;

    float facelet_offset = cube_half + 0.001;
    float facelet_width = cube_half - 0.05;

    Vertex v0;
    Vertex v1;
    Vertex v2;
    Vertex v3;

    if(axis == Axis::x || axis == Axis::nx){

        v0.pos = f3( 0.0f, -facelet_width, -facelet_width);
        v1.pos = f3( 0.0f,  facelet_width, -facelet_width);
        v2.pos = f3( 0.0f,  facelet_width,  facelet_width);
        v3.pos = f3( 0.0f, -facelet_width,  facelet_width);

        if(axis == Axis::x){
            v0.pos.x += facelet_offset;
            v1.pos.x += facelet_offset;
            v2.pos.x += facelet_offset;
            v3.pos.x += facelet_offset;
            v0.normal.x = 1.0f;
            v1.normal.x = 1.0f;
            v2.normal.x = 1.0f;
            v3.normal.x = 1.0f;
        }
        else{
            v0.pos.x -= facelet_offset;
            v1.pos.x -= facelet_offset;
            v2.pos.x -= facelet_offset;
            v3.pos.x -= facelet_offset;
            v0.normal.x = -1.0f;
            v1.normal.x = -1.0f;
            v2.normal.x = -1.0f;
            v3.normal.x = -1.0f;
        }
    }
    else if(axis == Axis::y || axis == Axis::ny){

        v0.pos = f3( -facelet_width, 0.0f, -facelet_width);
        v1.pos = f3(  facelet_width, 0.0f, -facelet_width);
        v2.pos = f3(  facelet_width, 0.0f,  facelet_width);
        v3.pos = f3( -facelet_width, 0.0f,  facelet_width);

        if(axis == Axis::y){
            v0.pos.y += facelet_offset;
            v1.pos.y += facelet_offset;
            v2.pos.y += facelet_offset;
            v3.pos.y += facelet_offset;
            v0.normal.y = 1.0f;
            v1.normal.y = 1.0f;
            v2.normal.y = 1.0f;
            v3.normal.y = 1.0f;
        }
        else{
            v0.pos.y -= facelet_offset;
            v1.pos.y -= facelet_offset;
            v2.pos.y -= facelet_offset;
            v3.pos.y -= facelet_offset;
            v0.normal.y = -1.0f;
            v1.normal.y = -1.0f;
            v2.normal.y = -1.0f;
            v3.normal.y = -1.0f;
        }
    }
    else if(axis == Axis::z || axis == Axis::nz){

        v0.pos = f3( -facelet_width, -facelet_width, 0.0f);
        v1.pos = f3(  facelet_width, -facelet_width, 0.0f);
        v2.pos = f3(  facelet_width,  facelet_width, 0.0f);
        v3.pos = f3( -facelet_width,  facelet_width, 0.0f);

        if(axis == Axis::z){
            v0.pos.z += facelet_offset;
            v1.pos.z += facelet_offset;
            v2.pos.z += facelet_offset;
            v3.pos.z += facelet_offset;
            v0.normal.z = 1.0f;
            v1.normal.z = 1.0f;
            v2.normal.z = 1.0f;
            v3.normal.z = 1.0f;
        }
        else{
            v0.pos.z -= facelet_offset;
            v1.pos.z -= facelet_offset;
            v2.pos.z -= facelet_offset;
            v3.pos.z -= facelet_offset;
            v0.normal.z = -1.0f;
            v1.normal.z = -1.0f;
            v2.normal.z = -1.0f;
            v3.normal.z = -1.0f;
        }
    }



    v0.color = color;
    v1.color = color;
    v2.color = color;
    v3.color = color;

    mesh.vertices.push_back(v0);
    mesh.vertices.push_back(v1);
    mesh.vertices.push_back(v2);
    mesh.vertices.push_back(v3);
    
    int v0i = mesh.vertices.size()-4;
    int v1i = mesh.vertices.size()-3;
    int v2i = mesh.vertices.size()-2;
    int v3i = mesh.vertices.size()-1;

    mesh.faces.emplace_back(TriangleFaceIndeces{v0i, v1i, v2i});
    mesh.faces.emplace_back(TriangleFaceIndeces{v2i, v3i, v0i});

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
    // v00.color.x = 0.0f;
    // v00.color.y = 0.0f;
    // v00.color.z = 0.0f;
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

    // Color & Normals
    for(Vertex& vert : mesh.vertices){
        vert.color.x = 0.3f;
        vert.color.y = 0.3f;
        vert.color.z = 0.3f;

        // extend vert position from center
        float len_2 = vert.pos.x * vert.pos.x + vert.pos.y * vert.pos.y + vert.pos.z * vert.pos.z;
        float len = sqrtf(len_2);
        vert.normal.x = vert.pos.x / len;
        vert.normal.y = vert.pos.y / len;
        vert.normal.z = vert.pos.z / len;
    }

    // Front
    mesh.faces.emplace_back(TriangleFaceIndeces{1, 5, 2});
    mesh.faces.emplace_back(TriangleFaceIndeces{2, 5, 6});

    // Right
    mesh.faces.emplace_back(TriangleFaceIndeces{2, 6, 3});
    mesh.faces.emplace_back(TriangleFaceIndeces{3, 6, 7});

    // Back
    mesh.faces.emplace_back(TriangleFaceIndeces{3, 7, 0});
    mesh.faces.emplace_back(TriangleFaceIndeces{0, 7, 4});

    // Left
    mesh.faces.emplace_back(TriangleFaceIndeces{0, 4, 1});
    mesh.faces.emplace_back(TriangleFaceIndeces{1, 4, 5});

    // Up
    mesh.faces.emplace_back(TriangleFaceIndeces{0, 1, 2});
    mesh.faces.emplace_back(TriangleFaceIndeces{2, 3, 0});

    // Down
    mesh.faces.emplace_back(TriangleFaceIndeces{4, 5, 6});
    mesh.faces.emplace_back(TriangleFaceIndeces{6, 7, 4});

    

}



}
