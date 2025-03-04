#ifndef DRAW_SHADER_HH
#define DRAW_SHADER_HH

#include <string>

#include "math/vec3.hh"

#include "conductor_viewport.hh"

// #include "ui/ui_globals.hh"

namespace draw {

        
        typedef class DrawShader {
            public:
                std::string vert_path = "src/draw/shaders/draw_vert.glsl";
                std::string frag_path = "src/draw/shaders/draw_frag.glsl";

                unsigned int shader_id;
                unsigned int vao;
                unsigned int vbo;

                unsigned int uiViewportTransformLoc;
                unsigned int uiPrimitiveTransformLoc;
                unsigned int texture;

                /** Tranform matrix based on the current viewport dimensions */
                // float viewportTransform16[16];
                pmath::Mat4f_base viewportTransformMat4f;

                

                /** Compiles, load locations, and set permanent vao. */
                void init();

                /** Set values needed for render draw. UI transform and texture.  */
                void set(float* primitiveTransform_mat, unsigned int texture);
                /** Makes rendering call with values previously specified using a set()-call. */
                void draw();

                /** Update the viewport transformation matrix using physimos window info */
                void set_window_info(const ViewportContext& viewport_context);

                DrawShader();
        } DrawShader;

        /** UI shader rendering a rectangle with texture. */
        extern DrawShader draw_shader;

}

#endif