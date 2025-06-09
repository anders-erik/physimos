
#pragma once

#include "math/vecmat.hh"
#include "math/box2D.hh"



namespace scene {



/** Camera for Scene2D : Box2D/AABB wrapper.  */
class Camera2D 
{
    f2 framebuffer_size_scene;
    Box2D box; // Camera size and offset in scene coordinates.

public:

    /** Camera view in scene coordinates */
    Box2D get_box();
    /** Matrix transforms camera box contents to match NDC size.
        e.g. [10, 30]x[10, 25] -> [-1, 1]x[-1, 1] (OpenGL : -> [0, 800]x[0, 600])
    */
    m3f3 get_matrix();
    
    /** Framebuffer size should have an aspect ratio equal to the set gl-viewport. */
    void set_framebuffer_size(f2 size);
    /** Camera width in scene coordinates */
    void set_width(float new_width);
    /** Current camera with in scene coordinates. */
    float get_width();
    /** Aspect ratio of camera view. Should match gl-viewport AR. */
    float AR();

    /** Move camera in opposite direction as delta */
    void pan(f2 delta_scene);

    f2 normalized_to_scene_coords(f2 normalized);
    f2 normalized_to_scene_delta(f2 normalized);

private:

    /** Make sure camera box has same aspect ratio as scene frame buffer */
    void reload_height();

};




}