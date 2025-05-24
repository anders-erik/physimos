
#pragma once


#include "math/vecmat.hh"
#include "math/transform.hh"


namespace scene2D {


struct Box2D {
    f2 pos;
    f2 size;

    void print();
};


/** Captures [-1, 1]x[-1,1]x[-1, -1] by default. */
class Camera2D {

float width_initial = 2.0f;

public:
    Box2D box; // Camera size and offset in scene coordinates.
    float units_per_px = 0.0f; // Number of scene units per pixel.
    Transform2D transform; // Used to generate a 3x3 view matrix
    f2 window_size_px;              //
    float aspect_ratio = 1.0f;      // 
    float zoom_current = 1.0f;      // 
    float zoom_multiplier = 1.1f;   // Rate at which scrolling change zoom level


    // box, units per pixel, transform, and reload transform.
    // After reloading the camera is ready to be used.
    void reload(); 
    
    void set_window_size_px(f2 size); // set window size and aspect ratio

    void set_zoom_multiplier(float multiplier);
    void zoom_set(float zoom); // set zoom and reload dims
    void zoom(float delta); // zoom fixed amount. Delta sign indicate direction.

    void pan(f2 delta_px);
    

};




}