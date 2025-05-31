#ifndef OPENGL_SHADER_HH
#define OPENGL_SHADER_HH

#include <string>

#include <lib/str.hh>

#include "shader.hh"


namespace opengl {


enum class ProgramName {
    phont_texture,
    phont_char,

    ndc_texture,
    ndc_black,
    ndc_white,

    Texture2D,

    ui_color,
    ui_string,
    ui_texture,

    ModelColor,
    ModelTexture,
    Axes,
    Vector,
};




class Program {

    Str program_name_str;
    
    Shader shader_vert;
    Shader shader_frag;

protected:


public:
    unsigned int id;

    // The program attempts to compile shaders and link program during construction.
    Program(Str program_name);

    unsigned int build();
    unsigned int link();
    void check_link_error();

    unsigned int get_program_id();
    void use();

};



unsigned int    gpu_get_program(ProgramName program_enum);
void            gpu_use_program(ProgramName program_enum);


/** Compile single vertex and fragment shader, links program, and returns program name. */
unsigned int build_program_vert_frag(ProgramName program_enum);



/** Compile single vertex and fragment shader, links program, and returns program name. */
unsigned int build_program_vert_frag(std::string vert_path, std::string frag_path);





/** INTERNAL */

void shader_error_check(unsigned int gl_shader, std::string shader_type, std::string shader_path);

void program_error_check(unsigned int gl_program);


}



#endif