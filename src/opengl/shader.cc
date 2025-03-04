#include "glad/glad.h"

#include "lib/log.hh"
#include "lib/fs.hh"

#include "shader.hh"

namespace opengl {

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