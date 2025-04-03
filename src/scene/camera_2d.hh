
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
struct Camera2D {

    /** Camera size and offset in scene coordinates. */
    Box2D box;

    /** Number of scene units per pixel. */
    float units_per_px = 0.0f;

    Transform2D transform;

    void reload_dims();
    
    f2 window_size_px;
    float aspect_ratio = 1.0f;
    void set_window_size_px(f2 size);

    float zoom_current = 1.0f;
    float zoom_multiplier = 1.1f;
    void set_zoom_multiplier(float multiplier);
    void zoom_set(float zoom);
    void zoom_in();
    void zoom_out();

    void pan(f2 delta_px);

private:
    float width_initial = 2.0f;

};




}