
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "scene/render/renderer2D.hh"
#include "opengl/texture.hh"

#include "window/auxwin.hh"

#include "math/vecmat.hh"
#include "math/shape.hh"

#include "scene/scene2D.hh"
#include "scene/camera2D.hh"
#include "scene/shapeS2D.hh"
#include "scene/quadS2D.hh"

#include "scene/manager.hh"

#include "phont/phont.hh"

struct GLFWWindow;



namespace scene {

struct QuadS2D;

/** 
    A Scene2D and QuadS2D pair.
    Primarily resitricted interface using indrection to the scene and quad.
*/
class SubScene2D {




public:
    /** The quad rendered by parent scene using the texture from accompanying scene. */
    QuadS2D quad;
    
    /** Unique object id from manager. A value of 0 means that it has no valid object. */
    size_t scene_id;


};


}