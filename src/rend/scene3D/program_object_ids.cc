#include "opengl/program.hh"

#include "math/vecmat.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>


#include "program_object_ids.hh"


void ProgramObjectIDs::
init()
{
    glUseProgram(id);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}


void ProgramObjectIDs::
set_camera_view_projection(m4f4 persective_mat, m4f4 view_mat)
{
    glUseProgram(id);
    // opengl::gpu_use_program(opengl::ProgramName::ModelTexture);


    glUniformMatrix4fv(glGetUniformLocation(id, "projection"), 1, GL_TRUE, (float*) &persective_mat);
    glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_TRUE, (float*) &view_mat);
}




void ProgramObjectIDs::
render(const m4f4& model_matrix, Mesh& mesh, OID oid)
{

    f4 vec4 = oid_to_vec4(oid);
    // OID new_OID = vec4_to_oid(vec4);
    // std::cout << oid << " " << new_OID << std::endl;
    

    glUseProgram(id);

    glUniform4fv(glGetUniformLocation(id, "oid_color"), 1, vec4.pointer());

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.verts.size() * sizeof(Vertex), mesh.verts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.faces.size() * sizeof(TriangleFaceIndeces), mesh.faces.data(), GL_STATIC_DRAW);


    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // glBindVertexArray(0);

    glBindVertexArray(VAO);
    glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_TRUE, model_matrix.pointer());


    // Obviously always fill
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    glDrawElements(GL_TRIANGLES, mesh.faces.size() * 3, GL_UNSIGNED_INT, 0);

}

f4 ProgramObjectIDs::oid_to_vec4(OID oid)
{
    float A = ((float) ((oid << 24) >> 24)) / 256.0f;
    float B = ((float) ((oid << 16) >> 24)) / 256.0f;
    float G = ((float) ((oid <<  8) >> 24)) / 256.0f;
    float R = ((float) ((oid <<  0) >> 24)) / 256.0f;

    return {R, G, B, A};
}

OID ProgramObjectIDs::vec4_to_oid(f4 vec4)
{
    OID R = ((OID) (vec4.x * 255.0f)) << 24;
    OID G = ((OID) (vec4.y * 255.0f)) << 16;
    OID B = ((OID) (vec4.z * 255.0f)) << 8;
    OID A = ((OID) (vec4.w * 255.0f)) << 0;

    return R + G + B + A;
}


