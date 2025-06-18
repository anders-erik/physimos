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
        draw_shader.update_main_view();
    }

    void draw(){
        // glClear(255);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        bitmap_texture_dynamic->updateTransformationMatrix();
        draw_shader.set(bitmap_texture_dynamic->squareTransform, bitmap_texture_dynamic->texture);
        draw_shader.draw();
    }

    bool left_btn_down(){
        bitmap_texture_dynamic->draw(transform_context.cursor_pos_scene_px.x, transform_context.cursor_pos_scene_px.y);
        draw_state.drawing = true;
        return true;
    }
    void left_btn_up(){
        if(draw_state.drawing){
            draw_state.drawing = false;
            bitmap_texture_dynamic->transaction_new();
        }
    }
    void mouse_backward(){
        bitmap_texture_dynamic->transaction_undo();
    }
    void mouse_forward(){
        bitmap_texture_dynamic->transaction_redo();
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

        float text_coord_x_before_zoom = transform_context.cursor_pos_scene.x;
        float text_coord_y_before_zoom = transform_context.cursor_pos_scene.y;

        // Adjust the pan to keep the same texture coordinate cursor target
        if(dy > 0){

            if(transform_context.zoom > 80)
                return;

            transform_context.zoom *= 1.25;

            // Pan to match the text coord target with old zoom value
            transform_context.pan_texture_pos.x = transform_context.cursor_pos_main_view.x / transform_context.zoom - text_coord_x_before_zoom;
            transform_context.pan_texture_pos.y = transform_context.cursor_pos_main_view.y / transform_context.zoom - text_coord_y_before_zoom;

        }
        else {

            if(transform_context.zoom < 0.125)
                return;

            transform_context.zoom *= 0.8;

            // Pan to match the text coord target with old zoom value
            transform_context.pan_texture_pos.x = transform_context.cursor_pos_main_view.x / transform_context.zoom - text_coord_x_before_zoom;
            transform_context.pan_texture_pos.y = transform_context.cursor_pos_main_view.y / transform_context.zoom - text_coord_y_before_zoom;

        }

        draw_shader.set_transform_context(transform_context);
        draw_shader.update_main_view();
    }

    void set_cursor_main_view(ViewportCursor cursor_main_view){
        transform_context.cursor_pos_main_view.x = cursor_main_view.x;
        transform_context.cursor_pos_main_view.y = cursor_main_view.y;
    }

    void cursor_move(ViewportCursor cursor_delta){

        transform_context.cursor_pos_scene.x = transform_context.cursor_pos_main_view.x / transform_context.zoom - transform_context.pan_texture_pos.x;
        transform_context.cursor_pos_scene.y = transform_context.cursor_pos_main_view.y / transform_context.zoom - transform_context.pan_texture_pos.y;

        transform_context.cursor_pos_scene_px.x = (int) std::floor(transform_context.cursor_pos_scene.x);
        transform_context.cursor_pos_scene_px.y = (int) std::floor(transform_context.cursor_pos_scene.y);

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
            transform_context.pan_texture_pos.x += cursor_delta.x / transform_context.zoom;
            transform_context.pan_texture_pos.y += cursor_delta.y / transform_context.zoom;
            draw_shader.set_transform_context(transform_context);
        }

        if(draw_state.drawing){
            bitmap_texture_dynamic->draw(transform_context.cursor_pos_scene_px.x, transform_context.cursor_pos_scene_px.y);
        }
    }

    TransformContext& ui_get_transform_context(){
        return transform_context;
    }

    Brush& ui_get_brush(){
        return brush_current;
    }


BitmapTexture_Dynamic::BitmapTexture_Dynamic(uint width, uint height) 
    :   bitmap_current_state { Bitmap(width , height) },
        bitmap_rendered         { Bitmap(width , height) }
{
    // SET BITMAP COLOR TO ALL-BLACK
    bitmap_rendered.replace_color({0,0,0,0}, {0,0,0, 255});
    bitmap_current_state.replace_color({0,0,0,0}, {0,0,0, 255});

    box.pos.x = 0.0f;
    box.pos.y = 0.0f;
    box.size.x = (float) width;
    box.size.y = (float) height;


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


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap_rendered.width, bitmap_rendered.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap_rendered.pixels.data());
    glGenerateMipmap(GL_TEXTURE_2D);


    reload_texture();

}

void BitmapTexture_Dynamic::transaction_undo(){
    // No edits to undo
    if(index_last_bitmap_transaction == -1)
        return;

        
    for( PixelTransaction pixel_tr : bitmap_transactions[index_last_bitmap_transaction] ){

        bitmap_current_state.set_pixel(pixel_tr.x, pixel_tr.y, pixel_tr.pixel_from);
        bitmap_rendered.set_pixel(pixel_tr.x, pixel_tr.y, pixel_tr.pixel_from);
        
    }
    
    // Move to previous transaction
    --index_last_bitmap_transaction;
    bitmap_texture_dynamic->reload_texture();
}
void BitmapTexture_Dynamic::transaction_redo(){

    if(bitmap_transactions.size() == 0)
        return;

    // Index is at end of transaction vector, thus there is no edits to redo
    if(index_last_bitmap_transaction == bitmap_transactions.size() - 1)
        return;

    // Use next transaction
    ++index_last_bitmap_transaction;

    for( PixelTransaction pixel_tr : bitmap_transactions[index_last_bitmap_transaction] ){

        bitmap_current_state.set_pixel(pixel_tr.x, pixel_tr.y, pixel_tr.pixel_to);
        bitmap_rendered.set_pixel(pixel_tr.x, pixel_tr.y, pixel_tr.pixel_to);
        
    }
    

    bitmap_texture_dynamic->reload_texture();
}

void BitmapTexture_Dynamic::transaction_new(){

    // when a new transaction is performed, all transaction ahead of current index is removed
    if(bitmap_transactions.size() != 0){

        if(index_last_bitmap_transaction != bitmap_transactions.size()-1){
            while(index_last_bitmap_transaction != bitmap_transactions.size()-1){
                
                bitmap_transactions.pop_back();
            }
        }

    }

    BitmapTransaction new_bitmap_transaction = bitmap_transactions.emplace_back(BitmapTransaction());

    index_last_bitmap_transaction = bitmap_transactions.size() - 1;
    
    

    // STEP THROUGH ALL PIXELS, STORE PIXELS THAT DIFFER TO TRANSACTION VECTOR
    for(int col = 0; col < bitmap_rendered.width; col++){
        for(int row = 0; row < bitmap_rendered.height; row++){

            BitmapPixel pix_new   = bitmap_rendered.get_pixel(col, row);
            BitmapPixel pix_last  = bitmap_current_state.get_pixel(col, row);

            bool pixel_are_equal = pixels_equal(pix_new, pix_last);

            if(!pixel_are_equal){

                // Store bitmap diff
                bitmap_transactions[index_last_bitmap_transaction].emplace_back( 
                    PixelTransaction{
                        col, 
                        row, 
                        pix_last,
                        pix_new
                    }
                );

                // Update the previous transaction bitmap to metch the rendered one
                bitmap_current_state.set_pixel(col, row, pix_new);
            }
            
            
        }
    }
    int x = 0;
}

void BitmapTexture_Dynamic::draw(int x_pointer, int y_pointer){

    // NEVER DRAW WHEN NOT CLICKING ON IMAGE
    if(x_pointer < 0 || y_pointer < 0){
        return;
    }
    // OK to compare to unsigned for now as we clear negative cases previously
    if(x_pointer > bitmap_rendered.width-1 || y_pointer > bitmap_rendered.height-1){
        return;
    }

    
    bitmap_rendered.set_square(x_pointer, y_pointer, brush_current.color, brush_current.size);

    reload_texture();
}

void BitmapTexture_Dynamic::reload_texture(){

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexSubImage2D(    GL_TEXTURE_2D, 0 , // identical to glTexImage2D
                        0, 0, // x, y
                        bitmap_rendered.width, bitmap_rendered.height, 
                        GL_RGBA, GL_UNSIGNED_BYTE, bitmap_rendered.pixels.data()); // identical to glTexImage2D

}

// make sure the transform matrix is updated to current height, width, x, and y
void BitmapTexture_Dynamic::updateTransformationMatrix() {

    squareTransform[0] = box.size.x;
    squareTransform[5] = box.size.y;

    squareTransform[3] = box.pos.x;
    squareTransform[7] = box.pos.y;

}




}
