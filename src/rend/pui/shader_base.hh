
#pragma once


#include "math/vecmat.hh"

#include "opengl/program.hh"



namespace UI 
{


class ShaderBase : public opengl::ShaderProgram {

    unsigned int vao;
    unsigned int vbo;

    unsigned int uiViewportTransformLoc;
    unsigned int uiPrimitiveTransformLoc;

    unsigned char color4Loc;
    unsigned char darknessShiftLoc;

public:

    ShaderBase() : ShaderProgram("ui/base") {};

    void init();
    void set_viewport_transform(m4f4 _viewport_transform);


    void draw() const;
    void set(float* primitive_transform_4x4, float _darkness_shift, f4 color4) const;
};




}