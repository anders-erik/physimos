#ifndef UI_SHADER_HH
#define UI_SHADER_HH

#include <string>

#include "ui/ui_globals.hh"

namespace UI {
    namespace shader {
        
        typedef class TextureShader {
            public:
                std::string vert_path = "src/ui/shaders/texture_vert.glsl";
                std::string frag_path = "src/ui/shaders/texture_frag.glsl";

                unsigned int shader_id;

                //
                float viewportTransform16[16];
                void update_viewport_dimensions(float height, float width);


                /** Reads shader files, compile, and returns 0 on failure. */
                void compile_shader();

                void shader_error_check(unsigned int gl_shader, std::string shader_type);
                void program_error_check(unsigned int gl_program);

                /** Set value needed for render draw. Texture & ui transform.  */
                void set();
                /** Makes rendering call with previously set values. */
                void draw();

                TextureShader();
        } TextureShader;

        /** UI shader rendering a rectangle with texture. */
        extern TextureShader texture_shader;
    }
}

#endif