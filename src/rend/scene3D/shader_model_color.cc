
#include "math/vecmat.hh"
#include "shader_model_color.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>




void ShaderModelColor::init()
{
    build();

    // gpu_build_vert_frag_program(shader_enum);

    // opengl::build_program_vert_frag(opengl::ProgramName::ModelColor);
    glUseProgram(id);

    // unsigned int program = gpu_get_program(opengl::ProgramName::ModelColor);

    transform_location = glGetUniformLocation(id, "transform");
    view_location = glGetUniformLocation(id, "view");
    perspective_location = glGetUniformLocation(id, "perspective");

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

}
void ShaderModelColor::create_render_context(RenderContextModelColor& context, model::ModelColor& model){
    
    // gpu_use_program(renderer_program);
    
    glUseProgram(id);

    // NEW OBJECTS
    

    context.vao = vao;
    context.vbo = vbo;
    context.ebo = ebo;

    glBindVertexArray(context.vao);

    glBindBuffer(GL_ARRAY_BUFFER, context.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model::VertexColor) * model.mesh.vertices.size(), model.mesh.vertices.data(), GL_STATIC_DRAW);

    // std::cout <<  model.mesh.vertices[1].pos.x << std::endl;
    

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model::TriangleFaceIndeces)*model.mesh.faces.size(), model.mesh.faces.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(model::VertexColor), (void*)0 );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(model::VertexColor), (void*)(sizeof(f3)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(model::VertexColor), (void*)(sizeof(f3)*2));
    glEnableVertexAttribArray(2);

}

void ShaderModelColor::set_camera_uniforms(m4f4 pers_mat, m4f4 view_mat){
    glUseProgram(id);

    // gpu_use_program(opengl::ProgramName::ModelColor);

    // unsigned int program = gpu_get_program(Shader::Model);

    // view_location = glGetUniformLocation(program, "view");
    // perspective_location = glGetUniformLocation(program, "perspective");

    glUniformMatrix4fv(view_location, 1, GL_TRUE, (float *) &view_mat);
    glUniformMatrix4fv(perspective_location, 1, GL_TRUE, (float *) &pers_mat);


}

void ShaderModelColor::render(const RenderContextModelColor& context, m4f4 matrix, int vertex_count){
    
    // gpu_use_program(renderer_program);
    glUseProgram(id);

    glBindVertexArray(context.vao);

    glUniformMatrix4fv(transform_location, 1, GL_TRUE, (float*) &matrix);


    glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);
    

}




