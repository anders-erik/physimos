
#include "math/vecmat.hh"
#include "opengl/texture.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

#include "program_texture.hh"



namespace UI {


const unsigned int SCREEN_INIT_WIDTH = 1200;
const unsigned int SCREEN_INIT_HEIGHT = 800;


float baseTextureSquareVertices[30] = {
    // Position         // texture coord
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // bottom-left
    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top-right
    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top-left
    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top-right
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // bottom-left
    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // bottom-right
};


void ProgramBaseTexture::init(){

    glUseProgram(id);

    // GET UNIFORM LOCATIONS
    uiViewportTransformLoc = glGetUniformLocation(id, "viewportTransform");
    uiPrimitiveTransformLoc = glGetUniformLocation(id, "primitiveTransform");


    // VAO, VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(baseTextureSquareVertices), baseTextureSquareVertices, GL_STATIC_DRAW);

    glBindVertexArray(vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}
void ProgramBaseTexture::set_viewport_transform(m4f4 _viewport_transform){

    glUseProgram(id);

    // GL_TRUE : Transpose before loading into uniform!
    glUniformMatrix4fv(uiViewportTransformLoc, 1, GL_TRUE, _viewport_transform.pointer_const());

}


void ProgramBaseTexture::set(float* primitiveTransform_mat, unsigned int texture) const {
    
    glUseProgram(id);
    
    glUniformMatrix4fv(uiPrimitiveTransformLoc, 1, GL_TRUE, primitiveTransform_mat);

    glBindTexture(GL_TEXTURE_2D, texture);

}


void ProgramBaseTexture::draw() const {

    glUseProgram(id);

    glBindVertexArray(vao);
    

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisable(GL_BLEND);

}


}
