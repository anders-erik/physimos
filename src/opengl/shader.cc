
#include "glad/glad.h"

#include "lib/str.hh"
#include "lib/log.hh"

#include "shader.hh"

namespace opengl {


Shader::Shader(Str program_name){

    program_name_str = program_name;
    assemble_core_path();

}


void Shader::set_shader_type(Type type){
    this->type = type;
}

void Shader::set_program_name(Str program_name){
    program_name_str = program_name;
    assemble_core_path();
}

void Shader::assemble_core_path(){

    path_core_full = "";
    path_core_full += shaders_dir_path;
    path_core_full += program_name_str;

    if(type == Type::Fragment)
        path_core_full += ".frag";
    if(type == Type::Vertex)
        path_core_full += ".vert";
    
}

unsigned int Shader::compile(){

    // Update file
    file.set_path_core(path_core_full);

    Str source = file.cat_as_str_core();
    // source.print();

    const char* source_c = source.to_c_str();

    if(type == Type::Vertex)
        shader_id = glCreateShader(GL_VERTEX_SHADER);
    if(type == Type::Fragment)
        shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(shader_id, 1, &source_c, NULL);
    glCompileShader(shader_id);
    check_error();


    return shader_id;
}

unsigned int Shader::get_shader_id(){
    return shader_id;
}

void Shader::delete_shader_id(){
    glDeleteShader(shader_id);
}

void Shader::check_error(){

    int success_flag;
    char infoLog[1024];

    std::string shader_type;
    std::string path_full_string = path_core_full.to_std_string();

    if(type == Type::Fragment)
        shader_type = "FRAGMENT";
    if(type == Type::Vertex)
        shader_type = "VERTEX";


    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success_flag);

    if (!success_flag) {
        glGetShaderInfoLog(shader_id, 1024, NULL, infoLog);
        plib::plog_error("DRAW ", "SHADER_" + shader_type + " ", "Failed to build DrawShader shader[" + path_full_string + "]. : InfoLog = " + std::string(infoLog));
    }
}



}

