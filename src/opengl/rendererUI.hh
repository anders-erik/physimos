
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


class UIColorProgram : public Program {

    unsigned int vao;
    unsigned int vbo;

    unsigned int uiViewportTransformLoc;
    unsigned int uiPrimitiveTransformLoc;

    unsigned char color4Loc;
    unsigned char darknessShiftLoc;

public:

    UIColorProgram() : Program("ui/color") {};

    UIType type = UIType::Color;

    void init();
    void set_viewport_transform(m4f4 _viewport_transform);


    void draw() const;
    void set(float* primitive_transform_4x4, float _darkness_shift, UI::Color color4) const;
};



class RendererUI {

    // Shared matrix across ui shader programs
    m4f4 viewport_transform;

    UIColorProgram program_color;


    opengl::ProgramName program_string = opengl::ProgramName::ui_string;
    unsigned int program_string_id;

    opengl::ProgramName program_texture = opengl::ProgramName::ui_texture;
    unsigned int program_texture_id;


public:

    RendererUI();

    void activate(UIType type);
    void set_window_info(float width, float height, float _xscale, float _yscale);


    // COLOR
    void color_set(float* primitive_transform_4x4, float _darkness_shift, UI::Color color4) const;
    void color_draw() const;



};


}