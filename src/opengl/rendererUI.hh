
#pragma once


#include "math/vecmat.hh"

#include "opengl/program.hh"

#include "ui/ui_globals.hh" // UI::Color


namespace opengl {


// BOOTSTRAP FOR INITIAL TRANSITION TO RENDERER - 2025-05-29
struct RendererUI;
RendererUI& get_renderer_ui();


enum UIType {
    Color,
    String,
    Texture
};


struct UIColorProgram {

    UIType type = UIType::Color;
    opengl::Programs name = opengl::Programs::ui_color;
    unsigned int id;

    unsigned int vao;
    unsigned int vbo;

    unsigned int uiViewportTransformLoc;
    unsigned int uiPrimitiveTransformLoc;

    unsigned char color4Loc;
    unsigned char darknessShiftLoc;

    m4f4 viewport_transform;
};



class RendererUI {


    UIColorProgram color_program;


    opengl::Programs program_string = opengl::Programs::ui_string;
    unsigned int program_string_id;

    opengl::Programs program_texture = opengl::Programs::ui_texture;
    unsigned int program_texture_id;


public:

    RendererUI();

    void activate(UIType type);

    void color_set(float* primitive_transform_4x4, float _darkness_shift, UI::Color color4) const;
    void color_draw() const;


private:

    void init_color();

};


}