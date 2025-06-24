
#pragma once

#include <array>

#include "math/vecmat.hh"

#include "opengl/program.hh"

#include "rend/defs.hh"

template <typename T>
struct Vec;

class Shader2DGraphLine : protected opengl::ShaderProgram {

    uint camera2D_mat_LOC;
    uint model_mat_LOC;
    uint color_LOC;

    uint vao_l;     // line data vao
    uint vbo_x_l;   // x line buffer
    uint vbo_y_l;   // y line buffer

    uint line_count = 0; // number of lines based on point count

public:

    Shader2DGraphLine() : ShaderProgram("2D/graph") {};

    void init();

    void set_camera_matrix(m3f3 _camera_matrix);

    void set_line_data(Vec<float>& x, Vec<float>& y);
    void draw_lines() const;

private:

/** Interleave points with themselves to enable line rendering. 
    {0, 1, 2, 3} -> {0, 1, 1, 2, 2, 3} */
Vec<float> points_to_line_points(Vec<float> points);

};




