
#pragma once

#include <array>

#include "math/vecmat.hh"
#include "math/transform.hh"
#include "math/shape.hh"

#include "opengl/program.hh"

#include "render3D/program_vector.hh"
#include "render3D/program_axes.hh"

#include "render3D/program_model_texture.hh"
#include "render3D/program_model_color.hh"


namespace opengl {
    class TextureFB;
    class TextureFBMS;
}





class RendererScene3D {

    ProgramAxes program_axes;

    ProgramVector program_vector;
    
    ProgramModelTexture program_model_texture;
    ProgramModelColor program_model_color;

public:

    RendererScene3D();

    void render_scene_3d(SceneBase& scene);

};


