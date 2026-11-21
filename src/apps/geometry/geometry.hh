
#pragma once

#include <cassert>

#include "lib/assert.hh"
#include "lib/io.hh"

#include "geo_camera.hh"
#include "opengl.hh"



//     const char* vert_src =
// "#version 330 core\n"

// "layout(location = 0) in vec3 aPos;\n"

// // "uniform mat4 view_matrix;\n"
// "uniform mat4 view;\n"
// "uniform mat4 projection;\n"


// "void main() {\n"
// "    vec4 pos = vec4(aPos, 1.0);\n"
// // "    gl_Position = pos * view_matrix;\n"
// "    gl_Position = projection * view * pos;\n"
// // "    gl_Position = pos;\n"
// "}";

    const char* frag_src =
"#version 330 core\n"

// "uniform float color;\n"
"uniform float time_s;\n"

"out vec4 FragColor;\n"

"float color_frequency_multiplier = 2.0;\n"

"void main() {\n"
"       float color_component = 0.5 * sin(time_s * color_frequency_multiplier);\n"
// "       float color_component = time_s;\n"

// "    FragColor = vec4(color, 0.0, 0.0, 1.0);\n"
// "    FragColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"    FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
// "    FragColor = vec4(color_component, color_component, color_component, 1.0);\n"
// "    FragColor = vec4(sin(color_component), 0.0, 0.0, 1.0);\n"
// "    FragColor = vec4(sin(time_s), 0.0, 0.0, 1.0);\n"
"}";



struct Geometry
{
    OpenGL::Program program;
    OpenGL::Buffer triangle_buffer;

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
        {0.0f,  1.0f, 0.0f}, // top center
        {-1.0f, -1.0f, 0.0f}, // bottom left
        {1.0f, -1.0f, 0.0f}  // bottom right
    };

    float new_last_vert[3] = {
        1.0f, -0.5f, 0.0f  // new bottom right
    };


    void init()
    {
        camera.init();
        camera.set_pos({0.0f, 0.2f, 1.0f});

        // camera.position = {10.0f, 0.0f, 5.0f};


        // camera.rotation.rotate(f3::X(), 0.0f);
        // camera.rotation.rotate(f3::Y(), -0.0f);
        // camera.rotation.rotate(f3::Z(), 1.0f);
        

        // camera.rotation.rotate(f3::X(), 1.0f);
        // camera.rotation.rotate(f3::Y(), -0.2f);
        // camera.rotation.rotate(f3::Z(), 1.0f);
        
        // program.init(vert_src, frag_src);

        triangle_buffer.init((float*)triangle_verts, sizeof(triangle_verts), OpenGL::Buffer::pos_3_f);
        Str vertex_shader_file = IO::cat("src/opengl/shaders/geometry/vertex.vert");
        Str fragment_shader_file = IO::cat("src/opengl/shaders/geometry/fragment.frag");
        program.init( vertex_shader_file, fragment_shader_file);
    
        // const char* vertex_shader_file_to_c_str = vertex_shader_file.to_c_str();
        // Str vertex_shader_from_static_c = Str{vert_src};
        // const char* vertex_shader_from_static_c_to_c_str = vertex_shader_from_static_c.to_c_str();

        // const char* vertex_shader_file_c = (const char*) vertex_shader_file.to_c_str();
        // Print::ln(vertex_shader_file_to_c_str);
        // Print::ln(vertex_shader_from_static_c_to_c_str);

        // const char* vertex_shader_from_c_str__ = (const char*) vertex_shader_from_static_c;

        // int result = strcmp(vertex_shader_file_to_c_str, vertex_shader_from_static_c_to_c_str);
        // Print::buf("strcmp = ");
        // Print::ln(Str::SI(result));

        // Assert( strcmp(vertex_shader_file.to_c_str(), vertex_shader_file_c) );
        // assert( strcmp(vertex_shader_file_to_c_str, vertex_shader_from_static_c_to_c_str) );
        // assert(strcmp(vertex_shader_from_c_str__, vertex_shader_from_c_str__));
        // assert(vertex_shader_file.to_c_str() == vertex_shader_from_c_str.to_c_str());


        OpenGL::print_opengl_error();

        glUseProgram(program.id);

        time_s.location = glGetUniformLocation(program.id, time_s.name);
        // glUniform1f(time_s.location, time_s.value);
        set_time_s(0.0f);

        color.location = glGetUniformLocation(program.id, color.name);
        glUniform1f(color.location, 0.0f);

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

        // VIEW MATRIX
        // view_matrix.value = m4f4::scale(0.5f * sin(time_s.value));
        // view_matrix.value = m4f4::scale(0.5f);
        // view_matrix.value = m4f4{};
        // camera.update_matrices();
        // camera.set_orbit_center();
        // camera.rotation = camera.rotation.rotate(f3::X(), 0.01f);
        // camera.rotation = camera.rotation.rotate(f3::Y(), 0.01f);
        // camera.rotation = camera.rotation.rotate(f3::Z(), 0.01f);
        // m4f4 view_mat = camera.view.matrix;
        // m4f4 projection_mat = camera.projection.matrix;
        // m4f4 model_mat = m4f4{};
        // m4f4 shader_matrix = camera.projection.matrix * camera.view.matrix * m4f4{};
        // m4f4 shader_matrix = camera.view.matrix * camera.projection.matrix * m4f4{};
        // shader_matrix.print();
        // Print::ln("");
        // view_mat.print();
        // view_matrix.value = view_mat * projection_mat;
        // view_matrix.value = projection_mat * view_mat * model_mat;
        // view_matrix.value = view_mat;

        m4f4 start_mat = m4f4::scale(1.0f);
        m4f4 end_mat = m4f4::scale(0.0f);
        // m4f4 end_mat = shader_matrix;
        float lerp_value = ((float)render_counter) / 1000.0f;
        // view_matrix.value = m4f4::lerp(start_mat, camera.view.matrix, lerp_value);
        // view_matrix.value = m4f4::lerp(start_mat, camera.view, lerp_value);

        // view_matrix.value = start_mat;
        // m4f4 final_matrix_1 = m4f4::lerp(start_mat, end_mat, 1.0f);
        // m4f4 final_matrix_2 = m4f4::lerp(end_mat, end_mat, 1.0f);

        // view_matrix.value = m4f4::lerp(start_mat, view_mat, 1.0f);
        // view_matrix.value = m4f4::lerp(end_mat, end_mat, lerp_value);

        // view_matrix.value = start_mat;

        // view_matrix.value.x.x = 0.4f;
        // view_matrix.value = view_matrix.value * m4f4::rotation_z(0.01f);
        // view_matrix.value += m4f4::translation({0.01f, 0.0f, 0.0f});
        // view_matrix.value.x.w += 0.01f;
        // view_matrix.value.print();

        // camera.set_pos({0.2f, 0.0f, 3.9f});
        // camera.move(f3::X() * 0.001f);
        // camera.move({0.01f, 0.0f, 0.0f});

        m4f4 _view_matrix = camera.view;
        m4f4 _projection_matrix = camera.projection;

        set_view_matrix(camera.view);
        set_projection_matrix(camera.projection);
        
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
