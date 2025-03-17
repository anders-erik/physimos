
#include "glad/glad.h"

#include <iostream>
#include <string>

#include "fs.hh"

#include "gpu.hh"

namespace xprubik {

unsigned int program_model;
unsigned int program_axes;


bool gpu_init(){

    bool build_ok = false;

    build_ok = gpu_build_vert_frag_program(Shader::Model);
    if(!build_ok) return build_ok;

    build_ok = gpu_build_vert_frag_program(Shader::Axes);
    if(!build_ok) return build_ok;


    return true;
}



void gpu_draw_triangle_moodel_shader(){
    gpu_use_program(Shader::Model);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

bool gpu_shader_compile_check(unsigned int shader){
    int shader_success;
    char infolog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_success);
    if(!shader_success){
        std::cout << "ERROR COMPILING VERT SHADER." << std::endl;

        glGetShaderInfoLog(shader, 512, NULL, infolog);
        std::cout << infolog << std::endl;
    }
    return shader_success;
}

bool gpu_shader_link_check(unsigned int program){
    int shader_success;
    char infolog[512];

    glGetProgramiv(program, GL_LINK_STATUS, &shader_success);
    if(!shader_success){
        std::cout << "ERROR LINKING SHADER PROGRAM." << std::endl;

        glGetProgramInfoLog(program, 512, NULL, infolog);
        std::cout << infolog << std::endl;
    }
    return shader_success;
}

unsigned int gpu_build_vert_frag_program(Shader shader_enum){

    std::string vert;
    std::string frag;

    switch (shader_enum)
    {
        case Shader::Model:
            vert = cat("shaders/model_vert.glsl");
            frag = cat("shaders/model_frag.glsl");
            break;

        case Shader::Axes:
            vert = cat("shaders/axes_vert.glsl");
            frag = cat("shaders/axes_frag.glsl");
            break;
        
        default:
            break;
    }


    const char* vert_c = vert.c_str();
    const char* frag_c = frag.c_str();
    
    
    bool build_ok = false;

    unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_c, NULL);
    glCompileShader(vert_shader);
    build_ok = gpu_shader_compile_check(vert_shader);
    if(!build_ok) return build_ok;

    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_c, NULL);
    glCompileShader(frag_shader);
    build_ok = gpu_shader_compile_check(frag_shader);
    if(!build_ok) return build_ok;

    unsigned int program;

    switch (shader_enum)
    {
        case Shader::Model:
            program_model = glCreateProgram();
            program = program_model;
            break;

        case Shader::Axes:
            program_axes = glCreateProgram();
            program = program_axes;
            break;
        
        default:
            break;
    }


    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);
    build_ok = gpu_shader_link_check(program);
    if(!build_ok) return build_ok;


    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    return build_ok;
}

void gpu_use_program(Shader shader_enum){

    switch (shader_enum){

        case Shader::Model:
            glUseProgram(program_model);
            break;
        
        case Shader::Axes:
            glUseProgram(program_axes);
            break;
        
        default:
            break;
    }
    
}

unsigned int gpu_get_program(Shader shader_enum){
    unsigned int program;

    switch (shader_enum){

        case Shader::Model:
            program = program_model;
            break;

        case Shader::Axes:
            program = program_axes;
            break;
        
        default:
            break;
    }

    return program;
}


}