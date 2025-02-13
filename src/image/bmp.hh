#ifndef BMP_HH
#define BMP_HH

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
        // TODO: turn into vector<unsigned char>
        // unsigned char* data;
        // unsigned int size;

        // Pixel* pixels;
        std::vector<Pixel> pixels;
        unsigned int pixelcount;

        unsigned int height;
        unsigned int width;

        void  set_pixel(unsigned int x, unsigned int y, Pixel pixel);
        Pixel get_pixel(unsigned int x, unsigned int y);

        // New RGBA Bitmap of specified size.
        Bitmap(unsigned int width, unsigned int height);
        ~Bitmap();
} Bitmap;

}

namespace pimage::io {

typedef enum LoadStatus {
    Ok = 0,

    ErrorOpeningFile = 10,
    ErrorFileFormat = 11,
    ErrorParsing = 12,

    ErrorUnknown = 13,
} LoadStatus;


/// @brief Stores the loaded bmp-file header in BIG-endian byte order [opposite of file], except 'BM' which remains as is. \n
/// Data size is 54 bytes and padded to 56 (usually).
typedef struct BMP_Header_BITMAPINFOHEADER {
// sizeof() = 56
// used size = 54

    // Filetype identifier : 'BM' as the first two bytes.
    unsigned short  BM;
    unsigned int    file_size;
    unsigned short  reserved_1;
    unsigned short  reserved_2;
    unsigned int    first_pixel_location;

    // BITMAPINFOHEADER

    // Size of BITMAPINFOHEADER - usually measured from this location to the first pixel.
    unsigned int    header_size;
    unsigned int    width;
    unsigned int    height;
    unsigned short  color_planes;
    unsigned short  bits_per_pixel; // TODO: make sure it works. Was changed from int to short!
    unsigned int    compression_method;
    /// Number of bytes that represent the actual image. Includes the padding bytes of 4-aligned rows.
    unsigned int    imageSize;
    /// Dot density in usints of pixels/meter. DPI ~ pixels/meter \div 39.37.
    unsigned int    horizontal_resolution;
    /// Dot density in usints of pixels/meter. DPI ~ pixels/meter \div 39.37. 
    unsigned int    vertical_resolution;
    // A value of 0 -> 2^n (default)
    unsigned int    palette_color_count;
    // Value of 0 treats all colors equally
    unsigned int    important_color_count;
    
} BMP_Header_BITMAPINFOHEADER;


typedef struct BMP_Result {
    ::pimage::Bitmap* bitmap;
    LoadStatus loadStatus;
} BMP_Result;


typedef struct BMP {
    ::pimage::Bitmap* bitmap;

    BMP_Header_BITMAPINFOHEADER* header;
    LoadStatus loadStatus;

    // Usually 3.
    unsigned int bytesPerPixel;
    // Number of bytes in one horizontal row taken up by actual pixel data.
    unsigned int bytesPerImageRow;
    // Total number of bytes of one horizontal image row. Is padded to 4-byte alignment.
    unsigned int bytesPerImageRow_padded;
    
    
    ::pimage::Bitmap*   load(std::filesystem::path filePath);
    BMP_Header_BITMAPINFOHEADER*         
                        extract_header_BITMAPINFOHEADER(std::vector<unsigned char>& bmp_data);
    void                load_header_BITMAPINFOHEADER();
    bool                save(std::filesystem::path filePath);
    void                print_header();
    
    BMP();
} BMP;


}

#endif