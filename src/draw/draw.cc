

#include "glad/glad.h"

#include "draw.hh"

#include "draw_shader.hh"


namespace draw {

    BitmapTexture_Dynamic* bitmap_texture_dynamic;

    void init(PhysWin new_window){

        draw_shader.set_window_info(
            new_window.raw.w,
            new_window.raw.w,
            new_window.xscale,
            new_window.yscale
        );
        draw_shader.init();
        // draw_shader.compile_shader();

        bitmap_texture_dynamic = new BitmapTexture_Dynamic(100, 100);
        // bitmap_texture_dynamic
    }

    void draw(){
        bitmap_texture_dynamic->updateTransformationMatrix();
        draw_shader.set(bitmap_texture_dynamic->drawRectangleTransform16, bitmap_texture_dynamic->glTexture);
        draw_shader.draw();
    }


BitmapTexture_Dynamic::BitmapTexture_Dynamic(uint width, uint height) 
    :   bitmap { pimage::Bitmap(width , height) }
{

    aabb.w = width;
    aabb.h = height;
    aabb.x = 0;
    aabb.y = 0;

    // pimage::Bitmap bitmap = pimage::Bitmap(width, height);/.

    id = "bitmap_texture_dynamic";

    glTexture = 0;

    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, glTexture);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.width, bitmap.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.pixels.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    // return newTexture;


}

// make sure the transform matrix is updated to current height, width, x, and y
void BitmapTexture_Dynamic::updateTransformationMatrix() {

    drawRectangleTransform16[0] = aabb.w;
    drawRectangleTransform16[5] = aabb.h;

    drawRectangleTransform16[3] = aabb.x;
    drawRectangleTransform16[7] = aabb.y;

}




}
