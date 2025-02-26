#include <stdlib.h> 

#include <iostream>
#include <fstream>
#include <vector>

#include "lib/fs.hh"

#include "lib/process.hh"

#include "bitmap.hh"


namespace pimage {

Bitmap::Bitmap(unsigned int width, unsigned int height) {

    this->height = height;
    this->width = width;
    
    this->pixelcount = height * width;
    this->pixels = std::vector<pimage::Pixel> (this->pixelcount);
    
}


void Bitmap::set_pixel(unsigned int x, unsigned int y, Pixel pixel) {

    unsigned int pixelIndex = this->width * y + x;

    this->pixels[pixelIndex] = pixel;

}

Pixel Bitmap::get_pixel(unsigned int x, unsigned int y) {

    unsigned int pixelIndex = this->width * y + x;

    return this->pixels[pixelIndex];
}

Bitmap Bitmap::get_rectangle(unsigned int x, unsigned int y, unsigned int w, unsigned int h){
    Bitmap rectangle = Bitmap(w, h);

    // Loop through rectangle dimensions and copy into rectangle
    for(unsigned int r = 0; r < h ; r++){
        for(unsigned int c = 0; c < w ; c++){
            rectangle.set_pixel(c, r, get_pixel(x+c, y+r));
        }
    }

    return rectangle;
}

void   Bitmap::set_rectangle(unsigned int x, unsigned int y, Bitmap& bitmap_to_write){
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


void Bitmap::replace_color(Pixel old_pixel, Pixel new_pixel){
    for(Pixel& pixel : pixels){
        // Pixel to replace identified
        if(pixel.R == old_pixel.R && pixel.G == old_pixel.G && pixel.B == old_pixel.B && pixel.A == old_pixel.A){
    
            pixel.R = new_pixel.R;
            pixel.G = new_pixel.G;
            pixel.B = new_pixel.B;
            pixel.A = new_pixel.A;

        }

    }
}

}

