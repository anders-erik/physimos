
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

    viewport_transform.x.x = 2.0f / (float)SCREEN_INIT_WIDTH;
    viewport_transform.y.y = 2.0f / (float)SCREEN_INIT_HEIGHT;
    viewport_transform.x.w = -1.0f;
    viewport_transform.y.w = -1.0f;


    program_color.init();
    program_color.set_viewport_transform(viewport_transform);

    // Init others
    program_string_id = opengl::build_program_vert_frag(program_string);
    program_texture_id = opengl::build_program_vert_frag(program_texture);
}


void RendererUI::activate(UIType type){
    
    switch (type)
    {
    case UIType::Color:
        program_color.use();
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

void RendererUI::set_window_info(float width, float height, float _xscale, float _yscale){

    viewport_transform.x.x = 2.0f * _xscale / width;
    viewport_transform.y.y = 2.0f * _yscale / height;

    program_color.set_viewport_transform(viewport_transform);


}


void RendererUI::color_set(float* primitive_transform_4x4, float _darkness_shift, UI::Color color4) const {

    program_color.set(primitive_transform_4x4, _darkness_shift, color4);
    

}

void RendererUI::color_draw() const {
    program_color.draw();
}




void UIColorProgram::init(){

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorSquareVertices), colorSquareVertices, GL_STATIC_DRAW);

    glBindVertexArray(vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}
void UIColorProgram::set_viewport_transform(m4f4 _viewport_transform){

    glUseProgram(id);

    // GL_TRUE : Transpose before loading into uniform!
    glUniformMatrix4fv(uiViewportTransformLoc, 1, GL_TRUE, _viewport_transform.pointer_const());
}

void UIColorProgram::draw() const {
    glUseProgram(id);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisable(GL_BLEND);
}

void UIColorProgram::set(float* primitive_transform_4x4, float _darkness_shift, UI::Color color4) const {
    
    glUseProgram(id);

    glUniformMatrix4fv(uiPrimitiveTransformLoc, 1, GL_TRUE, primitive_transform_4x4);
    glUniform1f(darknessShiftLoc, _darkness_shift);
    glUniform4f(color4Loc, color4.R, color4.G, color4.B, color4.A);
    
}

}
