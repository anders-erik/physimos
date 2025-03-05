

#include "glad/glad.h"

#include "draw.hh"

#include "draw_shader.hh"


namespace draw {

    TransformContext transform_context;

    DrawState draw_state;

    BitmapTexture_Dynamic* bitmap_texture_dynamic;

    void init(const ViewportContext& viewport_context){

        // draw_shader.set_window_info(viewport_context);
        draw_shader.set_viewport_context(viewport_context);
        draw_shader.init();
        // draw_shader.compile_shader();

        bitmap_texture_dynamic = new BitmapTexture_Dynamic(100, 100);
        // bitmap_texture_dynamic
    }

    void update_window(const ViewportContext& viewport_context){
        // draw_shader.set_window_info(viewport_context);
        draw_shader.set_viewport_context(viewport_context);
    }

    void draw(){
        // glClear(255);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        bitmap_texture_dynamic->updateTransformationMatrix();
        draw_shader.set(bitmap_texture_dynamic->squareTransform, bitmap_texture_dynamic->texture);
        draw_shader.draw();
    }

    void click(ViewportCursor cursor_main_view){
        transform_context.main_view_x = cursor_main_view.x;
        transform_context.main_view_y = cursor_main_view.y;
    }

    bool middle_btn_down(ViewportCursor cursor_main_view){
        draw_state.grabbed_canvas = true;
        return true;
    }
    void middle_btn_up(ViewportCursor cursor_main_view){
        draw_state.grabbed_canvas = false;
    }
    void scroll(double dy){
        transform_context.zoom += (float) dy * 0.1;
        draw_shader.set_transform_context(transform_context);
        draw_shader.update_main_view();
    }


    void cursor_move(ViewportCursor main_view_cursor_pos, ViewportCursor cursor_delta){

        // Use absolute value change to not update any every tiny cursor change
        if(abs(main_view_cursor_pos.x - transform_context.main_view_x) > 2.0){
            transform_context.main_view_x = main_view_cursor_pos.x;
            transform_context.texture_x = transform_context.main_view_x / transform_context.zoom - transform_context.pan_texture_coords_x;
        }
        if(abs(main_view_cursor_pos.y - transform_context.main_view_y) > 2.0){
            transform_context.main_view_y = main_view_cursor_pos.y;
            transform_context.texture_y = transform_context.main_view_y / transform_context.zoom - transform_context.pan_texture_coords_y;
        }

        if(draw_state.grabbed_canvas){
            transform_context.pan_texture_coords_x += cursor_delta.x / transform_context.zoom;
            transform_context.pan_texture_coords_y += cursor_delta.y / transform_context.zoom;
            draw_shader.set_transform_context(transform_context);
        }
    }

    TransformContext& ui_get_transform_context(){
        return transform_context;
    }


BitmapTexture_Dynamic::BitmapTexture_Dynamic(uint width, uint height) 
    :   bitmap { pimage::Bitmap(width , height) }
{
    // SET BITMAP COLOR TO ALL-BLACK
    bitmap.replace_color({0,0,0,0}, {0,0,0, 255});

    aabb.w = width;
    aabb.h = height;
    aabb.x = 0;
    aabb.y = 0;


    id = "bitmap_texture_dynamic";

    texture = 0;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texture);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.width, bitmap.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.pixels.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    // return newTexture;


}

// make sure the transform matrix is updated to current height, width, x, and y
void BitmapTexture_Dynamic::updateTransformationMatrix() {

    squareTransform[0] = aabb.w;
    squareTransform[5] = aabb.h;

    squareTransform[3] = aabb.x;
    squareTransform[7] = aabb.y;

}




}
