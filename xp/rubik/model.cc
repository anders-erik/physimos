
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

    // unsigned int program = gpu_get_program(Shader::Model);



    // view_location = glGetUniformLocation(program, "view");
    // perspective_location = glGetUniformLocation(program, "perspective");

    glUniformMatrix4fv(view_location, 1, GL_TRUE, (float *) &view_mat);
    glUniformMatrix4fv(perspective_location, 1, GL_TRUE, (float *) &pers_mat);


}

void RendererModel::render(Model& model){
    
    gpu_use_program(renderer_program);

    glUniformMatrix4fv(transform_location, 1, GL_TRUE, (float*) &model.transform.matrix);

    glBindVertexArray(model.rendering_context.vao);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    

}



}
