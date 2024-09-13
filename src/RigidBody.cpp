
#include "RigidBody.hpp"

#include <vector>

#include <glad/glad.h>

#include "VectorMath.hpp"





void RigidBody::setShaderProgram(Shader* shader) {
    this->shader = shader;
}


void RigidBody::setVaoVbo_wireframe() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    // printf("triangel : %lu\n", sizeof(triangle));
    // printf("data()   : %lu\n", sizeof(worldCube1.vertices.data()); // I guess this returns the size of the pointer to the underlaying data/array on the heap?
    // printf("data()   : %lu\n", sizeof(worldCube1.vertices[0])*worldCube1.vertices.size());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}




