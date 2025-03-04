#include <iostream>

#include <glad/glad.h>
#include "opengl/shader.hh"

#include "lib/fs.hh"
#include "lib/process.hh"
#include "lib/log.hh"


#include "draw.hh"
#include "draw_shader.hh"

// Read initial screen dimensions
#include "PSO_util.hpp"


namespace draw {

    DrawShader draw_shader;

        float squareVertices[30] = {
               0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
               1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top-right
               0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // top-left
               1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top-right
               0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
               1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        };

        DrawShader::DrawShader() {

            viewportTransformMat4f.x.x = 1.0f;
            viewportTransformMat4f.y.y = 1.0f;
            viewportTransformMat4f.z.z = 1.0f;
            viewportTransformMat4f.w.w = 1.0f;

            viewportTransformMat4f.x.w = -1.0f;
            viewportTransformMat4f.y.w = -1.0f;
        }

        void DrawShader::set_window_info(const ViewportContext& viewport_context) {

            float draw_zoom = 1.0f;
            float pan_x = 0.0f;
            float pan_y = 0.0f;

            // With zero zoom, one pixel in texture is one logical texture
            viewportTransformMat4f.x.x = 2.0f / (viewport_context.phys_win.logical.w * draw_zoom);
            viewportTransformMat4f.y.y = 2.0f / (viewport_context.phys_win.logical.h * draw_zoom);

            // Shift from NDC center to bottom left, then shift according to main_view percent location
            viewportTransformMat4f.x.w = -1.0f + 2.0f * viewport_context.view_sizes.main_view_percent.x + pan_x;
            viewportTransformMat4f.y.w = -1.0f + 2.0f * viewport_context.view_sizes.main_view_percent.y + pan_y;

        }



        void DrawShader::init() {
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
            glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

            glBindVertexArray(vao);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            set_window_info primitiveTransform_mat, unsigned int texture) {
            glUseProgram(shader_id);
            // GL_TRUE : Transpose before loading into uniform!
            // glUniformMatrix4fv(uiViewportTransformLoc, 1, GL_TRUE, viewportTransform16);
            glUniformMatrix4fv(uiViewportTransformLoc, 1, GL_TRUE, (float *)&viewportTransformMat4f);
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



}
