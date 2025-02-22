#ifndef BITMAP_HH
#define BITMAP_HH

#include <filesystem>
#include <vector>


namespace pimage {

typedef struct Pixel {
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
} Pixel;


// In memory bitmap structure that provides raw data access to underlying buffer bytes or pixels.
// Is always 4-byte RGBA structure with x,y = 0 at bottom left corner.
typedef class Bitmap {
    public:

        std::vector<Pixel> pixels;
        unsigned int pixelcount;

        unsigned int height;
        unsigned int width;

        void  set_pixel(unsigned int x, unsigned int y, Pixel pixel);
        Pixel get_pixel(unsigned int x, unsigned int y);

        Bitmap get_rectangle(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
        /** Write bitmap onto pther bitmap. Does not perform any size checking! */
        void   set_rectangle(unsigned int x, unsigned int y, Bitmap& bitmap_to_write);

        // void append_right(Bitmap& bitmap_to_append);

        /** Replace all pixels in bitmap with new value. NOTE: VERY INEFFICIENT! */
        void replace_color(Pixel old_pixel, Pixel new_pixel);

        // New RGBA Bitmap of specified size.
        Bitmap(unsigned int width, unsigned int height);
} Bitmap;



}


#endif