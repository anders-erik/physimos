
#pragma once


#include "math/vecmat.hh"

#include "opengl/program.hh"


namespace UI {



class ShaderTexture : public opengl::ShaderProgram {

    unsigned int vao;
    unsigned int vbo_pos;
    unsigned int vbo_tex;

    unsigned int uiViewportTransformLoc;
    unsigned int uiPrimitiveTransformLoc;
    uint    color_texunit_LOC;


public:

    ShaderTexture() : ShaderProgram("ui/base_texture") {};

    void init();
    void set_viewport_transform(m4f4 _viewport_transform);

    void reset_text_coord();
    void set_text_coord(f2 text_coord);

    void set(float* primitiveTransform_mat) const;
    void draw() const;

};





}