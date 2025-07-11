#include <stdlib.h> 

#include <iostream>
#include <fstream>
#include <vector>

#include "lib/fs.hh"

#include "lib/process.hh"

#include "bitmap.hh"




BitmapPixel pixel_color_white = {255, 255, 255, 255};
BitmapPixel pixel_color_black = {0, 0, 0, 255};

BitmapPixel pixel_color_red   = {255, 0, 0, 255};
BitmapPixel pixel_color_green = {0, 255, 0, 255};
BitmapPixel pixel_color_blue  = {0, 0, 255, 255};

Bitmap::Bitmap(unsigned int width, unsigned int height) {

    this->height = height;
    this->width = width;
    
    this->pixelcount = height * width;
    this->pixels = std::vector<BitmapPixel> (this->pixelcount);
    
}

Bitmap::Bitmap(ui2 size)
{
    width = size.x;
    height = size.y;
        
    pixelcount = height * width;

    pixels = std::vector<BitmapPixel> { pixelcount };
}

Bitmap::Bitmap(ui2 size, BitmapPixel pixel)
{
    width = size.x;
    height = size.y;
        
    pixelcount = height * width;

    pixels = std::vector<BitmapPixel> { pixelcount };

    pixels[0] = pixel;
}


void Bitmap::set_pixel(unsigned int x, unsigned int y, BitmapPixel pixel) 
{
    if(x > width-1 || y > height-1)
        return;

    unsigned int pixelIndex = this->width * y + x;

    this->pixels[pixelIndex] = pixel;

}

void  Bitmap::set_square(unsigned int x, unsigned int y, BitmapPixel pixel, int size){
    
    // trying to draw outside bitmap bounds not valid
    if(x > width || y > height)
        return;

    // Cut pixels beyond bitmap bounds+
    unsigned int x_max = x+size > width ? width : x+size;
    unsigned int y_max = y+size > height ? height : y+size;


    for(unsigned int r = y; r < y_max ; r++){
        for(unsigned int c = x; c < x_max ; c++){
            set_pixel(c, r, pixel);
        }
    }
}

BitmapPixel Bitmap::get_pixel(unsigned int x, unsigned int y) {

    unsigned int pixelIndex = this->width * y + x;

    return this->pixels[pixelIndex];
}

void Bitmap::clear(BitmapPixel clear_pixel)
{
    for(auto& pixel : pixels)
    {
        pixel = clear_pixel;
    }
}

Bitmap Bitmap::get_sub_bitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h){
    Bitmap rectangle = Bitmap(w, h);

    // Loop through rectangle dimensions and copy into rectangle
    for(unsigned int r = 0; r < h ; r++){
        for(unsigned int c = 0; c < w ; c++){
            rectangle.set_pixel(c, r, get_pixel(x+c, y+r));
        }
    }

    return rectangle;
}

void   Bitmap::set_sub_bitmap(unsigned int x, unsigned int y, Bitmap& bitmap_to_write){
    unsigned int h = bitmap_to_write.height;
    unsigned int w = bitmap_to_write.width;

    // Loop through rectangle dimensions and copy from rectangle
    for(unsigned int r = 0; r < h ; r++){
        for(unsigned int c = 0; c < w ; c++){

            set_pixel(x+c, y+r, bitmap_to_write.get_pixel(c, r));

        }
    }
}

// void Bitmap::append_right(Bitmap& bitmap_to_append){
//     if(height != bitmap_to_append.height){
//         plib::plog_error("IMAGE", "IMAGE_MANIP", "Failed to append bitmap on the right. Bitmaps different size. ");
//     }
// }


void Bitmap::replace_color(BitmapPixel old_pixel, BitmapPixel new_pixel){
    for(BitmapPixel& pixel : pixels){
        // Pixel to replace identified
        if(pixel.R == old_pixel.R && pixel.G == old_pixel.G && pixel.B == old_pixel.B && pixel.A == old_pixel.A){
    
            pixel.R = new_pixel.R;
            pixel.G = new_pixel.G;
            pixel.B = new_pixel.B;
            pixel.A = new_pixel.A;

        }

    }
}

bool Bitmap::has_a_non_black_white_pixel()
{
    BitmapPixel white = {255, 255, 255, 255};
    BitmapPixel black = {0  , 0  , 0  , 255};

    for(auto pixel : pixels)
    {
        if(pixel != white && pixel != black)
            return true;
    }

    return false;
}


BitmapPixel* Bitmap::pointer()
{
    return pixels.data();
}


bool pixels_equal(BitmapPixel a, BitmapPixel b){
    return  a.R == b.R &&
            a.G == b.G &&
            a.B == b.B &&
            a.A == b.A;
}




