#include "glad/glad.h"

#include "lib/log.hh"
#include "lib/fs.hh"
#include "lib/process.hh"

#include "program.hh"

namespace opengl {

unsigned int program_model_color;
unsigned int program_model_texture;
unsigned int program_axes;
unsigned int program_vector;
unsigned int program_texture_2d;
unsigned int program_ndc_black;



void gpu_use_program(Programs program_enum){

    switch (program_enum){

        case Programs::ModelColor:
            glUseProgram(program_model_color);
            break;
        
        case Programs::ModelTexture:
            glUseProgram(program_model_texture);
            break;
        
        case Programs::Texture2D:
            glUseProgram(program_texture_2d);
            break;
        
        case Programs::ndc_black:
            glUseProgram(program_ndc_black);
            break;

        case Programs::Axes:
            glUseProgram(program_axes);
            break;

        case Programs::Vector:
            glUseProgram(program_vector);
            break;
        
        default:
            break;
    }
    
}

unsigned int gpu_get_program(Programs program_enum){
    unsigned int program;

    switch (program_enum){

        case Programs::ModelColor:
            program = program_model_color;
            break;
        
        case Programs::ModelTexture:
            program = program_model_texture;
            break;

        case Programs::Texture2D:
            program = program_texture_2d;
            break;

        case Programs::Axes:
            program = program_axes;
            break;

        case Programs::Vector:
            program = program_vector;
            break;
        
        default:
            break;
    }

    return program;
}

unsigned int build_program_vert_frag(Programs program_enum){

    unsigned int new_program;

    std::string physimos_root_dir = physimos_root_dir_or_die();

    std::string vert_str;
    std::string frag_str;

    switch (program_enum)
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

    case Programs::Texture2D :
        vert_str = physimos_root_dir + "/src/opengl/shaders/2d/texture_vert.glsl";
        frag_str = physimos_root_dir + "/src/opengl/shaders/2d/texture_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        break;

    case Programs::ndc_texture :
        vert_str = physimos_root_dir + "/src/opengl/shaders/ndc/texture_vert.glsl";
        frag_str = physimos_root_dir + "/src/opengl/shaders/ndc/texture_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        break;

    case Programs::ndc_black :
        vert_str = physimos_root_dir + "/src/opengl/shaders/ndc/vert.glsl";
        frag_str = physimos_root_dir + "/src/opengl/shaders/ndc/black_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        program_ndc_black = new_program;
        break;

    case Programs::ndc_white :
        vert_str = physimos_root_dir + "/src/opengl/shaders/ndc/vert.glsl";
        frag_str = physimos_root_dir + "/src/opengl/shaders/ndc/white_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        break;

    case Programs::ModelColor :
        vert_str = physimos_root_dir + "/src/opengl/shaders/model_editor_vert.glsl";
        frag_str = physimos_root_dir + "/src/opengl/shaders/model_editor_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        program_model_color = new_program;
        break;

    case Programs::ModelTexture :
        vert_str = physimos_root_dir + "/src/opengl/shaders/model_texture_vert.glsl";
        frag_str = physimos_root_dir + "/src/opengl/shaders/model_texture_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        program_model_texture = new_program;
        break;

    case Programs::Axes :
        vert_str = physimos_root_dir + "/src/opengl/shaders/axes_vert.glsl";
        frag_str = physimos_root_dir + "/src/opengl/shaders/axes_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        program_axes = new_program;
        break;

    case Programs::Vector :
        vert_str = physimos_root_dir + "/src/opengl/shaders/vector.vert";
        frag_str = physimos_root_dir + "/src/opengl/shaders/vector.frag";
        new_program = build_program_vert_frag(vert_str, frag_str);
        program_vector = new_program;
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