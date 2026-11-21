
#pragma once

#include "lib/print.hh"
#include "opengl/loader.hh"

namespace OpenGL
{

void print_opengl_error()
{
    GLenum error_enum = glGetError();
    if(error_enum == GL_NO_ERROR)
        return;

    Print::buf("OPENGL ERROR: ");
    Str error_str;

    switch (error_enum)
    {
    case GL_INVALID_ENUM: error_str = "GL_INVALID_ENUM"; break;
    case GL_INVALID_VALUE: error_str = "GL_INVALID_VALUE"; break;
    case GL_INVALID_OPERATION: error_str = "GL_INVALID_OPERATION"; break;
    case GL_INVALID_FRAMEBUFFER_OPERATION: error_str = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
    case GL_OUT_OF_MEMORY: error_str = "GL_OUT_OF_MEMORY"; break;
    case GL_STACK_UNDERFLOW: error_str = "GL_STACK_UNDERFLOW"; break;
    case GL_STACK_OVERFLOW: error_str = "GL_STACK_OVERFLOW"; break;
    
    default:
        error_str = "UNKNOWN";
        break;
    }

    Print::ln(error_str);
}


void set_viewport_size(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);
}



};