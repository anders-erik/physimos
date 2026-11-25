#pragma once


#include "opengl/loader.hh"
#include "opengl/util.hh"
#include "opengl/program.hh"

#include "mesh.hh"

namespace OpenGL
{

typedef float Point3f[3];


struct VertexShader
{
    GLuint id = 0;

    void init(Str _code)
    {
        const char* code_c_str = _code.to_c_str();
        id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(id, 1, &code_c_str, NULL);
        glCompileShader(id);
    }

    void destroy()
    {
        glDeleteShader(id);
        id = 0;
    }
};

struct FragmentShader
{
    GLuint id = 0;

    void init(Str _code)
    {
        const char* code_c_str = _code.to_c_str();
        id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(id, 1, &code_c_str, NULL);
        glCompileShader(id);
        // return shader;
    }

    void destroy()
    {
        glDeleteShader(id);
        id = 0;
    }
};


struct Program
{
    GLuint id = 0;
    VertexShader vertex_shader;
    FragmentShader fragment_shader;

    void init(Str vertex_shader_source, Str fragment_shader_source)
    {
        id = glCreateProgram();

        vertex_shader.init(vertex_shader_source);
        fragment_shader.init(fragment_shader_source);

        glAttachShader(id, vertex_shader.id);
        glAttachShader(id, fragment_shader.id);
        glLinkProgram(id);
    }


    void destroy()
    {
        glDeleteShader(fragment_shader.id);
        glDeleteShader(vertex_shader.id);
    }
};



struct Buffer
{

    GLuint id = 0;
    GLuint ebo = 0;
    GLuint vao = 0;

    float new_last_vert[3] = {
        1.0f, -0.5f, 0.0f  // new bottom right
    };


    Buffer* init(float* data, int data_size)
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);

        glDeleteBuffers(1, &id);

        if(false)
        {
            if(!glIsBuffer(id))
                Print::ln("1. buffer.id not yet a valid buffer");
            glGenBuffers(1, &id);
            if(!glIsBuffer(id))
                Print::ln("2. buffer.id still not a valid buffer");
            glBindBuffer(GL_ARRAY_BUFFER, id);
            if(glIsBuffer(id))
                Print::ln("3. buffer.id is a valid buffer");
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            if(glIsBuffer(id))
                Print::ln("4. buffer.id is still a valid buffer");
        }

        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);

        OpenGL::print_opengl_error();


        // float verts[9] = {
        //     0.0f,  1.0f, 0.0f, // top center
        //     -1.0f, -1.0f, 0.0f, // bottom left
        //     1.0f, -1.0f, 0.0f  // bottom right
        // };

        // glBufferData(GL_ARRAY_BUFFER, data_size, verts, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, data_size, (const void *)data, GL_STATIC_DRAW);
        // glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float), sizeof(new_last_vert), new_last_vert);
        // glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float), sizeof(new_last_vert), new_last_vert);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
        // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float), NULL);
        

        return this;
    }


    Buffer* init(Mesh& mesh)
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        

        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, mesh.verts.count_byte(), (const void*)mesh.verts.data(), GL_STATIC_DRAW);

        
        OpenGL::print_opengl_error();


        // float verts[9] = {
        //     0.0f,  1.0f, 0.0f, // top center
        //     -1.0f, -1.0f, 0.0f, // bottom left
        //     1.0f, -1.0f, 0.0f  // bottom right
        // };

        // glBufferData(GL_ARRAY_BUFFER, data_size, verts, GL_STATIC_DRAW);
        // glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float), sizeof(new_last_vert), new_last_vert);
        // glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float), sizeof(new_last_vert), new_last_vert);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0* sizeof(float)));
        glEnableVertexAttribArray(0);
        // glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3* sizeof(float)));
        // glEnableVertexAttribArray(1);
        
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.faces.count_byte(), (const void*)mesh.faces.data(), GL_STATIC_DRAW);

        // glBindBuffer(GL_ARRAY_BUFFER, id);

        return this;
    }

    void draw()
    {
        glEnableVertexAttribArray(vao);
        OpenGL::print_opengl_error();
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        // glDrawArrays(GL_LINES, 0, 6);
        // glDrawElements(GL_TRIANGLES, 2, GL_UNSIGNED_INT, (const void *)0);
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void *)0);
    }

    void destroy()
    {
        glDeleteBuffers(1, &id);
    }
};


// template <typename T>
// struct Uniform
// {
//     T value;
//     const char* name = "";
//     GLint location = 0;

//     Uniform(const char* name) : name {name} {}

// };


template <typename T>
struct Uniform
{
    const char* name = "";
    GLint location = 0;

    Uniform(const char* name) : name {name} {}
};


} // 'OpenGL' END
