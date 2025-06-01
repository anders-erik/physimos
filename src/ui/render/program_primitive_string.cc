
#include "math/vecmat.hh"
#include "opengl/texture.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

#include "program_primitive_string.hh"


namespace UI {


const unsigned int SCREEN_INIT_WIDTH = 1200;
const unsigned int SCREEN_INIT_HEIGHT = 800;


float stringSquareVertices[30] = {
    // Position         // texture coord
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // bottom-left
    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top-right
    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top-left
    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top-right
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // bottom-left
    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // bottom-right
};






void ProgramPrimitiveString::init(){

    glUseProgram(id);

    // GET UNIFORM LOCATIONS
    uiViewportTransformLoc = glGetUniformLocation(id, "viewportTransform");
    uiPrimitiveTransformLoc = glGetUniformLocation(id, "primitiveTransform");


    // VAO, VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(stringSquareVertices), stringSquareVertices, GL_STATIC_DRAW);

    glBindVertexArray(vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}
void ProgramPrimitiveString::set_viewport_transform(m4f4 _viewport_transform){

    glUseProgram(id);

    // GL_TRUE : Transpose before loading into uniform!
    glUniformMatrix4fv(uiViewportTransformLoc, 1, GL_TRUE, _viewport_transform.pointer_const());

}


void ProgramPrimitiveString::set_primitive_transform(float * primitiveTransform_mat) const
{
    glUseProgram(id);

    // We want the with of a char!
    float orig_x = primitiveTransform_mat[0];
    float orig_y = primitiveTransform_mat[5];
    // offset for visibility
    primitiveTransform_mat[7] -= 0;

    primitiveTransform_mat[0] = 9;
    primitiveTransform_mat[5] = 18;


    glUniformMatrix4fv(uiPrimitiveTransformLoc, 1, GL_TRUE, primitiveTransform_mat);


    primitiveTransform_mat[0] = orig_x;
    primitiveTransform_mat[5] = orig_y;

    primitiveTransform_mat[7] += 0;

    // this->texture = texture;
}


void ProgramPrimitiveString::set_texture(unsigned int new_texture){
    glUseProgram(id);

    texture = new_texture;

    glBindTexture(GL_TEXTURE_2D, texture);

}



void ProgramPrimitiveString::set_vertex_data(VertexFontBitmap* vertex_data, unsigned int sizeof_vertex_data){

    vertex_count = sizeof_vertex_data / sizeof(VertexFontBitmap);

    glUseProgram(id);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof_vertex_data, vertex_data, GL_STATIC_DRAW);

    glBindVertexArray(vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void ProgramPrimitiveString::draw() const {
    glUseProgram(id);

    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, texture);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    glDisable(GL_BLEND);
}



}
