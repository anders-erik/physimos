
#include "math/vecmat.hh"
#include "opengl/texture.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

#include "rendererUI.hh"


namespace opengl {


const unsigned int SCREEN_INIT_WIDTH = 1200;
const unsigned int SCREEN_INIT_HEIGHT = 800;


float colorSquareVertices[30] = {
    0.0f, 0.0f, 0.0f,   // bottom-left
    1.0f, 1.0f, 0.0f,   // top-right
    0.0f, 1.0f, 0.0f,   // top-left
    1.0f, 1.0f, 0.0f,   // top-right
    0.0f, 0.0f, 0.0f,   // bottom-left
    1.0f, 0.0f, 0.0f,   // bottom-right
};


// BOOTSTRAP FOR INITIAL TESTING - 2025-05-29
bool renderer_created = false;
RendererUI* renderer_ui;
RendererUI& get_renderer_ui(){
    if(!renderer_created){
        renderer_ui = new RendererUI();
            renderer_created = true;
    }
    return *renderer_ui;
}


RendererUI::RendererUI(){
    init_color();
    // Init others
    program_string_id = opengl::build_program_vert_frag(program_string);
    program_texture_id = opengl::build_program_vert_frag(program_texture);
}


void RendererUI::activate(UIType type){
    
    switch (type)
    {
    case UIType::Color:
        glUseProgram(color_program.id);
        break;

    case UIType::String:
        glUseProgram(program_string_id);
        break;

    case UIType::Texture:
        glUseProgram(program_texture_id);
        break;
    
    default:
        break;
    }
    
}



void RendererUI::color_set(float* primitive_transform_4x4, float _darkness_shift, UI::Color color4) const {

    glUseProgram(color_program.id);
    // GL_TRUE : Transpose before loading into uniform!
    glUniformMatrix4fv(color_program.uiViewportTransformLoc, 1, GL_TRUE, color_program.viewport_transform.pointer_const());
    glUniformMatrix4fv(color_program.uiPrimitiveTransformLoc, 1, GL_TRUE, primitive_transform_4x4);
    glUniform1f(color_program.darknessShiftLoc, _darkness_shift);
    glUniform4f(color_program.color4Loc, color4.R, color4.G, color4.B, color4.A);

}

void RendererUI::color_draw() const {
    glUseProgram(color_program.id);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBindVertexArray(color_program.vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisable(GL_BLEND);
}



void RendererUI::init_color(){

    color_program.id = opengl::build_program_vert_frag(color_program.name);

    color_program.viewport_transform.x.x = 2.0f / (float)SCREEN_INIT_WIDTH;
    color_program.viewport_transform.y.y = 2.0f / (float)SCREEN_INIT_HEIGHT;
    color_program.viewport_transform.x.w = -1.0f;
    color_program.viewport_transform.y.w = -1.0f;

    glUseProgram(color_program.id);

    // GET UNIFORM LOCATIONS
    color_program.uiViewportTransformLoc = glGetUniformLocation(color_program.id, "viewportTransform");
    color_program.uiPrimitiveTransformLoc = glGetUniformLocation(color_program.id, "primitiveTransform");
    color_program.darknessShiftLoc = glGetUniformLocation(color_program.id, "darknessShift");
    color_program.color4Loc = glGetUniformLocation(color_program.id, "color4");


    // VAO, VBO
    glGenVertexArrays(1, &color_program.vao);
    glGenBuffers(1, &color_program.vbo);

    glBindBuffer(GL_ARRAY_BUFFER, color_program.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorSquareVertices), colorSquareVertices, GL_STATIC_DRAW);

    glBindVertexArray(color_program.vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}


}
