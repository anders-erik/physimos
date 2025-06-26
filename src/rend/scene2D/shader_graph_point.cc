
#include "glad/glad.h"


#include "math/vecmat.hh"
#include "math/vec.hh"

#include "shader_graph_point.hh"





void Shader2DGraphPoint::
init()
{
    build();

    glUseProgram(id);

    // GET UNIFORM LOCATIONS
    camera2D_mat_LOC = glGetUniformLocation(id, "camera2D_mat");
    model_mat_LOC = glGetUniformLocation(id, "model_mat");

    color_LOC = glGetUniformLocation(id, "color");
    glUniform4f(color_LOC, 0.0f, 0.0f, 0.0f, 1.0f);

    glGenVertexArrays(1, &vao_p);
    glGenBuffers(1, &vbo_x_p);
    glGenBuffers(1, &vbo_y_p);
}



void Shader2DGraphPoint::
set_camera_matrix(m3f3 _camera_matrix)
{
    glUseProgram(id);

    glUniformMatrix3fv(camera2D_mat_LOC, 1, GL_TRUE, _camera_matrix.pointer() );
}




void Shader2DGraphPoint::
set_point_data(Vec<float>& x, Vec<float>& y)
{
    point_count = x.size();

    glUseProgram(id);

    glBindVertexArray(vao_p);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_x_p);
    glBufferData(GL_ARRAY_BUFFER, x.size_byte(), x.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_y_p);
    glBufferData(GL_ARRAY_BUFFER, y.size_byte(), y.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}





void Shader2DGraphPoint::
draw_points() const 
{
    glUseProgram(id);

    // points
    glBindVertexArray(vao_p);
    glPointSize(2);
    glDrawArrays(GL_POINTS, 0, point_count);
}





