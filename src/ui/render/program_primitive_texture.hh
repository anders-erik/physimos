
#pragma once


#include "math/vecmat.hh"

#include "opengl/program.hh"


namespace UI {



class ProgramPrimitiveTexture : public opengl::Program {

    unsigned int vao;
    unsigned int vbo;

    unsigned int uiViewportTransformLoc;
    unsigned int uiPrimitiveTransformLoc;

    // unsigned int texture;

public:

    ProgramPrimitiveTexture() : Program("ui/texture") {};


    void init();
    void set_viewport_transform(m4f4 _viewport_transform);

    void draw() const;
    void set(float* primitiveTransform_mat, unsigned int texture) const;

};





}