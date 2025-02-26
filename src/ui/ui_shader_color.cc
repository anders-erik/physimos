#include <iostream>

#include <glad/glad.h>

#include "lib/fs.hh"
#include "lib/process.hh"
#include "lib/log.hh"

#include "ui/ui_globals.hh"
#include "ui/ui_shader_color.hh"

// Read initial screen dimensions
#include "PSO_util.hpp"
// #include "ui_shader.hh"


namespace UI {
    namespace shader {

        ColorShader color_shader;

        // ONLY FOR INITIAL COPYING INTO SHADER
        float _ColorViewportTransform16[16] = {
                                    2.0f / (float)SCREEN_INIT_WIDTH, 0, 0, -1.0f,
                                    0, 2.0f / (float)SCREEN_INIT_HEIGHT, 0, -1.0f,
                                    0, 0, 1, 0,
                                    0, 0, 0, 1,
        };

        float colorSquareVertices[30] = {
               0.0f, 0.0f, 0.0f,   // bottom-left
               1.0f, 1.0f, 0.0f,   // top-right
               0.0f, 1.0f, 0.0f,   // top-left
               1.0f, 1.0f, 0.0f,   // top-right
               0.0f, 0.0f, 0.0f,   // bottom-left
               1.0f, 0.0f, 0.0f,   // bottom-right
        };

        ColorShader::ColorShader() {
            for (size_t i = 0; i < 16; i++) {
                viewportTransform16[i] = _ColorViewportTransform16[i];
            }
        }
        void ColorShader::set_window_info(float width, float height, float _xscale, float _yscale) {
            viewportTransform16[0] = 2.0f * _xscale / width;
            viewportTransform16[5] = 2.0f * _yscale / height;
        }



        void ColorShader::init() {
            compile_shader();
            

            glUseProgram(shader_id);


            // GET UNIFORM LOCATIONS
            uiViewportTransformLoc = glGetUniformLocation(shader_id, "viewportTransform");
            uiPrimitiveTransformLoc = glGetUniformLocation(shader_id, "primitiveTransform");
            darknessShiftLoc = glGetUniformLocation(shader_id, "darknessShift");
            color4Loc = glGetUniformLocation(shader_id, "color4");


            // VAO, VBO
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(colorSquareVertices), colorSquareVertices, GL_STATIC_DRAW);

            glBindVertexArray(vao);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void ColorShader::stencil_start_new_capture(){

            glEnable(GL_STENCIL_TEST);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glClear(GL_STENCIL_BUFFER_BIT); 

            glStencilFunc(GL_ALWAYS, 1, 0xFF); 
            glStencilMask(0xFF); // each bit is written to the stencil buffer as is

        }
        void ColorShader::stencil_apply(){

            glStencilFunc(GL_EQUAL, 1, 0xFF);
            glStencilMask(0x00); // each bit ends up as 0 in the stencil buffer (disabling writes)

        }
        void ColorShader::stencil_disable(){
            glDisable(GL_STENCIL_TEST);
        }
        


        void ColorShader::set(float* primitiveTransform_mat, float _darkness_shift, Color color4) {
            glUseProgram(shader_id);
            // GL_TRUE : Transpose before loading into uniform!
            glUniformMatrix4fv(uiViewportTransformLoc, 1, GL_TRUE, viewportTransform16);
            glUniformMatrix4fv(uiPrimitiveTransformLoc, 1, GL_TRUE, primitiveTransform_mat);
            glUniform1f(darknessShiftLoc, _darkness_shift);
            glUniform4f(color4Loc, color4.R, color4.G, color4.B, color4.A);
        }

        void ColorShader::draw() {
            glUseProgram(shader_id);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glDisable(GL_BLEND);
        }




        void ColorShader::compile_shader() {


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

        void ColorShader::shader_error_check(unsigned int gl_shader, std::string shader_type) {

            int success;
            char infoLog[1024];

            glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(gl_shader, 1024, NULL, infoLog);
                plib::plog_error("UI ", "SHADER_" + shader_type + " ", "Failed to build TextureShader shader. : InfoLog = " + std::string(infoLog));
            }

        }
        void ColorShader::program_error_check(unsigned int gl_program) {

            int success;
            char infoLog[1024];

            glGetProgramiv(gl_program, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(gl_program, 1024, NULL, infoLog);
                plib::plog_error("UI ", "SHADER_PROGRAM ", "Failed to link TextureShader program. : InfoLog = " + std::string(infoLog));
            }

        }

    }
}
