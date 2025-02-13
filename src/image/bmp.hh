#ifndef BMP_HH
#define BMP_HH

#include <filesystem>


namespace pimage {

typedef struct RGBA {
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
} RGBA;

// In memory bitmap structure that provides raw data access to underlying buffer bytes or pixels.
// Is always 4-byte RGBA structure.
typedef class Bitmap {
    public:
        unsigned char* data;
        unsigned int size;

        RGBA* pixels;
        unsigned int pixelcount;

        unsigned int height;
        unsigned int width;

        // Loads an already existing bitmap data into RGBA bitmap.
        // If bytes per pixel is not 4, then a new char array will be allocated to match the RGBA structure.
        Bitmap(unsigned char* data, unsigned int dataSize, unsigned int height, unsigned int width, unsigned char bytesPerPixel);
        // New RGBA Bitmap of specified size.
        Bitmap(unsigned int height, unsigned int width);
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
    short BM;
} BMP_Header;

typedef struct BMP_Result {
    ::pimage::Bitmap* bitmap;
    LoadStatus loadStatus;
} BMP_Result;

typedef struct BMP_Loader {
    ::pimage::Bitmap* bitmap;
    LoadStatus loadStatus;
} BMP_Loader;

// ::pimage::loaders::BMP_Result BMP_load(std::filesystem::path filePath);
::pimage::Bitmap* BMP_load(std::filesystem::path filePath);


}

#endif