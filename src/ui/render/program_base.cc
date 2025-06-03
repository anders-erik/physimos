
#include "math/vecmat.hh"
#include "opengl/texture.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

#include "program_base.hh"


namespace UI {


float baseSquareVertices[18] = {
    0.0f, 0.0f, 0.0f,   // bottom-left
    1.0f, 1.0f, 0.0f,   // top-right
    0.0f, 1.0f, 0.0f,   // top-left
    1.0f, 1.0f, 0.0f,   // top-right
    0.0f, 0.0f, 0.0f,   // bottom-left
    1.0f, 0.0f, 0.0f,   // bottom-right
};







void ProgramBase::init(){

    // id = program_id;

    glUseProgram(id);


    // GET UNIFORM LOCATIONS
    uiViewportTransformLoc = glGetUniformLocation(id, "viewportTransform");
    uiPrimitiveTransformLoc = glGetUniformLocation(id, "primitiveTransform");
    darknessShiftLoc = glGetUniformLocation(id, "darknessShift");
    color4Loc = glGetUniformLocation(id, "color4");
    

    // VAO, VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(baseSquareVertices), baseSquareVertices, GL_STATIC_DRAW);

    glBindVertexArray(vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}
void ProgramBase::set_viewport_transform(m4f4 _viewport_transform){

    glUseProgram(id);

    // GL_TRUE : Transpose before loading into uniform!
    glUniformMatrix4fv(uiViewportTransformLoc, 1, GL_TRUE, _viewport_transform.pointer_const());
}

void ProgramBase::draw() const {
    glUseProgram(id);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisable(GL_BLEND);
}

void ProgramBase::set(float* primitive_transform_4x4, float _darkness_shift, UI::Color color4) const {
    
    glUseProgram(id);

    glUniformMatrix4fv(uiPrimitiveTransformLoc, 1, GL_TRUE, primitive_transform_4x4);
    glUniform1f(darknessShiftLoc, _darkness_shift);
    glUniform4f(color4Loc, color4.R, color4.G, color4.B, color4.A);
    
}








}
