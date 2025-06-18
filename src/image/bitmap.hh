#ifndef BITMAP_HH
#define BITMAP_HH


#include <filesystem>
#include <vector>

#include "math/vecmat.hh"

namespace pimage {

typedef struct Pixel {
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;

    bool operator==(Pixel& rhs)
    {
        if (R == rhs.R && G == rhs.G && B == rhs.B && A == rhs.A)
            return true;
        return false;
    }
} Pixel;

extern Pixel pixel_color_white;
extern Pixel pixel_color_black;

extern Pixel pixel_color_red;
extern Pixel pixel_color_green;
extern Pixel pixel_color_blue;


// In memory bitmap structure that provides raw data access to underlying buffer bytes or pixels.
// Is always 4-byte RGBA structure with x,y = 0 at bottom left corner.
typedef class Bitmap {
    public:

        std::vector<Pixel> pixels;
        unsigned int pixelcount;

        unsigned int height;
        unsigned int width;

        // New RGBA Bitmap of specified size.
        Bitmap(unsigned int width, unsigned int height);
        Bitmap(ui2 size);

        /** Set pixel with bound checking. If out of bounds nothing happens. */
        void  set_pixel(unsigned int x, unsigned int y, Pixel pixel);
        void  set_square(unsigned int x, unsigned int y, Pixel pixel, int size);
        Pixel get_pixel(unsigned int x, unsigned int y);

        /** Set all pixels to new value */
        void clear(Pixel clear_pixel);

        Bitmap get_sub_bitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
        /** Write bitmap onto pther bitmap. Does not perform any size checking! */
        void   set_sub_bitmap(unsigned int x, unsigned int y, Bitmap& bitmap_to_write);

        // void append_right(Bitmap& bitmap_to_append);

        /** Replace all pixels in bitmap with new value. NOTE: VERY INEFFICIENT! */
        void replace_color(Pixel old_pixel, Pixel new_pixel);

        bool has_a_non_black_white_pixel();

} Bitmap;

bool pixels_equal(Pixel a, Pixel b);

} // pimage


#endif