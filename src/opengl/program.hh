#ifndef OPENGL_SHADER_HH
#define OPENGL_SHADER_HH

#include <string>


namespace opengl {

    enum class Programs {
        phont_texture,
        phont_char,

        ndc_texture,
        ndc_black,
        ndc_white,

        Texture2D,

        ModelColor,
        ModelTexture,
        Axes,
        Vector,
    };

    unsigned int    gpu_get_program(Programs program_enum);
    void            gpu_use_program(Programs program_enum);


    /** Compile single vertex and fragment shader, links program, and returns program name. */
    unsigned int build_program_vert_frag(Programs program_enum);



    /** Compile single vertex and fragment shader, links program, and returns program name. */
    unsigned int build_program_vert_frag(std::string vert_path, std::string frag_path);




    /** INTERNAL */

    void shader_error_check(unsigned int gl_shader, std::string shader_type, std::string shader_path);

    void program_error_check(unsigned int gl_program);
}



#endif