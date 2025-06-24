
#pragma once


#include "math/vecmat.hh"

#include "opengl/program.hh"

#include "ui/ui_globals.hh" // UI::Color


namespace UI {



class ProgramPrimitiveColor : public opengl::ShaderProgram {

    unsigned int vao;
    unsigned int vbo;

    unsigned int uiViewportTransformLoc;
    unsigned int uiPrimitiveTransformLoc;

    unsigned char color4Loc;
    unsigned char darknessShiftLoc;

public:

    ProgramPrimitiveColor() : ShaderProgram("ui/color") {};

    void init();
    void set_viewport_transform(m4f4 _viewport_transform);


    void draw() const;
    void set(float* primitive_transform_4x4, float _darkness_shift, UI::Color color4) const;
};




}