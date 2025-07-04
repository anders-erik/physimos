#pragma once

#include "opengl/texture_unit.hh"

namespace opengl {

struct OpenGL
{
    TextureUnits texture_units;

    OpenGL();

    void init(void* (*process_fn)(const char *procname));


};


}