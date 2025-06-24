
#pragma once

#include <array>

#include "math/vecmat.hh"

#include "opengl/program.hh"

#include "rend/defs.hh"

template <typename T>
struct Vec;

class Shader2DGraphPoint : protected opengl::ShaderProgram {

    uint camera2D_mat_LOC;
    uint model_mat_LOC;
    uint color_LOC;

    uint vao_p;     // point data vao
    uint vbo_x_p;   // x point buffer
    uint vbo_y_p;   // y point buffer

    uint point_count = 0; // number of points in input x-vector

public:

    Shader2DGraphPoint() : ShaderProgram("2D/graph") {};

    void init();

    void set_camera_matrix(m3f3 _camera_matrix);

    void set_point_data(Vec<float>& x, Vec<float>& y);
    void draw_points() const;

private:

};




