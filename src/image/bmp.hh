#ifndef BMP_HH
#define BMP_HH

#include <filesystem>


namespace pimage {

typedef struct Pixel {
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
} Pixel;

// In memory bitmap structure that provides raw data access to underlying buffer bytes or pixels.
// Is always 4-byte RGBA structure.
typedef class Bitmap {
    public:
        unsigned char* data;
        unsigned int size;

        Pixel* pixels;
        unsigned int pixelcount;

        unsigned int height;
        unsigned int width;

        void  set_pixel(unsigned int x, unsigned int y, Pixel pixel);
        Pixel set_pixel(unsigned int x, unsigned int y);

        // Loads an already existing bitmap data into RGBA bitmap.
        // If bytes per pixel is not 4, then a new char array will be allocated to match the RGBA structure.
        Bitmap(unsigned char* data, unsigned int dataSize, unsigned int height, unsigned int width, unsigned char bytesPerPixel);
        // New RGBA Bitmap of specified size.
        Bitmap(unsigned int width, unsigned int height);
        ~Bitmap();
} Bitmap;

}

namespace pimage::loaders {

typedef enum LoadStatus {
    Ok = 0,

    ErrorOpeningFile = 10,
    ErrorFileFormat = 11,
    ErrorParsing = 12,

    ErrorUnknown = 13,
} LoadStatus;

typedef struct BMP_Header {

    unsigned short  BM;
    unsigned int    file_size;
    unsigned short  reserved_1;
    unsigned short  reserved_2;
    unsigned int    first_pixel_location;

    unsigned int    header_size;
    unsigned int    width;
    unsigned int    height;
    unsigned short  color_planes;
    unsigned short  bits_per_pixel; // TODO: make sure it works. Was changed from int to short!
    unsigned int    compression_method;
    unsigned int    imageSize;
    unsigned int    horizontal_resolution;
    unsigned int    vertical_resolution;
    unsigned int    palette_color_count;
    unsigned int    important_color_count;
    
} BMP_Header;

typedef struct BMP_Result {
    ::pimage::Bitmap* bitmap;
    LoadStatus loadStatus;
} BMP_Result;

typedef struct BMP_Loader {
    ::pimage::Bitmap* bitmap;

    BMP_Header* header;

    unsigned int bytesPerPixel;
    unsigned int bytesPerImageRow;
    unsigned int bytesPerImageRow_padded;
    
    LoadStatus loadStatus;

    ::pimage::Bitmap* load(std::filesystem::path filePath);

    void print_header();
    
    BMP_Loader();
} BMP_Loader;

// ::pimage::loaders::BMP_Result BMP_load(std::filesystem::path filePath);


}

#endif