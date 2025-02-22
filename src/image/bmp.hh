#ifndef BMP_HH
#define BMP_HH

#include <filesystem>
#include <vector>

#include "image/bitmap.hh"


namespace pimage::io {

extern unsigned int BMP_Header_BITMAPINFOHEADER_size;


typedef enum LoadStatus {
    Ok = 0,

    ErrorOpeningFile = 10,
    ErrorFileFormat = 11,
    ErrorParsing = 12,

    ErrorUnknown = 13,
} LoadStatus;



/// @brief Stores the loaded bmp-file header in BIG-endian byte order [opposite of file], except 'BM' which remains as is. \n
/// Data size is 54 bytes and padded to 56 (usually).
typedef struct BMP_BITMAPINFOHEADER {
// sizeof() = 56
// used size = 54

    // Filetype identifier : 'BM' as the first two bytes.
    unsigned short  BM;
    // Total file size in bytes
    unsigned int    file_size;
    // Applicaiton specific
    unsigned short  reserved_1;
    // Applicaiton specific
    unsigned short  reserved_2;
    // Byte count from beginning of file
    unsigned int    first_pixel_location;


    // BITMAPINFOHEADER

    // Size of BITMAPINFOHEADER - usually measured from this location to the first pixel.
    unsigned int    header_size;
    // Image pixel width
    unsigned int    width;
    // Image pixel height
    unsigned int    height;
    // Will always be one
    unsigned short  color_planes;
    // Traditionally 24 - RGB, no alpha
    unsigned short  bits_per_pixel;
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
    
} BMP_BITMAPINFOHEADER;


/** BMP loader and writer. Uses pimage::bitmap for internal image representation.  */
typedef struct BMP {
    // ::pimage::Bitmap* bitmap = nullptr;

    // BITMAPINFOHEADER header
    BMP_BITMAPINFOHEADER* header;
    // In memory buffer of file contents.
    std::vector<unsigned char>* bmp_buffer = nullptr;


    // Usually 3.
    unsigned int bytesPerPixel;
    // Number of bytes in one horizontal row taken up by actual pixel data.
    unsigned int bytesPerImageRow;
    // Total number of bytes of one horizontal image row. Is padded to 4-byte alignment.
    unsigned int bytesPerImageRow_padded;
    
    // NOT CURRENTLY IN USE
    LoadStatus loadStatus;

    // Load a bmp file from file.
    ::pimage::Bitmap   load(std::filesystem::path filePath);
    // Internal. 
    // Copies the header data from bmp_buffer to header struct.
    // Currently 'BITMAPINFOHEADER' is the only header that I support
    // Header formats : https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header
    void                extract_header_BITMAPINFOHEADER();
    // Set the internal BMP structure using a physimos bitmap, making it read for write calls.
    bool                from_bitmap(::pimage::Bitmap* new_bitmap);
    // Write the in-memory bmp_buffer to file. 
    bool                write(std::filesystem::path filePath);
    // Pretty print the contents of the struct. 
    void                print_header();

    
    BMP();
    // Convenience constructor that is equivelent to 'bmp=BMP(); bmp.from_bitmap(...)'
    BMP(::pimage::Bitmap* new_bitmap);
    ~BMP();
} BMP;


}

#endif