
#pragma once

#include <cassert>

#include "lib/assert.hh"
#include "lib/io.hh"

#include "geo_camera.hh"
#include "mesh.hh"
#include "opengl.hh"





struct Geometry
{
    OpenGL::Program program;
    
    OpenGL::Buffer triangle_buffer;
    Mesh mesh;

    GeoCamera camera;

    float background_color = 0.0f;
    OpenGL::Uniform<float> color {"color"};
    OpenGL::Uniform<float> time_s {"time_s"};
    OpenGL::Uniform<m4f4> view_matrix {"view"};
    OpenGL::Uniform<m4f4> projection_matrix {"projection"};

    void set_color(float _color)
    {
        glUseProgram(program.id);
        glUniform1f(color.location, _color);
    }
    void set_time_s(float _time_s)
    {
        glUseProgram(program.id);
        glUniform1f(time_s.location, _time_s);
    }
    void set_view_matrix(m4f4 _view_matrix)
    {
        glUseProgram(program.id);
        // glUniform1f(time_s.location, (float*) &_view_matrix);
        glUniformMatrix4fv(view_matrix.location, 1, GL_TRUE, _view_matrix.pointer_const());
        // glUniformMatrix4fv(view_matrix.location, 1, GL_FALSE, _view_matrix.pointer_const());
    }
    void set_projection_matrix(m4f4 _projection_matrix)
    {
        glUseProgram(program.id);
        // glUniform1f(time_s.location, (float*) &_projection_matrix);
        glUniformMatrix4fv(projection_matrix.location, 1, GL_TRUE, _projection_matrix.pointer_const());
        // glUniformMatrix4fv(view_matrix.location, 1, GL_FALSE, _projection_matrix.pointer_const());
    }

    i32 render_counter = 0;

    OpenGL::Point3f triangle_verts[3] = {
        { 0.0f,  1.0f, 0.0f}, // top center
        {-1.0f, -1.0f, 0.0f}, // bottom left
        { 1.0f, -1.0f, 0.0f}  // bottom right
    };

    // Arr<Mesh::V3C1> cube_v3c1[6] = 
    // {
    //     // lower left triangle
    //     {{-1.0f, -2.0f, 0.0f}, 0.2f},
    //     {{-1.0f,  2.0f, 0.0f}, 0.2f},
    //     {{ 1.0f, -2.0f, 0.0f}, 0.2f},
    //     // upper triangle
    //     {{ 1.0f,  2.0f, 0.0f}, 0.4f},
    //     {{ 1.0f, -2.0f, 0.0f}, 0.4f},
    //     {{-1.0f,  2.0f, 0.0f}, 0.4f},
    // };

    float new_last_vert[3] = {
        1.0f, -0.5f, 0.0f  // new bottom right
    };


    void init(int width, int height)
    {
        camera.init(width, height);
        camera.set_pos({0.0f, 0.0f, 10.0f});
        
        mesh.init();
        triangle_buffer.init(mesh);


        Str vertex_shader_file = IO::cat("src/opengl/shaders/geometry/vertex.vert");
        Str fragment_shader_file = IO::cat("src/opengl/shaders/geometry/fragment.frag");
        program.init( vertex_shader_file, fragment_shader_file);
    


        OpenGL::print_opengl_error();

        glUseProgram(program.id);

        time_s.location = glGetUniformLocation(program.id, time_s.name);
        // glUniform1f(time_s.location, time_s.value);
        set_time_s(0.0f);

        color.location = glGetUniformLocation(program.id, color.name);
        glUniform1f(color.location, mesh.color);

        // view_matrix.value = m4f4::scale(1.0f);
        view_matrix.location = glGetUniformLocation(program.id, view_matrix.name);
        set_view_matrix(m4f4{});
        // glUniformMatrix4fv(view_matrix.location, 1, GL_FALSE, view_matrix.value.pointer_const());

        // projection_matrix.value = m4f4::scale(1.0f);
        projection_matrix.location = glGetUniformLocation(program.id, projection_matrix.name);
        set_projection_matrix(m4f4{});
        // glUniformMatrix4fv(projection_matrix.location, 1, GL_FALSE, projection_matrix.value.pointer_const());
    }

    void draw(float _time_s)
    {
        glUseProgram(program.id);


        m4f4 start_mat = m4f4::scale(1.0f);
        m4f4 end_mat = m4f4::scale(0.0f);
        // m4f4 end_mat = shader_matrix;
        float lerp_value = ((float)render_counter) / 1000.0f;
        // view_matrix.value = m4f4::lerp(start_mat, camera.view.matrix, lerp_value);
        // view_matrix.value = m4f4::lerp(start_mat, camera.view, lerp_value);


        m4f4 _view_matrix = camera.view;
        m4f4 _projection_matrix = camera.projection.matrix();

        camera.reload_matrixes();
        set_view_matrix(camera.view);
        set_projection_matrix(camera.projection.matrix());
        
        // glUniformMatrix4fv(view_matrix.location, 1, GL_TRUE, view_matrix.value.ptr());
        // glUniformMatrix4fv(projection_matrix.location, 1, GL_TRUE, projection_matrix.value.ptr());
        // view_matrix.value.print();
        
        // BACKGROUND
        background_color = 0.5f * sin(_time_s);
        glClearColor(background_color, background_color, background_color, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TRIANGLE
        triangle_buffer.draw();

        OpenGL::print_opengl_error();
        render_counter++;
    }


    void destroy()
    {
        program.destroy();
        triangle_buffer.destroy();
    }
};
