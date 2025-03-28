#include "opengl/program.hh"

#include "math/vecmat.hh"
#include "renderer_model_texture.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

namespace opengl {


void RendererModelTexture::init(){
    // program = opengl::
    // opengl::build_program_vert_frag(Programs::Model)
    program = opengl::build_program_vert_frag(Programs::ModelTexture);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // glEnableVertexAttribArray(0); // Position
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // glEnableVertexAttribArray(1); // Normal
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // glEnableVertexAttribArray(2); // TexCoords
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
}


void RendererModelTexture::set_camera_view_projection(m4f4 persective_mat, m4f4 view_mat){
    // glUseProgram(program);
    opengl::gpu_use_program(Programs::ModelTexture);

    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, (float*) &persective_mat);
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (float*) &view_mat);
}



void RendererModelTexture::create_model_rendering_context(xpeditor::MeshTexture& mesh, RendererModelTextureContext& context, Texture texture_enum){

    glUseProgram(program);
    context.program = program;


    context.texture = texture_get_id(texture_enum);


    glGenVertexArrays(1, &context.VAO);
    glGenBuffers(1, &context.VBO);
    glGenBuffers(1, &context.EBO);

    glBindVertexArray(context.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, context.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(xpeditor::VertexTexture), mesh.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.faces.size() * sizeof(xpeditor::TriangleFaceIndeces), mesh.faces.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(xpeditor::VertexTexture), (void*)0);

    glEnableVertexAttribArray(1); // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(xpeditor::VertexTexture), (void*)offsetof(xpeditor::VertexTexture, normal));
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);

    glEnableVertexAttribArray(2); // TexCoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(xpeditor::VertexTexture), (void*)offsetof(xpeditor::VertexTexture, tex));

}
void RendererModelTexture::render_model_rendering_context(xpeditor::MeshTexture& mesh, RendererModelTextureContext& context, m4f4 model_matrix){

    glUseProgram(program);

    glBindVertexArray(context.VAO);

    glBindTexture(GL_TEXTURE_2D, context.texture);

    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (float*) &model_matrix);

    glDrawElements(GL_TRIANGLES, mesh.faces.size() * 3, GL_UNSIGNED_INT, 0);

}



void RendererModelTexture::render(xpeditor::MeshTexture& model, m4f4 model_matrix){

    glUseProgram(program);

    // glBindTexture(GL_TEXTURE_2D, get_texture_id(Texture::Colors));


    // std::cout << "Vertex size: " << sizeof(Vertex) << std::endl;
    // std::cout << "Position offset: " << offsetof(Vertex, position) << std::endl;
    // std::cout << "Normal offset: " << offsetof(Vertex, normal) << std::endl;
    // std::cout << "TexCoords offset: " << offsetof(Vertex, texCoords) << std::endl;

    // std::cout << "Vertices count: " << model.vertices.size() << std::endl;
    // std::cout << "Indices count: " << model.indices.size() << std::endl;



    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(xpeditor::VertexTexture), model.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.faces.size() * sizeof(xpeditor::TriangleFaceIndeces), model.faces.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(xpeditor::VertexTexture), (void*)0);

    glEnableVertexAttribArray(1); // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(xpeditor::VertexTexture), (void*)offsetof(xpeditor::VertexTexture, normal));
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);

    glEnableVertexAttribArray(2); // TexCoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(xpeditor::VertexTexture), (void*)offsetof(xpeditor::VertexTexture, tex));
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)24);

    // glBindVertexArray(0);

    glBindVertexArray(VAO);
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (float *) &model_matrix);

    glDrawElements(GL_TRIANGLES, model.faces.size() * 3, GL_UNSIGNED_INT, 0);

}




}
