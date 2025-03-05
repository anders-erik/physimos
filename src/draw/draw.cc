#include <cmath>

#include "glad/glad.h"

#include "draw.hh"

#include "draw_shader.hh"


namespace draw {

    TransformContext transform_context;

    DrawState draw_state;

    BitmapTexture_Dynamic* bitmap_texture_dynamic;

    Brush brush_current;

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

    bool left_btn_down(){
        bitmap_texture_dynamic->draw(transform_context.texture_px_x, transform_context.texture_px_y);
        draw_state.drawing = true;
        return true;
    }
    void left_btn_up(){
        draw_state.drawing = false;
    }
    bool middle_btn_down(){
        draw_state.pan_canvas = true;
        return true;
    }
    void middle_btn_up(){
        draw_state.pan_canvas = false;
    }

    void scroll(double dy){
        // transform_context.zoom += (float) dy * 0.3;

        // Adjust the pan to keep the same part of texture in view
        if(dy > 0){

            if(transform_context.zoom > 80)
                return;

            transform_context.zoom *= 1.25;

            // transform_context.pan_texture_coords_x += 

            transform_context.pan_texture_coords_x *= 0.8;
            transform_context.pan_texture_coords_x -= bitmap_texture_dynamic->bitmap.width / (transform_context.zoom * 2);

            transform_context.pan_texture_coords_y *= 0.8;
            transform_context.pan_texture_coords_y -= bitmap_texture_dynamic->bitmap.height / (transform_context.zoom * 2);
        }
        else {

            if(transform_context.zoom < 0.125)
                return;

            transform_context.zoom *= 0.8;
            // transform_context.pan_texture_coords_x += transform_context.pan_texture_coords_x*0.3;
            transform_context.pan_texture_coords_x *= 1.25;
            transform_context.pan_texture_coords_x += bitmap_texture_dynamic->bitmap.width / (transform_context.zoom * 2);

            transform_context.pan_texture_coords_y *= 1.25;
            transform_context.pan_texture_coords_y += bitmap_texture_dynamic->bitmap.height / (transform_context.zoom * 2);
        }

        draw_shader.set_transform_context(transform_context);
        draw_shader.update_main_view();
    }

    void set_cursor_main_view(ViewportCursor cursor_main_view){
        transform_context.main_view_x = cursor_main_view.x;
        transform_context.main_view_y = cursor_main_view.y;
    }

    void cursor_move(ViewportCursor cursor_delta){

        transform_context.texture_x = transform_context.main_view_x / transform_context.zoom - transform_context.pan_texture_coords_x;
        transform_context.texture_y = transform_context.main_view_y / transform_context.zoom - transform_context.pan_texture_coords_y;

        transform_context.texture_px_x = (int) std::floor(transform_context.texture_x);
        transform_context.texture_px_y = (int) std::floor(transform_context.texture_y);

        // // Use absolute value change to not update any every tiny cursor change
        // if(abs(transform_context.main_view_x + cursor_delta.x) > 2.0){
        //     transform_context.main_view_x = main_view_cursor_pos.x;
        //     transform_context.texture_x = transform_context.main_view_x / transform_context.zoom - transform_context.pan_texture_coords_x;
        // }
        // if(abs(main_view_cursor_pos.y - transform_context.main_view_y) > 2.0){
        //     transform_context.main_view_y = main_view_cursor_pos.y;
        //     transform_context.texture_y = transform_context.main_view_y / transform_context.zoom - transform_context.pan_texture_coords_y;
        // }

        if(draw_state.pan_canvas){
            transform_context.pan_texture_coords_x += cursor_delta.x / transform_context.zoom;
            transform_context.pan_texture_coords_y += cursor_delta.y / transform_context.zoom;
            draw_shader.set_transform_context(transform_context);
        }

        if(draw_state.drawing){
            bitmap_texture_dynamic->draw(transform_context.texture_px_x, transform_context.texture_px_y);
        }
    }

    TransformContext& ui_get_transform_context(){
        return transform_context;
    }

    Brush& ui_get_brush(){
        return brush_current;
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

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, texture);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.width, bitmap.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.pixels.data());
    glGenerateMipmap(GL_TEXTURE_2D);


    // bitmap.set_pixel(0, 0, {255, 255, 255, 255});

    reload_texture();

}

void BitmapTexture_Dynamic::draw(int x_pointer, int y_pointer){

    // NEVER DRAW WHEN NOT CLICKING ON IMAGE
    if(x_pointer < 0 || y_pointer < 0){
        return;
    }
    // OK to compare to unsigned for now as we clear negative cases previously
    if(x_pointer > bitmap.width-1 || y_pointer > bitmap.height-1){
        return;
    }

    
    bitmap.set_square(x_pointer, y_pointer, brush_current.color, brush_current.size);

    reload_texture();
}

void BitmapTexture_Dynamic::reload_texture(){

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexSubImage2D(    GL_TEXTURE_2D, 0 , // identical to glTexImage2D
                        0, 0, // x, y
                        bitmap.width, bitmap.height, 
                        GL_RGBA, GL_UNSIGNED_BYTE, bitmap.pixels.data()); // identical to glTexImage2D

}

// make sure the transform matrix is updated to current height, width, x, and y
void BitmapTexture_Dynamic::updateTransformationMatrix() {

    squareTransform[0] = aabb.w;
    squareTransform[5] = aabb.h;

    squareTransform[3] = aabb.x;
    squareTransform[7] = aabb.y;

}




}
