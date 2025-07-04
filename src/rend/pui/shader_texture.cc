
#include "math/vecmat.hh"
#include "opengl/texture.hh"
#include "opengl/texture_unit.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

#include "shader_texture.hh"



namespace UI {


const unsigned int SCREEN_INIT_WIDTH = 1200;
const unsigned int SCREEN_INIT_HEIGHT = 800;


float baseTextureSquareVertices[18] = {
    // Position
    0.0f, 0.0f, 0.0f, // bottom-left
    1.0f, 1.0f, 0.0f, // top-right
    0.0f, 1.0f, 0.0f, // top-left
    1.0f, 1.0f, 0.0f, // top-right
    0.0f, 0.0f, 0.0f, // bottom-left
    1.0f, 0.0f, 0.0f, // bottom-right
};

float baseTextureCoord[12] = {
    // texture coord
    0.0f, 0.0f,   // bottom-left
    1.0f, 1.0f,   // top-right
    0.0f, 1.0f,   // top-left
    1.0f, 1.0f,   // top-right
    0.0f, 0.0f,   // bottom-left
    1.0f, 0.0f,   // bottom-right
};


void ShaderTexture::init()
{
    build();

    glUseProgram(id);

    // GET UNIFORM LOCATIONS
    uiViewportTransformLoc = glGetUniformLocation(id, "viewportTransform");
    uiPrimitiveTransformLoc = glGetUniformLocation(id, "primitiveTransform");
    color_texunit_LOC = glGetUniformLocation(id, "color_texunit");

    glUniform1i(color_texunit_LOC, opengl::TextureUnits::Color);

    // VAO, VBO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo_pos);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(baseTextureSquareVertices), baseTextureSquareVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vbo_tex);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(baseTextureCoord), baseTextureCoord, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void ShaderTexture::set_viewport_transform(m4f4 _viewport_transform){

    glUseProgram(id);

    // GL_TRUE : Transpose before loading into uniform!
    glUniformMatrix4fv(uiViewportTransformLoc, 1, GL_TRUE, _viewport_transform.pointer_const());

}

void ShaderTexture::reset_text_coord()
{
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(baseTextureCoord), baseTextureCoord, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ShaderTexture::set_text_coord(f2 text_coord)
{
    f2 coord_array[6] = {text_coord, text_coord, text_coord, text_coord ,text_coord, text_coord};

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coord_array), coord_array, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void ShaderTexture::set(float* primitiveTransform_mat) const {
    
    glUseProgram(id);
    
    glUniformMatrix4fv(uiPrimitiveTransformLoc, 1, GL_TRUE, primitiveTransform_mat);

}


void ShaderTexture::draw() const {

    glUseProgram(id);

    glBindVertexArray(vao);
    

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisable(GL_BLEND);

}


}
