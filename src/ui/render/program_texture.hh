
#pragma once


#include "math/vecmat.hh"

#include "opengl/program.hh"


namespace UI {



class ProgramBaseTexture : public opengl::Program {

    unsigned int vao;
    unsigned int vbo;

    unsigned int uiViewportTransformLoc;
    unsigned int uiPrimitiveTransformLoc;

    // unsigned int texture;


public:

    // TODO: Figure out why the new shader is not working, despite exactly the same source
    // ProgramBaseTexture() : Program("ui/texture") {};
    ProgramBaseTexture() : Program("ui/base_texture") {};

    void init();
    void set_viewport_transform(m4f4 _viewport_transform);

    void set(float* primitiveTransform_mat, unsigned int texture) const;
    void draw() const;

};





}