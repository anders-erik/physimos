#ifndef UI_SHADER_TEXTURE_HH
#define UI_SHADER_TEXTURE_HH

#include <string>

#include "ui/ui_globals.hh"

namespace UI {
    namespace shader {
        
        typedef class TextureShader {
            public:
                std::string vert_path = "src/ui/shaders/texture_vert.glsl";
                std::string frag_path = "src/ui/shaders/texture_frag.glsl";

                unsigned int shader_id;
                unsigned int vao;
                unsigned int vbo;

                unsigned int uiViewportTransformLoc;
                unsigned int uiPrimitiveTransformLoc;
                unsigned int texture;

                /** Tranform matrix based on the current viewport dimensions */
                float viewportTransform16[16];
                

                /** Compiles, load locations, and set permanent vao. */
                void init();

                /** Reads shader files, compile, and returns 0 on failure. */
                void compile_shader();

                void shader_error_check(unsigned int gl_shader, std::string shader_type);
                void program_error_check(unsigned int gl_program);

                /** Set values needed for render draw. UI transform and texture.  */
                void set(float* primitiveTransform_mat, unsigned int texture);
                /** Makes rendering call with values previously specified using a set()-call. */
                void draw();

                /** Update the viewport transformation matrix using physimos window info */
                void set_window_info(float width, float height, float _xscale, float _yscale);

                TextureShader();
        } TextureShader;

        /** UI shader rendering a rectangle with texture. */
        extern TextureShader texture_shader;
    }
}

#endif