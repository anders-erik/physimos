
#include "glad/glad.h"


#include "math/vecmat.hh"
#include "math/vec.hh"

#include "rend/scene2D/shader_graph_line.hh"





void Shader2DGraphLine::
init()
{
    build();

    glUseProgram(id);

    // GET UNIFORM LOCATIONS
    camera2D_mat_LOC = glGetUniformLocation(id, "camera2D_mat");
    model_mat_LOC = glGetUniformLocation(id, "model_mat");

    color_LOC = glGetUniformLocation(id, "color");
    glUniform4f(color_LOC, 0.5f, 0.0f, 1.0f, 1.0f);


    glGenBuffers(1, &vao_l);
    glGenBuffers(1, &vbo_x_l);
    glGenBuffers(1, &vbo_y_l);

}



void Shader2DGraphLine::
set_camera_matrix(m3f3 _camera_matrix)
{
    glUseProgram(id);

    glUniformMatrix3fv(camera2D_mat_LOC, 1, GL_TRUE, _camera_matrix.pointer() );
}



void Shader2DGraphLine::
set_line_data(Vec<float>& x, Vec<float>& y)
{
    line_count = x.size() - 1;

    Vec<float> x_l = points_to_line_points(x);
    Vec<float> y_l = points_to_line_points(y);


    glBindVertexArray(vao_l);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_x_l);
    glBufferData(GL_ARRAY_BUFFER, x_l.size_byte(), x_l.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_y_l);
    glBufferData(GL_ARRAY_BUFFER, y_l.size_byte(), y_l.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}





void Shader2DGraphLine::
draw_lines() const 
{
    glUseProgram(id);

    // Lines
    glBindVertexArray(vao_l);
    glLineWidth(1);
    glDrawArrays(GL_LINES, 0, line_count * 2);
}




Vec<float> Shader2DGraphLine::
points_to_line_points(Vec<float> points)
{
    // Two points for each line
    uint line_point_count = line_count * 2;

    Vec<float> line_points {line_point_count};

    for(size_t i = 0; i < line_count; i++)
    {
        line_points[i*2]    = points[i];
        line_points[i*2+1]  = points[i+1]; 
    }

    return line_points;
}
