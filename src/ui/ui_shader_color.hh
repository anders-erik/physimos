#ifndef UI_SHADER_COLOR_HH
#define UI_SHADER_COLOR_HH

#include <string>

#include "ui/ui_globals.hh"

namespace UI {
    namespace shader {
        
        typedef class ColorShader {
            public:
                std::string vert_path = "src/ui/shaders/color_vert.glsl";
                std::string frag_path = "src/ui/shaders/color_frag.glsl";

                unsigned int shader_id;
                unsigned int vao;
                unsigned int vbo;

                unsigned int uiViewportTransformLoc;
                unsigned int uiPrimitiveTransformLoc;

                unsigned char color4Loc;
                unsigned char darknessShiftLoc;

                /** Tranform matrix based on the current viewport dimensions */
                float viewportTransform16[16];
                

                /** Compiles, load locations, and set permanent vao. */
                void init();

                /** Reads shader files, compile, and returns 0 on failure. */
                void compile_shader();

                void shader_error_check(unsigned int gl_shader, std::string shader_type);
                void program_error_check(unsigned int gl_program);

                /** Set values needed for render draw. UI transform and texture.  */
                void set(float* primitiveTransform_mat, float _darkness_shift, Color color4);
                /** Makes rendering call with values previously specified using a set()-call. */
                void draw();

                /** Update the viewport transformation matrix using physimos window info */
                void set_window_info(float width, float height, float _xscale, float _yscale);

                ColorShader();
        } ColorShader;

        /** UI shader rendering a rectangle with texture. */
        extern ColorShader color_shader;
    }
}

#endif