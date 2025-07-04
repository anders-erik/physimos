#include <iostream>

#include "glad/glad.h"

#include "opengl/texture.hh"
#include "opengl/texture_unit.hh"

#include "opengl/opengl.hh"

opengl::
OpenGL::
OpenGL()
{

}


void opengl::OpenGL::
init(void* (*process_fn)(const char *procname))
{

    if (!gladLoadGLLoader((GLADloadproc)process_fn))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

    // OpenGL Fatures
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    opengl::textures_init();

    texture_units.init();


    // GLint maxTextureSize;
    // glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    // printf("Max texture size: %d\n", maxTextureSize);

    // int maxUnits;
    // glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnits);
    // printf("Max texture size: %d\n", maxUnits);
}