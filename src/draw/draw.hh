
#ifndef DRAW_HH
#define DRAW_HH

#include <string>

#include "Windowing.hpp"

#include "conductor_viewport.hh"

#include "image/bitmap.hh"

namespace draw {

/** Draw information for transforming texture into main_view */
typedef struct TransformContext {
    float zoom = 1.0f;
    float pan_texture_coords_x = 0.0f;
    float pan_texture_coords_y = 0.0f;
    float main_view_x = 0.0f;
    float main_view_y = 0.0f;
    float texture_x = 0.0f;
    float texture_y = 0.0f;
} TransformContext;

/** Draw information for transforming texture into main_view */
typedef struct DrawState {
    bool grabbed_canvas = false;
} DrawState;



typedef struct AABB_FLOAT_2D {
    float w;
    float h;
    float x; 
    float y;
} AABB_FLOAT_2D;

void init(const ViewportContext& viewport_context);

void update_window(const ViewportContext& viewport_context);

void click(ViewportCursor cursor_main_view);

/** Returns true if cursor is grabbed */
bool middle_btn_down(ViewportCursor cursor_main_view);
void middle_btn_up(ViewportCursor cursor_main_view);
void scroll(double dy);
void cursor_move(ViewportCursor main_view_cursor_pos, ViewportCursor cursor_delta);

void draw();

TransformContext& ui_get_transform_context();

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

    float squareTransform[16] = {   1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    0, 0, 0, 1, };

    BitmapTexture_Dynamic(uint width, uint height);
} BitmapTexture_Dynamic;



}

#endif 