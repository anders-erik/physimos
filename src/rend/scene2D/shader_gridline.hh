
#pragma once

#include "lib/arr.hh"
#include "math/number.hh"

#include "math/vecmat.hh"

#include "opengl/program.hh"

#include "rend/defs.hh"


class Shader2DGridline : private opengl::ShaderProgram {

    uint camera2D_mat_LOC;
    uint color_LOC;

    uint vao_xy;    // x-y axes data
    uint vbo_xy;    // point buffer for x-y axes

    const uint xy_axis_line_count   = 2;
    const uint auxillary_line_count = 4;
    const uint line_count           = xy_axis_line_count + auxillary_line_count;

    Arr<R2<float>> line_points;

    const float xy_max = 100.0f;
public:

    Shader2DGridline() : ShaderProgram("2D/gridline") {};

    void init();

    void set_line_point_buffer();

    void set_camera_matrix(m3f3 _camera_matrix);

    void draw_gridlines();

};




