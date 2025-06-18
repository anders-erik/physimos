#include <stdlib.h> 
#include <cstring> // memcpy

#include <iostream>
#include <fstream>
#include <vector>

#include "lib/fs.hh"

#include "bmp.hh"


namespace BMP
{


unsigned int BMP_Header_BITMAPINFOHEADER_size = 54;



FileHeader& File::file_header()
{
    return header.file_header;
}

BITMAPINFOHEADER& File::info_header()
{
    return header.info_header;
}

File::File(Bitmap& new_bitmap){
    this->from_bitmap(new_bitmap);
}





bool File::write(std::filesystem::path filePath) {

    plib::FS_STATUS write_status = plib::fs_echo_bin(filePath.string(), bmp_data);
    if (write_status == plib::FsError) {
        std::cout << "EROROROROROOR" << std::endl;
        return false;
    }

    return true;
}




::Bitmap File::load(std::filesystem::path filePath) {

    // save file data in BMP struct
    bmp_data = plib::fs_cat_bin(filePath.string());

    // file format: header size check
    if (bmp_data.size() < 54) 
    {
        throw std::runtime_error("Loaded BMP file is less than 54 bytes");
        return Bitmap(0,0);
    }


    extract_header_BITMAPINFOHEADER();



    // Image Utility Dimensions
    bytesPerPixel = old_header.bits_per_pixel / 8;
    bytesPerImageRow = bytesPerPixel * old_header.width;
    bytesPerImageRow_padded = bytesPerImageRow;

    // If row size is not a multiple of 4 bytes, the row is padded to 4-byte alignment
    if (bytesPerImageRow % 4 != 0) { 
        unsigned int paddingSize = 4 - (bytesPerImageRow % 4);
        bytesPerImageRow_padded = bytesPerImageRow + paddingSize;
    }



    // Create Physimos RGBA Bitmap and copy bmp file data into it
    Bitmap bitmap = Bitmap(old_header.width, old_header.height);

    // NOTE: very much NOT optimized for performance
    // 
    for (unsigned int y = 0; y < old_header.height; y++) {

        for (unsigned int x = 0; x < old_header.width; x++) {

            // Pixel location within raw BMP file buffer
            unsigned int pixelLocation = 0;
            pixelLocation += old_header.first_pixel_location;    // first pixel
            pixelLocation += bytesPerImageRow_padded * y;   // first pixel of row
            pixelLocation += x * bytesPerPixel;             // offset in row

            BitmapPixel newPixel = BitmapPixel{
                bmp_data[pixelLocation + 2],
                bmp_data[pixelLocation + 1],
                bmp_data[pixelLocation + 0],
                255, // No transparancy by default
            };

            bitmap.set_pixel(x, y, newPixel);
        }

    }

    return bitmap;

}



bool File::from_bitmap(Bitmap& new_bitmap) 
{
    header.construct_header(new_bitmap);

    // Utility data
    bytesPerPixel = 3;
    bytesPerImageRow = bytesPerPixel * new_bitmap.width;
    bytesPerImageRow_padded = bytesPerImageRow;
    if (bytesPerPixel != 4) { // Each row is padded to 4-byte alignment
        unsigned int paddingSize = 4 - (bytesPerImageRow % 4);
        bytesPerImageRow_padded = bytesPerImageRow + paddingSize;
    }

    unsigned int output_file_size = BMP_Header_BITMAPINFOHEADER_size + new_bitmap.height * bytesPerImageRow_padded;
    // std::vector<unsigned char> bmp_data(output_file_size);

    // New in-memory buffer
    bmp_data.clear();
    bmp_data.reserve(output_file_size);


    // Reference to BMP buffer for convenience
    // std::vector<unsigned char>& bmp_data = bmp_buffer;

    // from_bitmap();
    bmp_data[0] = 'B';
    bmp_data[1] = 'M';

    // output should be little endian, so we load least significant first
    bmp_data[2] = (output_file_size & 0x000000FF);
    bmp_data[3] = (output_file_size & 0x0000FF00) >> 8;
    bmp_data[4] = (output_file_size & 0x00FF0000) >> 16;
    bmp_data[5] = (output_file_size & 0xFF000000) >> 24;

    // reserved_1 - unused
    bmp_data[6] = 0;
    bmp_data[7] = 0;

    // reserved_2 - unused
    bmp_data[8] = 0;
    bmp_data[9] = 0;

    // First pixel is always at 54 (0x36) for BITMAPINFOHEADER
    unsigned int first_pixel_location = 54;
    bmp_data[10] = first_pixel_location;
    bmp_data[11] = 0;
    bmp_data[12] = 0;
    bmp_data[13] = 0;



    // BITMAPINFOHEADER begins here

    // BITMAPINFOHEADER is always 40 bytes
    bmp_data[14] = 40;
    bmp_data[15] = 0;
    bmp_data[16] = 0;
    bmp_data[17] = 0;

    // width
    bmp_data[18] = (new_bitmap.width & 0x000000FF);
    bmp_data[19] = (new_bitmap.width & 0x0000FF00) >> 8;
    bmp_data[20] = (new_bitmap.width & 0x00FF0000) >> 16;
    bmp_data[21] = (new_bitmap.width & 0xFF000000) >> 24;

    // height
    bmp_data[22] = (new_bitmap.height & 0x000000FF);
    bmp_data[23] = (new_bitmap.height & 0x0000FF00) >> 8;
    bmp_data[24] = (new_bitmap.height & 0x00FF0000) >> 16;
    bmp_data[25] = (new_bitmap.height & 0xFF000000) >> 24;

    // Color planes - copied existing file
    bmp_data[26] = 1;
    bmp_data[27] = 0;

    // bits per pixel - traditional bmp value of 24, ignoring transparency
    bmp_data[28] = 24;
    bmp_data[29] = 0;

    // Compression method - 0 = none
    bmp_data[30] = 0;
    bmp_data[31] = 0;
    bmp_data[32] = 0;
    bmp_data[33] = 0;



    // Image size including row padding
    unsigned int image_size = bytesPerImageRow_padded * new_bitmap.height;
    bmp_data[34] = image_size & 0x000000FF;
    bmp_data[35] = (image_size & 0x0000FF00) >> 8;
    bmp_data[36] = (image_size & 0x00FF0000) >> 16;
    bmp_data[37] = (image_size & 0xFF000000) >> 24;

    // 96 DPI * 39.27
    unsigned int hori_res = 3780;
    bmp_data[38] = hori_res & 0x000000FF;
    bmp_data[39] = (hori_res & 0x0000FF00) >> 8;
    bmp_data[40] = (hori_res & 0x00FF0000) >> 16;
    bmp_data[41] = (hori_res & 0xFF000000) >> 24;

    // 96 DPI * 39.27
    unsigned int verti_res = 3780;
    bmp_data[42] = verti_res & 0x000000FF;
    bmp_data[43] = (verti_res & 0x0000FF00) >> 8;
    bmp_data[44] = (verti_res & 0x00FF0000) >> 16;
    bmp_data[45] = (verti_res & 0xFF000000) >> 24;

    // palette_color_count: 0 to default to 2n
    bmp_data[46] = 0;
    bmp_data[47] = 0;
    bmp_data[48] = 0;
    bmp_data[49] = 0;

    // important colors
    bmp_data[50] = 0;
    bmp_data[51] = 0;
    bmp_data[52] = 0;
    bmp_data[53] = 0;


    memcpy(&old_header, bmp_data.data(), BMP_Header_BITMAPINFOHEADER_size);


    // LOAD ACTUAL IMAGE DATA
    for (unsigned int y = 0; y < new_bitmap.height; y++) {
        for (unsigned int x = 0; x < new_bitmap.width; x++) {
            // Pixel location within raw BMP file buffer
            unsigned int pixelLocation = 0;
            pixelLocation += first_pixel_location;    // first pixel
            pixelLocation += bytesPerImageRow_padded * y;   // first pixel of row
            pixelLocation += x * bytesPerPixel;             // offset in row

            BitmapPixel px = new_bitmap.get_pixel(x, y);

            bmp_data[pixelLocation + 0] = px.B;
            bmp_data[pixelLocation + 1] = px.G;
            bmp_data[pixelLocation + 2] = px.R;
        }
    }

    return true;
}




void File::extract_header_BITMAPINFOHEADER() {

    // std::cout << "sizeof(BMP_BITMAPINFOHEADER_BIGEND) = " << sizeof(BMP_BITMAPINFOHEADER_BIGEND) << std::endl;


    // BMP HEADER
    // https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header

    old_header.BM = bmp_data[0] << 8;
    old_header.BM += bmp_data[1];

    old_header.file_size = bmp_data[2];
    old_header.file_size += bmp_data[3] << 8;
    old_header.file_size += bmp_data[4] << 16;
    old_header.file_size += bmp_data[5] << 24;

    old_header.reserved_1 = bmp_data[6];
    old_header.reserved_1 += bmp_data[7] << 8;

    old_header.reserved_2 = bmp_data[8];
    old_header.reserved_2 += bmp_data[9] << 8;

    old_header.first_pixel_location = bmp_data[10];
    old_header.first_pixel_location += bmp_data[11] << 8;
    old_header.first_pixel_location += bmp_data[12] << 16;
    old_header.first_pixel_location += bmp_data[13] << 24;



    // BITMAPINFOHEADER begins here

    old_header.header_size = bmp_data[14];
    old_header.header_size += bmp_data[15] << 8;
    old_header.header_size += bmp_data[16] << 16;
    old_header.header_size += bmp_data[17] << 24;

    old_header.width = bmp_data[18];
    old_header.width += bmp_data[19] << 8;
    old_header.width += bmp_data[20] << 16;
    old_header.width += bmp_data[21] << 24;

    old_header.height = bmp_data[22];
    old_header.height += bmp_data[23] << 8;
    old_header.height += bmp_data[24] << 16;
    old_header.height += bmp_data[25] << 24;

    old_header.color_planes = bmp_data[26];
    old_header.color_planes += bmp_data[27] << 8;

    old_header.bits_per_pixel = bmp_data[28];
    old_header.bits_per_pixel += bmp_data[29] << 8;

    old_header.compression_method = bmp_data[30];
    old_header.compression_method += bmp_data[31] << 8;
    old_header.compression_method += bmp_data[32] << 16;
    old_header.compression_method += bmp_data[33] << 24;

    old_header.imageSize = bmp_data[34];
    old_header.imageSize += bmp_data[35] << 8;
    old_header.imageSize += bmp_data[36] << 16;
    old_header.imageSize += bmp_data[37] << 24;

    old_header.horizontal_resolution = bmp_data[38];
    old_header.horizontal_resolution += bmp_data[39] << 8;
    old_header.horizontal_resolution += bmp_data[40] << 16;
    old_header.horizontal_resolution += bmp_data[41] << 24;

    old_header.vertical_resolution = bmp_data[42];
    old_header.vertical_resolution += bmp_data[43] << 8;
    old_header.vertical_resolution += bmp_data[44] << 16;
    old_header.vertical_resolution += bmp_data[45] << 24;

    old_header.palette_color_count = bmp_data[46];
    old_header.palette_color_count += bmp_data[47] << 8;
    old_header.palette_color_count += bmp_data[48] << 16;
    old_header.palette_color_count += bmp_data[49] << 24;

    old_header.important_color_count = bmp_data[50];
    old_header.important_color_count += bmp_data[51] << 8;
    old_header.important_color_count += bmp_data[52] << 16;
    old_header.important_color_count += bmp_data[53] << 24;


    // Sanity check
    if (old_header.file_size != bmp_data.size()) {
        std::cout << "[WARN] : BMP encoded size does not match buffer size read." << std::endl;
        std::cout << "header->file_size = " << old_header.file_size << std::endl;
        std::cout << "bmp_data.size() = " << bmp_data.size() << std::endl;
        std::cout << "" << std::endl;
    }

}




void File::print_header(){
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "BMP HEADER PRINT:" << std::endl;
    std::cout << "" << std::endl;
    
    std::cout << "BM                        =    \"" << (char)(old_header.BM >> 8) << (char) old_header.BM << "\"" << std::endl;
    std::cout << "file_size                 =    " << old_header.file_size                << "        [byte]" << std::endl;
    std::cout << "reserved_1                =    " << old_header.reserved_1               << std::endl;
    std::cout << "reserved_2                =    " << old_header.reserved_2               << std::endl;
    std::cout << "first_pixel_location      =    " << old_header.first_pixel_location     << "        [byte]" << std::endl;

    std::cout << "header_size               =    " << old_header.header_size              << "        [byte]" << std::endl;
    std::cout << "width                     =    " << old_header.width                    << std::endl;
    std::cout << "height                    =    " << old_header.height                   << std::endl;
    std::cout << "color_planes              =    " << old_header.color_planes             << std::endl;
    std::cout << "bits_per_pixel            =    " << old_header.bits_per_pixel           << std::endl;
    std::cout << "compression_method        =    " << old_header.compression_method       << std::endl;
    std::cout << "imageSize                 =    " << old_header.imageSize                << "        [byte]" << std::endl;
    std::cout << "horizontal_resolution     =    " << old_header.horizontal_resolution    << std::endl;
    std::cout << "vertical_resolution       =    " << old_header.vertical_resolution      << std::endl;
    std::cout << "palette_color_count       =    " << old_header.palette_color_count      << std::endl;
    std::cout << "important_color_count     =    " << old_header.important_color_count    << std::endl;

    std::cout << "" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
}

ui2 File::padded_data_indeces()
{
    return {    header.file_header.first_pixel_location, 
                header.file_header.file_size            };
}

unsigned char File::padded_data_size()
{
    return header.info_header.image_size;
}

unsigned char * File::padded_data()
{
    unsigned char* data = bmp_data.data();
    return (data + header.file_header.first_pixel_location);
}

std::vector<unsigned char>& File::get_image_data_raw()
{
    return bmp_data;
}

}