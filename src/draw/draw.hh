
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
    float texture_x = 0.0f; /** texture coord - exact */
    float texture_y = 0.0f; /** texture coord - exact */
    int texture_px_x = 0; /** texture coord rounded - actual pixel we draw to */
    int texture_px_y = 0; /** texture coord rounded - actual pixel we draw to */
} TransformContext;

typedef struct Brush {
    int size = 3;
    pimage::Pixel color = pimage::pixel_color_green;
} Brush;

/** Draw information for transforming texture into main_view */
typedef struct DrawState {
    /** Is set if canvas panning is currently enabled, usually when middle mouse button is held down */
    bool pan_canvas = false;
    /** Flag that is set if currently drawing. Usually enabled when left mouse button held down triggering bitmap draw call on mouse movement. */
    bool drawing = false;
} DrawState;



typedef struct AABB_FLOAT_2D {
    float w;
    float h;
    float x; 
    float y;
} AABB_FLOAT_2D;

void init(const ViewportContext& viewport_context);

void update_window(const ViewportContext& viewport_context);

void set_cursor_main_view(ViewportCursor cursor_main_view);

/** Returns true if cursor is grabbed */
bool left_btn_down();
void left_btn_up();
/** Returns true if cursor is grabbed */
bool middle_btn_down();
void middle_btn_up();
void scroll(double dy);
void cursor_move(ViewportCursor cursor_delta);

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

    void draw(int x, int y);

    /** Reload the texture with current bitmap data. */
    void reload_texture();

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