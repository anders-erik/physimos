
#pragma once

#include <iostream>

#include "opengl/error.hh"

#include "glad/glad.h"

namespace opengl {


void error_check(){

    GLenum err;
    // while ((err = glGetError()) != GL_NO_ERROR) {
    //     std::cerr << "OpenGL error: " << err << std::endl;
    // }

    if( ((err = glGetError()) == GL_NO_ERROR) )
        std::cerr << "OpenGL: NO error: " << err << std::endl;
    else
        std::cerr << "OpenGL: ERROR: " << err << std::endl;

}
}