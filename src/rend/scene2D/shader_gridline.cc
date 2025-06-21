
#include "glad/glad.h"

#include "lib/arr.hh"
#include "math/types.hh"

#include "math/vecmat.hh"
#include "math/vec.hh"

#include "shader_gridline.hh"





void Shader2DGridline::
init()
{
    glUseProgram(id);


    camera2D_mat_LOC = glGetUniformLocation(id, "camera2D_mat");
    color_LOC = glGetUniformLocation(id, "color");
    glUniform4f(color_LOC, 1.0f, 1.0f, 1.0f, 1.0f);



    Arr<R2<float>> line_points {line_count * 2};
    // X-Y axes
    line_points[0] = {-xy_max,  0.0f};
    line_points[1] = { xy_max,  0.0f};
    line_points[2] = { 0.0f,    -xy_max};
    line_points[3] = { 0.0f,    xy_max};
    // Auxillary lines
    line_points[4]  = {-xy_max, -1.0f}; // y = -1
    line_points[5]  = { xy_max, -1.0f};
    line_points[6]  = {-xy_max,  1.0f}; // y = 1
    line_points[7]  = { xy_max,  1.0f};
    line_points[8]  = { -1.0f,    -xy_max}; // x = -1
    line_points[9]  = { -1.0f,    xy_max};
    line_points[10] = {  1.0f,    -xy_max}; // x = 1
    line_points[11] = {  1.0f,    xy_max};


    glGenVertexArrays(1, &vao_xy);
    glBindVertexArray(vao_xy);

    glGenBuffers(1, &vbo_xy);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_xy);
    glBufferData(GL_ARRAY_BUFFER, line_points.size_byte(), line_points.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(R2<float>), (void *)0);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



void Shader2DGridline::
set_camera_matrix(m3f3 _camera_matrix)
{
    glUseProgram(id);

    glUniformMatrix3fv(camera2D_mat_LOC, 1, GL_TRUE, _camera_matrix.pointer() );
}




void Shader2DGridline::
draw_gridlines() const 
{
    glUseProgram(id);

    glBindVertexArray(vao_xy);

    // x=y axes
    glLineWidth(2);
    glDrawArrays(   GL_LINES, 
                    0, 
                    xy_axis_line_count * 2);

    // aux axes
    glLineWidth(0.5);
    glDrawArrays(   GL_LINES, 
                    xy_axis_line_count * 2, 
                    auxillary_line_count * 2);
}





