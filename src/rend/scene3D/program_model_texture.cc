#include "opengl/program.hh"

#include "math/vecmat.hh"
#include "program_model_texture.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>




void ProgramModelTexture::init(){
    // program = opengl::
    // opengl::build_program_vert_frag(Programs::Model)
    // program = opengl::build_program_vert_frag(ProgramName::ModelTexture);

    glUseProgram(id);

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


void ProgramModelTexture::set_camera_view_projection(m4f4 persective_mat, m4f4 view_mat)
{
    glUseProgram(id);
    // opengl::gpu_use_program(opengl::ProgramName::ModelTexture);


    glUniformMatrix4fv(glGetUniformLocation(id, "projection"), 1, GL_TRUE, (float*) &persective_mat);
    glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_TRUE, (float*) &view_mat);
}



void ProgramModelTexture::
create_model_rendering_context( model::MeshT& mesh, 
                                ProgramModelTextureContext& context, 
                                opengl::Textures texture_enum)
{

    glUseProgram(id);
    context.program = id;


    context.texture = opengl::texture_get_id(texture_enum);


    glGenVertexArrays(1, &context.VAO);
    glGenBuffers(1, &context.VBO);
    glGenBuffers(1, &context.EBO);

    glBindVertexArray(context.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, context.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(model::VertexT), mesh.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.faces.size() * sizeof(model::TriangleFaceIndeces), mesh.faces.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(model::VertexT), (void*)0);

    glEnableVertexAttribArray(1); // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(model::VertexT), (void*)offsetof(model::VertexT, normal));
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);

    glEnableVertexAttribArray(2); // TexCoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(model::VertexT), (void*)offsetof(model::VertexT, tex));

}


void ProgramModelTexture::
render_model_rendering_context( model::MeshT& mesh, 
                                ProgramModelTextureContext& context, 
                                m4f4 model_matrix)
{

    glUseProgram(id);

    glBindVertexArray(context.VAO);

    glBindTexture(GL_TEXTURE_2D, context.texture);

    glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, (float*) &model_matrix);

    // Set draw mode
    // glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
    // glPointSize(5);
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    // glLineWidth(2);
    // glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    glDrawElements(GL_TRIANGLES, mesh.faces.size() * 3, GL_UNSIGNED_INT, 0);

}



void ProgramModelTexture::
render( model::MeshT& model, 
        m4f4 model_matrix)
{

    glUseProgram(id);

    // glBindTexture(GL_TEXTURE_2D, get_texture_id(Texture::Colors));


    // std::cout << "Vertex size: " << sizeof(Vertex) << std::endl;
    // std::cout << "Position offset: " << offsetof(Vertex, position) << std::endl;
    // std::cout << "Normal offset: " << offsetof(Vertex, normal) << std::endl;
    // std::cout << "TexCoords offset: " << offsetof(Vertex, texCoords) << std::endl;

    // std::cout << "Vertices count: " << model.vertices.size() << std::endl;
    // std::cout << "Indices count: " << model.indices.size() << std::endl;



    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(model::VertexT), model.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.faces.size() * sizeof(model::TriangleFaceIndeces), model.faces.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(model::VertexT), (void*)0);

    glEnableVertexAttribArray(1); // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(model::VertexT), (void*)offsetof(model::VertexT, normal));
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);

    glEnableVertexAttribArray(2); // TexCoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(model::VertexT), (void*)offsetof(model::VertexT, tex));
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)24);

    // glBindVertexArray(0);

    glBindVertexArray(VAO);
    glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, (float *) &model_matrix);

    glDrawElements(GL_TRIANGLES, model.faces.size() * 3, GL_UNSIGNED_INT, 0);

}


