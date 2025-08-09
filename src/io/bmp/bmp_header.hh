
#pragma once

#include <vector>

struct Str;
struct Bitmap;


namespace BMP 
{

/** File header info. */
struct FileHeader
{
    // alignas(14)
    // sizeof() = 16

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

    bool operator!=(const FileHeader& rhs) { return *this == rhs ? false : true;};
    bool operator==(const FileHeader& rhs)
    {
        return  BM                      == rhs.BM                   &&
                file_size               == rhs.file_size            &&
                reserved_1              == rhs.reserved_1           &&
                reserved_2              == rhs.reserved_2           &&
                first_pixel_location    == rhs.first_pixel_location;
    }

    Str to_str();
};





/// @brief Stores the loaded bmp-file header in BIG-endian byte order [opposite of file], except 'BM' which remains as is. \n
/// Data size is 54 bytes and padded to 56 (usually).
struct BITMAPINFOHEADER 
{
    // alignas(40)
    // sizeof() = 40

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
    /// Number of bytes that represent the actual image. 
    /// Including the padding bytes of 4-aligned rows.
    unsigned int    image_size;
    /// Dot density in usints of pixels/meter. DPI ~ pixels/meter \div 39.37.
    unsigned int    horizontal_resolution;
    /// Dot density in usints of pixels/meter. DPI ~ pixels/meter \div 39.37. 
    unsigned int    vertical_resolution;
    // A value of 0 -> 2^n (default)
    unsigned int    palette_color_count;
    // Value of 0 treats all colors equally
    unsigned int    important_color_count;


    bool operator!=(const BITMAPINFOHEADER& rhs) { return *this == rhs ? false : true;};
    bool operator==(const BITMAPINFOHEADER& rhs)
    {
        return  header_size             == rhs.header_size              &&
                width                   == rhs.width                    &&
                height                  == rhs.height                   &&
                color_planes            == rhs.color_planes             &&
                bits_per_pixel          == rhs.bits_per_pixel           &&
                compression_method      == rhs.compression_method       &&
                image_size               == rhs.image_size                &&
                horizontal_resolution   == rhs.horizontal_resolution    &&
                vertical_resolution     == rhs.vertical_resolution      &&
                palette_color_count     == rhs.palette_color_count      &&
                important_color_count   == rhs.important_color_count;
    }
};


/** Represents a complete BMP header.
    Is made up of 1) a `file_header`, and 2) a `DIB` header.
    Currently the only supported `DIB` header is `BITMAPINFOHEADER`.
    NOTE: the fil */
struct Header 
{
    FileHeader file_header;
    BITMAPINFOHEADER info_header;

    enum Endian
    {
        BIG,
        LITTLE,
    } endian = LITTLE;

    const unsigned int size_file_header = 14;
    const unsigned int size_BITMAPINFOHEADER = 40;
    const unsigned int size_header = size_file_header + size_BITMAPINFOHEADER;

    unsigned int calculate_padded_bitmap_size(Bitmap& bitmap, unsigned int bytes_per_pixel);

    void extract_header(std::vector<unsigned char>& file_data);

    void construct_header(Bitmap& bitmap);

    void endian_swap();
    void print();
};


}