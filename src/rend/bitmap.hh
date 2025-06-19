#ifndef BITMAP_HH
#define BITMAP_HH


#include <filesystem>
#include <vector>

#include "math/vecmat.hh"



struct BitmapPixel {
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;

    bool operator==(BitmapPixel& rhs)
    {
        if (R == rhs.R && G == rhs.G && B == rhs.B && A == rhs.A)
            return true;
        return false;
    }
};

extern BitmapPixel pixel_color_white;
extern BitmapPixel pixel_color_black;

extern BitmapPixel pixel_color_red;
extern BitmapPixel pixel_color_green;
extern BitmapPixel pixel_color_blue;


// In memory bitmap structure that provides raw data access to underlying buffer bytes or pixels.
// Is always 4-byte RGBA structure with x,y = 0 at bottom left corner.
typedef class Bitmap {
    public:

        std::vector<BitmapPixel> pixels;
        unsigned int pixelcount;

        unsigned int height;
        unsigned int width;

        // New RGBA Bitmap of specified size.
        Bitmap(ui2 size);
        Bitmap(ui2 size, BitmapPixel pixel); // set an initial bitmap value
        Bitmap(unsigned int width, unsigned int height);

        /** Set pixel with bound checking. If out of bounds nothing happens. */
        void  set_pixel(unsigned int x, unsigned int y, BitmapPixel pixel);
        void  set_square(unsigned int x, unsigned int y, BitmapPixel pixel, int size);
        BitmapPixel get_pixel(unsigned int x, unsigned int y);

        /** Set all pixels to new value */
        void clear(BitmapPixel clear_pixel);

        Bitmap get_sub_bitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
        /** Write bitmap onto pther bitmap. Does not perform any size checking! */
        void   set_sub_bitmap(unsigned int x, unsigned int y, Bitmap& bitmap_to_write);

        // void append_right(Bitmap& bitmap_to_append);

        /** Replace all pixels in bitmap with new value. NOTE: VERY INEFFICIENT! */
        void replace_color(BitmapPixel old_pixel, BitmapPixel new_pixel);

        bool has_a_non_black_white_pixel();

        BitmapPixel* pointer();

} Bitmap;

bool pixels_equal(BitmapPixel a, BitmapPixel b);




#endif