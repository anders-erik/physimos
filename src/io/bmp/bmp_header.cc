#include <cstring>

#include <iostream>

#include "rend/bitmap.hh"

#include "bmp_header.hh"


namespace BMP 
{



unsigned int Header::
calculate_padded_bitmap_size(Bitmap & bitmap, unsigned int bytes_per_pixel)
{
    // Number of bytes per image width assuming NO PADDING
    unsigned int bytes_per_image_row = bytes_per_pixel * bitmap.width;

    // Image width in file WITH PADDING
    unsigned int bytes_per_image_row_padded = bytes_per_image_row;


    // Each row is padded to 4-byte alignment
    if (bytes_per_pixel != 4) 
    { 
        unsigned int padding_size = 4 - (bytes_per_image_row % 4);
        bytes_per_image_row_padded += padding_size;
    }

    return bytes_per_image_row_padded;
}



void Header::
extract_header(std::vector<unsigned char>& file_buffer)
{
    endian = LITTLE;

    unsigned char* data = file_buffer.data();

    memcpy( &file_header, 
            data, 
            size_file_header);

    memcpy( &info_header, 
            data + size_file_header, // pointer arithmetic to beginning of BITMAPINFOHEADER
            size_BITMAPINFOHEADER);

}




void Header::
construct_header(Bitmap& bitmap)
{
    using uchar = unsigned char;
    
    endian = BIG; // copy directly into header

    const unsigned int bytesPerPixel = 3; // Always RGB

    const unsigned int padded_data_size = calculate_padded_bitmap_size(bitmap, bytesPerPixel);

    // FILE HEADER
    file_header.BM = 0x6677;
    file_header.file_size = size_file_header + 
                            size_BITMAPINFOHEADER +
                            padded_data_size;
    file_header.reserved_1 = 0x0000;
    file_header.reserved_2 = 0x0000;
    file_header.first_pixel_location = size_header;  // First pixel is always at 54 (0x36) for BITMAPINFOHEADER

    // BITMAPINFOHEADER
    info_header.header_size             = size_BITMAPINFOHEADER;
    info_header.width                   = bitmap.width;
    info_header.height                  = bitmap.height;
    info_header.color_planes            = 1;
    info_header.bits_per_pixel          = 24;
    info_header.compression_method      = 0;
    info_header.image_size               = padded_data_size;
    info_header.horizontal_resolution   = 3780; // 96 DPI * 39.27
    info_header.vertical_resolution     = 3780; // 96 DPI * 39.27
    info_header.palette_color_count     = 0;
    info_header.important_color_count   = 0;
}





void Header::
endian_swap()
{
    using uchar = unsigned char;

    endian = endian == BIG ? LITTLE : BIG;

    uchar current_file_header[size_file_header];
    uchar swapped_file_header[size_file_header];

    // Populate current
    memcpy(current_file_header, &file_header, size_file_header);

    // "BM" never changes
    swapped_file_header[0] = current_file_header[0];
    swapped_file_header[1] = current_file_header[1];

    swapped_file_header[2] = current_file_header[5];
    swapped_file_header[3] = current_file_header[4];
    swapped_file_header[4] = current_file_header[3];
    swapped_file_header[5] = current_file_header[2];

    swapped_file_header[6] = current_file_header[7];
    swapped_file_header[7] = current_file_header[6];

    swapped_file_header[8] = current_file_header[9];
    swapped_file_header[9] = current_file_header[8];

    swapped_file_header[10] = current_file_header[13];
    swapped_file_header[11] = current_file_header[12];
    swapped_file_header[12] = current_file_header[11];
    swapped_file_header[13] = current_file_header[10];

    memcpy(&file_header, swapped_file_header, size_file_header);




    // BITMAPINFOHEADER

    uchar current_info_header[size_BITMAPINFOHEADER];
    uchar swapped_info_header[size_BITMAPINFOHEADER];

    // Populate current
    memcpy(current_info_header, &info_header, size_BITMAPINFOHEADER);

    swapped_info_header[0] = current_info_header[3];
    swapped_info_header[1] = current_info_header[2];
    swapped_info_header[2] = current_info_header[1];
    swapped_info_header[3] = current_info_header[0];

    // width
    swapped_info_header[4] = current_info_header[7];
    swapped_info_header[5] = current_info_header[6];
    swapped_info_header[6] = current_info_header[5];
    swapped_info_header[7] = current_info_header[4];

    // height
    swapped_info_header[8] =  current_info_header[11];
    swapped_info_header[9] =  current_info_header[10];
    swapped_info_header[10] = current_info_header[9];
    swapped_info_header[11] = current_info_header[8];

    // Color planes
    swapped_info_header[12] = current_info_header[13];
    swapped_info_header[13] = current_info_header[12];

    // bits per pixel
    swapped_info_header[14] = current_info_header[15];
    swapped_info_header[15] = current_info_header[14];

    // Compression method
    swapped_info_header[16] = current_info_header[19];
    swapped_info_header[17] = current_info_header[18];
    swapped_info_header[18] = current_info_header[17];
    swapped_info_header[19] = current_info_header[16];

    // padded image size
    swapped_info_header[20] = current_info_header[23];
    swapped_info_header[21] = current_info_header[22];
    swapped_info_header[22] = current_info_header[21];
    swapped_info_header[23] = current_info_header[20];

    // hori res
    swapped_info_header[24] = current_info_header[27];
    swapped_info_header[25] = current_info_header[26];
    swapped_info_header[26] = current_info_header[25];
    swapped_info_header[27] = current_info_header[24];

    // Vert res
    swapped_info_header[28] = current_info_header[31];
    swapped_info_header[29] = current_info_header[30];
    swapped_info_header[30] = current_info_header[29];
    swapped_info_header[31] = current_info_header[28];

    // palette_color_count: 0 to default to 2n
    swapped_info_header[32] = current_info_header[35];
    swapped_info_header[33] = current_info_header[34];
    swapped_info_header[34] = current_info_header[33];
    swapped_info_header[35] = current_info_header[32];

    // important colors
    swapped_info_header[36] = current_info_header[39];
    swapped_info_header[37] = current_info_header[38];
    swapped_info_header[38] = current_info_header[37];
    swapped_info_header[39] = current_info_header[36];

    memcpy(&info_header, swapped_info_header, size_BITMAPINFOHEADER);

}




void Header::
print()
{
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "BMP HEADER PRINT:" << std::endl;
    std::cout << "" << std::endl;
    

    std::cout << "BM                        =    \"" << (char)(file_header.BM >> 8) << (char) file_header.BM << "\"" << std::endl;
    std::cout << "file_size                 =    " << file_header.file_size                << "        [byte]" << std::endl;
    std::cout << "reserved_1                =    " << file_header.reserved_1               << std::endl;
    std::cout << "reserved_2                =    " << file_header.reserved_2               << std::endl;
    std::cout << "first_pixel_location      =    " << file_header.first_pixel_location     << "        [byte]" << std::endl;



    std::cout << "header_size               =    " << info_header.header_size              << "        [byte]" << std::endl;
    std::cout << "width                     =    " << info_header.width                    << std::endl;
    std::cout << "height                    =    " << info_header.height                   << std::endl;
    std::cout << "color_planes              =    " << info_header.color_planes             << std::endl;
    std::cout << "bits_per_pixel            =    " << info_header.bits_per_pixel           << std::endl;
    std::cout << "compression_method        =    " << info_header.compression_method       << std::endl;
    std::cout << "imageSize                 =    " << info_header.image_size                << "        [byte]" << std::endl;
    std::cout << "horizontal_resolution     =    " << info_header.horizontal_resolution    << std::endl;
    std::cout << "vertical_resolution       =    " << info_header.vertical_resolution      << std::endl;
    std::cout << "palette_color_count       =    " << info_header.palette_color_count      << std::endl;
    std::cout << "important_color_count     =    " << info_header.important_color_count    << std::endl;


    std::cout << "" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
}

}