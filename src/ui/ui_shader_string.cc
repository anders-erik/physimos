#include <iostream>

#include <glad/glad.h>

#include "lib/fs.hh"
#include "lib/process.hh"
#include "lib/log.hh"

#include "ui/ui_globals.hh"
#include "ui/ui_shader_string.hh"

// Read initial screen dimensions
#include "PSO_util.hpp"
// #include "ui_shader.hh"

#include "opengl/shader.hh"
#include "ui/ui_texture.hh"

namespace UI {
    namespace shader {

        StringShader string_shader;

        // ONLY FOR INITIAL COPYING INTO SHADER
        float _viewportTransform16_[16] = {
                                    2.0f / (float)SCREEN_INIT_WIDTH, 0, 0, -1.0f,
                                    0, 2.0f / (float)SCREEN_INIT_HEIGHT, 0, -1.0f,
                                    0, 0, 1, 0,
                                    0, 0, 0, 1,
        };

        float squareVertices_[30] = {
               0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
               1.0f, 1.0f, 0.0f, 1.0f, 0.01f,   // top-right
               0.0f, 1.0f, 0.0f, 0.0f, 0.01f,   // top-left
               1.0f, 1.0f, 0.0f, 1.0f, 0.01f,   // top-right
               0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
               1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        };

        StringShader::StringShader() {
            for (size_t i = 0; i < 16; i++) {
                viewportTransform16[i] = _viewportTransform16_[i];
            }
        }
        void StringShader::set_window_info(float width, float height, float _xscale, float _yscale) {
            viewportTransform16[0] = 2.0f * _xscale / width;
            viewportTransform16[5] = 2.0f * _yscale / height;
        }


        void StringShader::set_bitmap(pimage::Bitmap& bitmap){

            texture = texture::new_from_bitmap(bitmap);

        }

        void StringShader::set_texture(unsigned int new_texture){
            texture = new_texture;
        }



        void StringShader::init() {
            // compile_shader();

            shader_id = opengl::build_program_vert_frag(vert_path, frag_path);
            

            glUseProgram(shader_id);


            // GET UNIFORM LOCATIONS
            uiViewportTransformLoc = glGetUniformLocation(shader_id, "viewportTransform");
            uiPrimitiveTransformLoc = glGetUniformLocation(shader_id, "primitiveTransform");


            // VAO, VBO
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices_), squareVertices_, GL_STATIC_DRAW);

            glBindVertexArray(vao);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        


        void StringShader::set_primitive_transform(float* primitiveTransform_mat) {
            glUseProgram(shader_id);

            // We want the with of a char!
            float orig_x = primitiveTransform_mat[0];
            float orig_y = primitiveTransform_mat[5];
            // offset for visibility
            primitiveTransform_mat[7] -= 20;
            primitiveTransform_mat[0] = 11;
            primitiveTransform_mat[5] = 20;

            // GL_TRUE : Transpose before loading into uniform!
            glUniformMatrix4fv(uiViewportTransformLoc, 1, GL_TRUE, viewportTransform16);
            glUniformMatrix4fv(uiPrimitiveTransformLoc, 1, GL_TRUE, primitiveTransform_mat);


            primitiveTransform_mat[0] = orig_x;
            primitiveTransform_mat[5] = orig_y;
            primitiveTransform_mat[7] += 20;

            // this->texture = texture;
        }

        void StringShader::set_vertex_data(VertexTexture* vertex_data, unsigned int sizeof_vertex_data){
            vertex_count = sizeof_vertex_data / sizeof(VertexTexture);

            glUseProgram(shader_id);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof_vertex_data, vertex_data, GL_STATIC_DRAW);

            glBindVertexArray(vao);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
            glEnableVertexAttribArray(2);

            glBindBuffer(GL_ARRAY_BUFFER, 0);

        }

        void StringShader::draw() {
            glUseProgram(shader_id);

            glBindVertexArray(vao);
            glBindTexture(GL_TEXTURE_2D, texture);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDrawArrays(GL_TRIANGLES, 0, vertex_count);
            glDisable(GL_BLEND);
        }



    }
}
