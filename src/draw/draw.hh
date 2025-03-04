
#ifndef DRAW_HH
#define DRAW_HH

#include <string>

#include "Windowing.hpp"

#include "conductor_viewport.hh"

#include "image/bitmap.hh"

namespace draw {

    typedef struct AABB_FLOAT_2D {
        float w;
        float h;
        float x; 
        float y;
    } AABB_FLOAT_2D;

    void init(const ViewportContext& viewport_context);

    void update_window(const ViewportContext& viewport_context);

    void draw();

/** A bitmap that automatically reloads a bound glTexture when updated. */
typedef struct BitmapTexture_Dynamic {
    std::string id;

    /** Bitmap storage in ram */
    ::pimage::Bitmap bitmap;
    
    /** Bitmap storage in VRAM as texture */
    uint texture;

    /**  */
    AABB_FLOAT_2D aabb;

    /** Set the 4x4 transformation matrix that will be set as shader uniform */
    void updateTransformationMatrix();

    float squareTransform[16] = {  1, 0, 0, 0,
                                            0, 1, 0, 0,
                                            0, 0, 1, 0,
                                            0, 0, 0, 1, };

    BitmapTexture_Dynamic(uint width, uint height);
} BitmapTexture_Dynamic;





}

#endif 