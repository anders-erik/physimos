#ifndef OPENGL_SHADER_HH
#define OPENGL_SHADER_HH

#include <string>


namespace opengl {


    /** Compile single vertex and fragment shader, links program, and returns program name. */
    unsigned int build_program_vert_frag(std::string vert_path, std::string frag_path);


    /** INTERNAL */

    void shader_error_check(unsigned int gl_shader, std::string shader_type, std::string shader_path);

    void program_error_check(unsigned int gl_program);
}



#endif