#include <iostream>

#include <glad/glad.h>

#include "lib/fs.hh"
#include "lib/process.hh"
#include "lib/log.hh"


#include "draw.hh"
#include "draw_shader.hh"

// Read initial screen dimensions
#include "PSO_util.hpp"


namespace draw {

    DrawShader draw_shader;

        // ONLY FOR INITIAL COPYING INTO SHADER
        float _viewportTransform16[16] = {
                                    2.0f / (float)SCREEN_INIT_WIDTH, 0, 0, -1.0f,
                                    0, 2.0f / (float)SCREEN_INIT_HEIGHT, 0, -1.0f,
                                    0, 0, 1, 0,
                                    0, 0, 0, 1,
        };

        float squareVertices[30] = {
               0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
               1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top-right
               0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // top-left
               1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top-right
               0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
               1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        };

        DrawShader::DrawShader() {
            for (size_t i = 0; i < 16; i++) {
                viewportTransform16[i] = _viewportTransform16[i];
            }
        }
        void DrawShader::set_window_info(float width, float height, float _xscale, float _yscale) {
            viewportTransform16[0] = 2.0f * _xscale / width;
            viewportTransform16[5] = 2.0f * _yscale / height;
        }



        void DrawShader::init() {
            compile_shader();
            

            glUseProgram(shader_id);


            // GET UNIFORM LOCATIONS
            uiViewportTransformLoc = glGetUniformLocation(shader_id, "viewportTransform");
            uiPrimitiveTransformLoc = glGetUniformLocation(shader_id, "primitiveTransform");


            // VAO, VBO
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

            glBindVertexArray(vao);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        


        void DrawShader::set(float* primitiveTransform_mat, unsigned int texture) {
            glUseProgram(shader_id);
            // GL_TRUE : Transpose before loading into uniform!
            glUniformMatrix4fv(uiViewportTransformLoc, 1, GL_TRUE, viewportTransform16);
            glUniformMatrix4fv(uiPrimitiveTransformLoc, 1, GL_TRUE, primitiveTransform_mat);

            this->texture = texture;
        }

        void DrawShader::draw() {
            glUseProgram(shader_id);

            // glDisable(GL_DEPTH_TEST);

            glBindVertexArray(vao);
            glBindTexture(GL_TEXTURE_2D, texture);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glDisable(GL_BLEND);

            // glEnable(GL_DEPTH_TEST);
        }




        void DrawShader::compile_shader() {


            // READ FILE
            std::string vertFileString = plib::fs_cat(vert_path);
            std::string fragFileString = plib::fs_cat(frag_path);

            const char* vertFile_cstr = vertFileString.c_str();
            const char* fragFile_cstr = fragFileString.c_str();



            // COMPILE 
            unsigned int vert_shader, frag_shader;

            // Vertex shader
            vert_shader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vert_shader, 1, &vertFile_cstr, NULL);
            glCompileShader(vert_shader);
            shader_error_check(vert_shader, "VERTEX");

            // Fragment Shader
            frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(frag_shader, 1, &fragFile_cstr, NULL);
            glCompileShader(frag_shader);
            shader_error_check(frag_shader, "FRAGMENT");

            // shader Program
            shader_id = glCreateProgram(); //Creates a program object
            glAttachShader(shader_id, vert_shader);
            glAttachShader(shader_id, frag_shader);
            glLinkProgram(shader_id);
            program_error_check(shader_id);

            // Individual shaders are built and linked.
            glDeleteShader(vert_shader);
            glDeleteShader(frag_shader);
        }

        void DrawShader::shader_error_check(unsigned int gl_shader, std::string shader_type) {

            int success;
            char infoLog[1024];

            glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(gl_shader, 1024, NULL, infoLog);
                plib::plog_error("DRAW ", "SHADER_" + shader_type + " ", "Failed to build DrawShader shader. : InfoLog = " + std::string(infoLog));
            }

        }
        void DrawShader::program_error_check(unsigned int gl_program) {

            int success;
            char infoLog[1024];

            glGetProgramiv(gl_program, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(gl_program, 1024, NULL, infoLog);
                plib::plog_error("DRAW ", "SHADER_PROGRAM ", "Failed to link DrawShader program. : InfoLog = " + std::string(infoLog));
            }

        }


}
