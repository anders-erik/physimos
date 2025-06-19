
#include "math/vecmat.hh"
#include "opengl/texture.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

#include "program_string.hh"


namespace UI {


const unsigned int SCREEN_INIT_WIDTH = 1200;
const unsigned int SCREEN_INIT_HEIGHT = 800;


float baseStringSquareVertices[30] = {
    // Position         // texture coord
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // bottom-left
    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top-right
    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top-left
    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top-right
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // bottom-left
    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // bottom-right
};






void ProgramString::init()
{
    glUseProgram(id);

    // GET UNIFORM LOCATIONS
    uiViewportTransformLoc = glGetUniformLocation(id, "viewportTransform");
    uiPrimitiveTransformLoc = glGetUniformLocation(id, "primitiveTransform");


    // VAO, VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(baseStringSquareVertices), baseStringSquareVertices, GL_STATIC_DRAW);

    glBindVertexArray(vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void ProgramString::set_viewport_transform(m4f4 _viewport_transform)
{
    glUseProgram(id);

    // GL_TRUE : Transpose before loading into uniform!
    glUniformMatrix4fv(uiViewportTransformLoc, 1, GL_TRUE, _viewport_transform.pointer_const());
}




void ProgramString::set_base_transform(f2 base_location, f2 glyph_size) const
{
    glUseProgram(id);

    m4f4 matrix;

    matrix.x.x = glyph_size.x;
    matrix.y.y = glyph_size.y;

    matrix.x.w = base_location.x;
    matrix.y.w = base_location.y;

    glUniformMatrix4fv(uiPrimitiveTransformLoc, 1, GL_TRUE, matrix.pointer());
}




void ProgramString::set_texture(unsigned int new_texture)
{
    glUseProgram(id);

    texture = new_texture;

    glBindTexture(GL_TEXTURE_2D, texture);
}




void ProgramString::set_glyph_data(const std::vector<UI::Font::GlyphFontBitmap>& glyphs)
{
    glyph_count = glyphs.size();
    // 6 verts per glyph
    int data_size = glyph_count * verts_per_glyph * sizeof(UI::Font::VertexFontBitmap);
    

    glUseProgram(id);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data_size, glyphs.data(), GL_STATIC_DRAW);

    glBindVertexArray(vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void ProgramString::draw() const {
    glUseProgram(id);

    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, texture);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, glyph_count * verts_per_glyph);
    glDisable(GL_BLEND);
}



}
