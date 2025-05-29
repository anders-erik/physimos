
#pragma once


#include "math/vecmat.hh"
#include "math/transform.hh"


namespace scene {


struct Box2D {
    f2 pos;
    f2 size;

    void print();
};


/** Captures [0, 1]x[0, 1/AR]x[-1, 1] by default. */
class Camera2D {

m3f3 M_s_ndc;   // Complete matrix: from scene coordinate to NDC. Constructed by the M's below

m3f3 t_M_s_c;   // translate: scene to camera
m3f3 s_M_c_ndc; // scale: camera to NDC
m3f3 t_M_c_ndc; // translate: camera to NDC


f2 window_size_px;
Box2D box; // Camera size and offset in scene coordinates.

float AR = 1.0f; // Aspect Ratio
float zoom_factor = 1.1f;   // Rate at which scrolling change zoom level

public:

    // Called when change detected on: window_size & camera width.
    // Reload all dependent values and reloads matrix to transform scene->NDC
    void matrix_reload();
    m3f3 get_matrix();
    
    void set_window_size_px(f2 size); // set window size and aspect ratio
    void set_zoom_factor(float factor);
    void set_width(float zoom); // set zoom and reload dims
    
    // Change camera view size by zoom_factor
    void zoom(float delta);
    // Change camera position
    void pan(f2 delta_px);

    Box2D get_box();
    
    void print();
};




}