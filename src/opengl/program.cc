#include "glad/glad.h"

#include "lib/log.hh"
#include "lib/fs.hh"
#include "lib/process.hh"

#include "program.hh"

namespace opengl {

unsigned int program_model;
unsigned int program_axes;


void gpu_use_program(Programs program_enum){

    switch (program_enum){

        case Programs::Model:
            glUseProgram(program_model);
            break;
        
        case Programs::Axes:
            glUseProgram(program_axes);
            break;
        
        default:
            break;
    }
    
}

unsigned int gpu_get_program(Programs program_enum){
    unsigned int program;

    switch (program_enum){

        case Programs::Model:
            program = program_model;
            break;

        case Programs::Axes:
            program = program_axes;
            break;
        
        default:
            break;
    }

    return program;
}

unsigned int build_program_vert_frag(Programs phont_texture){

    unsigned int new_program;

    std::string physimos_root_dir = physimos_root_dir_or_die();

    std::string vert_str;
    std::string frag_str;

    switch (phont_texture)
    {

    case Programs::phont_texture :
        vert_str = physimos_root_dir + "/src/phont/shaders/phont_texture_vert.glsl";
        frag_str = physimos_root_dir + "/src/phont/shaders/phont_texture_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        break;

    case Programs::phont_char :
        vert_str = physimos_root_dir + "/src/phont/shaders/phont_char_vert.glsl";
        frag_str = physimos_root_dir + "/src/phont/shaders/phont_char_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        break;

    case Programs::Model :
        vert_str = physimos_root_dir + "/src/model/shaders/model_editor_vert.glsl";
        frag_str = physimos_root_dir + "/src/model/shaders/model_editor_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        program_model = new_program;
        break;

    case Programs::Axes :
        vert_str = physimos_root_dir + "/src/model/shaders/axes_vert.glsl";
        frag_str = physimos_root_dir + "/src/model/shaders/axes_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        program_axes = new_program;
        break;
    
    default:
        break;
    }


    return new_program;
}

unsigned int build_program_vert_frag(std::string vert_path, std::string frag_path){

    unsigned int shader_id;

    // READ FILE
    std::string vertFileString = plib::fs_cat(vert_path);
    std::string fragFileString = plib::fs_cat(frag_path);

    const char* vertFile_cstr = vertFileString.c_str();
    const char* fragFile_cstr = fragFileString.c_str();



    // COMPILE 
    unsigned int vert_shader, frag_shader;

    // Vertex shader
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vertFile_cstr, NULL);
    glCompileShader(vert_shader);
    shader_error_check(vert_shader, "VERTEX", vert_path);

    // Fragment Shader
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &fragFile_cstr, NULL);
    glCompileShader(frag_shader);
    shader_error_check(frag_shader, "FRAGMENT", frag_path);

    // shader Program
    shader_id = glCreateProgram(); //Creates a program object
    glAttachShader(shader_id, vert_shader);
    glAttachShader(shader_id, frag_shader);
    glLinkProgram(shader_id);
    program_error_check(shader_id);

    // Individual shaders are built and linked.
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    return shader_id;
}


void shader_error_check(unsigned int gl_shader, std::string shader_type, std::string shader_path){

    int success;
    char infoLog[1024];

    glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(gl_shader, 1024, NULL, infoLog);
        plib::plog_error("DRAW ", "SHADER_" + shader_type + " ", "Failed to build DrawShader shader[" + shader_path + "]. : InfoLog = " + std::string(infoLog));
    }

}

void program_error_check(unsigned int gl_program){

    int success;
    char infoLog[1024];

    glGetProgramiv(gl_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(gl_program, 1024, NULL, infoLog);
        plib::plog_error("DRAW ", "SHADER_PROGRAM ", "Failed to link DrawShader program. : InfoLog = " + std::string(infoLog));
    }

}


}