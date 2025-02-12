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

typedef class Bitmap {
    public:
        RGBA* pixels;
        unsigned int size;

        unsigned char* data;
        unsigned int pixelcount;

        Bitmap(unsigned char data, unsigned int size); // load an already existing bitmap
        Bitmap(unsigned int size); // Allocate size
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

::pimage::loaders::BMP_Result BMP_load(std::filesystem::path filePath);


}

#endif