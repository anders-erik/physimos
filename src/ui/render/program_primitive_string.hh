
#pragma once


#include "math/vecmat.hh"

#include "opengl/program.hh"

#include "ui/font/font_bitmap.hh"

#include "ui/ui_shader_string.hh" // VertexTexture


namespace UI {




class ProgramPrimitiveString : public opengl::Program {


    unsigned int vao;
    unsigned int vbo;

    unsigned int uiViewportTransformLoc;
    unsigned int uiPrimitiveTransformLoc;

    unsigned int texture;

    unsigned int vertex_count;

public:

    ProgramPrimitiveString() : Program("ui/string") {};


    void init();
    void set_viewport_transform(m4f4 _viewport_transform);
    void set_texture(unsigned int new_texture);


    void set_vertex_data(shader::VertexTexture* vertex_data, unsigned int sizeof_vertex_data);


    void set_primitive_transform(float* primitiveTransform_mat) const;

    void draw() const;

};





}