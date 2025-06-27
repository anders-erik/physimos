#include "glad/glad.h"

#include "lib/str_std.hh"
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

unsigned int program_ui_color;
unsigned int program_ui_string;
unsigned int program_ui_texture;

// ProgramPaths ui_color_ = {"ui/color"};


ShaderProgram::ShaderProgram(Str program_name) 
    :   program_name_str {program_name}
{

    shader_vert.set_shader_type(Shader::Type::Vertex);
    shader_frag.set_shader_type(Shader::Type::Fragment);

    shader_vert.set_program_name(program_name_str);
    shader_frag.set_program_name(program_name_str);

}


unsigned int ShaderProgram::build(){
    
    shader_vert.compile();
    shader_frag.compile();

    link();

    shader_vert.delete_shader_id();
    shader_frag.delete_shader_id();
    
    return id;
}


unsigned int ShaderProgram::link(){
    
    // shader Program
    id = glCreateProgram(); //Creates a program object
    glAttachShader(id, shader_vert.get_shader_id());
    glAttachShader(id, shader_frag.get_shader_id());
    glLinkProgram(id);
    check_link_error();

    
    return id;
}


void ShaderProgram::check_link_error(){

    int success;
    char infoLog[1024];

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 1024, NULL, infoLog);
        plib::plog_error("OPENGL ", "SHADER_PROGRAM ", "Failed to link program. : InfoLog = " + std::string(infoLog));
    }
    else {
        std::cout << StrStd::to_string(program_name_str) << " linked succesfully." << std::endl;
    }
}


unsigned int ShaderProgram::get_program_id(){
    return id;
}


void ShaderProgram::use(){
    glUseProgram(id);
}






// OLD BELOW





void gpu_use_program(ProgramName program_enum){

    switch (program_enum){

        case ProgramName::ModelColor:
            glUseProgram(program_model_color);
            break;
        
        case ProgramName::ModelTexture:
            glUseProgram(program_model_texture);
            break;
        
        case ProgramName::Quad2D:
            glUseProgram(program_texture_2d);
            break;
        
        case ProgramName::ndc_black:
            glUseProgram(program_ndc_black);
            break;

        case ProgramName::Axes:
            glUseProgram(program_axes);
            break;

        case ProgramName::Vector:
            glUseProgram(program_vector);
            break;

        case ProgramName::ui_color:
            glUseProgram(program_ui_color);
            break;
        case ProgramName::ui_string:
            glUseProgram(program_ui_string);
            break;
        case ProgramName::ui_texture:
            glUseProgram(program_ui_texture);
            break;
        
        default:
            break;
    }
    
}

unsigned int gpu_get_program(ProgramName program_enum){
    unsigned int program;

    switch (program_enum){

        case ProgramName::ModelColor:
            program = program_model_color;
            break;
        
        case ProgramName::ModelTexture:
            program = program_model_texture;
            break;

        case ProgramName::Quad2D:
            program = program_texture_2d;
            break;

        case ProgramName::Axes:
            program = program_axes;
            break;

        case ProgramName::Vector:
            program = program_vector;
            break;

        case ProgramName::ui_color:
            program = program_ui_color;
            break;
        case ProgramName::ui_string:
            program = program_ui_string;
            break;
        case ProgramName::ui_texture:
            program = program_ui_texture;
            break;
        
        default:
            break;
    }

    return program;
}

unsigned int build_program_vert_frag(ProgramName program_enum){

    unsigned int new_program;

    std::string physimos_root_dir = physimos_root_dir_or_die();

    std::string vert_str;
    std::string frag_str;

    switch (program_enum)
    {

    case ProgramName::phont_texture :
        vert_str = physimos_root_dir + "/src/phont/shaders/phont_texture_vert.glsl";
        frag_str = physimos_root_dir + "/src/phont/shaders/phont_texture_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        break;

    case ProgramName::phont_char :
        vert_str = physimos_root_dir + "/src/phont/shaders/phont_char_vert.glsl";
        frag_str = physimos_root_dir + "/src/phont/shaders/phont_char_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        break;

    case ProgramName::Quad2D :
        vert_str = physimos_root_dir + "/src/opengl/shaders/2D/quad.vert";
        frag_str = physimos_root_dir + "/src/opengl/shaders/2D/quad.frag";
        new_program = build_program_vert_frag(vert_str, frag_str);
        break;

    case ProgramName::ndc_texture :
        vert_str = physimos_root_dir + "/src/opengl/shaders/ndc/texture_vert.glsl";
        frag_str = physimos_root_dir + "/src/opengl/shaders/ndc/texture_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        break;

    case ProgramName::ndc_black :
        vert_str = physimos_root_dir + "/src/opengl/shaders/ndc/vert.glsl";
        frag_str = physimos_root_dir + "/src/opengl/shaders/ndc/black_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        program_ndc_black = new_program;
        break;

    case ProgramName::ndc_white :
        vert_str = physimos_root_dir + "/src/opengl/shaders/ndc/vert.glsl";
        frag_str = physimos_root_dir + "/src/opengl/shaders/ndc/white_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        break;

    case ProgramName::ModelColor :
        vert_str = physimos_root_dir + "/src/opengl/shaders/model_editor_vert.glsl";
        frag_str = physimos_root_dir + "/src/opengl/shaders/model_editor_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        program_model_color = new_program;
        break;

    case ProgramName::ModelTexture :
        vert_str = physimos_root_dir + "/src/opengl/shaders/model_texture_vert.glsl";
        frag_str = physimos_root_dir + "/src/opengl/shaders/model_texture_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        program_model_texture = new_program;
        break;

    case ProgramName::Axes :
        vert_str = physimos_root_dir + "/src/opengl/shaders/axes_vert.glsl";
        frag_str = physimos_root_dir + "/src/opengl/shaders/axes_frag.glsl";
        new_program = build_program_vert_frag(vert_str, frag_str);
        program_axes = new_program;
        break;

    case ProgramName::Vector :
        vert_str = physimos_root_dir + "/src/opengl/shaders/vector.vert";
        frag_str = physimos_root_dir + "/src/opengl/shaders/vector.frag";
        new_program = build_program_vert_frag(vert_str, frag_str);
        program_vector = new_program;
        break;


    case ProgramName::ui_color :
        vert_str = physimos_root_dir + "/src/opengl/shaders/ui/color.vert";
        frag_str = physimos_root_dir + "/src/opengl/shaders/ui/color.frag";
        new_program = build_program_vert_frag(vert_str, frag_str);
        program_ui_color = new_program;
        break;
    case ProgramName::ui_string :
        vert_str = physimos_root_dir + "/src/opengl/shaders/ui/color.vert";
        frag_str = physimos_root_dir + "/src/opengl/shaders/ui/color.frag";
        new_program = build_program_vert_frag(vert_str, frag_str);
        program_ui_string = new_program;
        break;
    case ProgramName::ui_texture :
        vert_str = physimos_root_dir + "/src/opengl/shaders/ui/texture.vert";
        frag_str = physimos_root_dir + "/src/opengl/shaders/ui/texture.frag";
        new_program = build_program_vert_frag(vert_str, frag_str);
        program_ui_texture = new_program;
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