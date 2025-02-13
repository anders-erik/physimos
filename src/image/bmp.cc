#include <stdlib.h> 

#include <iostream>
#include <fstream>
#include <vector>

#include "lib/fs.hh"

#include "bmp.hh"


namespace pimage {

Bitmap::Bitmap(unsigned int height, unsigned int width) {
    // const unsigned int newBitmapSize = height * width * 4;
    // this->size = newBitmapSize;
    // this->data = (unsigned char*)malloc(newBitmapSize * sizeof(unsigned char));
    

    this->height = height;
    this->width = width;
    
    // this->pixels = (Pixel*) this->data;
    this->pixelcount = height * width;
    this->pixels = std::vector<pimage::Pixel> (this->pixelcount);
    // std::cout << "" << var << std::endl;

    // this->size = newBitmapSize;
    // this->data = pixels.data();
    
}
Bitmap::~Bitmap(){
    // std::cout << "      ~Bitmap()" << std::endl;
}


void Bitmap::set_pixel(unsigned int x, unsigned int y, Pixel pixel) {

    unsigned int pixelIndex = this->width * y + x;

    this->pixels[pixelIndex] = pixel;

}

Pixel Bitmap::get_pixel(unsigned int x, unsigned int y) {

    unsigned int pixelIndex = this->width * y + x;

    return this->pixels[pixelIndex];
}


}



namespace pimage::io {

BMP::BMP(){
    header = new BMP_Header_BITMAPINFOHEADER();
    header->first_pixel_location = 0;
    header->width = 0;
    header->height = 0;
    header->imageSize = 0;
    header->bits_per_pixel = 0;

    bitmap = nullptr;
}



// Currently 'BITMAPINFOHEADER' is the only header that I support
// Header formats : https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header
BMP_Header_BITMAPINFOHEADER* BMP::extract_header_BITMAPINFOHEADER(std::vector<unsigned char>& bmp_data) {

    // std::cout << "sizeof(BMP_Header_BITMAPINFOHEADER) = " << sizeof(BMP_Header_BITMAPINFOHEADER) << std::endl;

    // BMP HEADER
    // https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header

    header->BM = bmp_data[0] << 8;
    header->BM += bmp_data[1];

    header->file_size = bmp_data[2];
    header->file_size += bmp_data[3] << 8;
    header->file_size += bmp_data[4] << 8;
    header->file_size += bmp_data[5] << 8;

    header->reserved_1 = bmp_data[6];
    header->reserved_1 += bmp_data[7] << 8;

    header->reserved_2 = bmp_data[8];
    header->reserved_2 += bmp_data[9] << 8;

    header->first_pixel_location = bmp_data[10];
    header->first_pixel_location += bmp_data[11] << 8;
    header->first_pixel_location += bmp_data[12] << 8;
    header->first_pixel_location += bmp_data[13] << 8;



    // BITMAPINFOHEADER begins here

    header->header_size = bmp_data[14];
    header->header_size += bmp_data[15] << 8;
    header->header_size += bmp_data[16] << 8;
    header->header_size += bmp_data[17] << 8;

    header->width = bmp_data[18];
    header->width += bmp_data[19] << 8;
    header->width += bmp_data[20] << 8;
    header->width += bmp_data[21] << 8;

    header->height = bmp_data[22];
    header->height += bmp_data[23] << 8;
    header->height += bmp_data[24] << 8;
    header->height += bmp_data[25] << 8;

    header->color_planes = bmp_data[26];
    header->color_planes += bmp_data[27] << 8;

    header->bits_per_pixel = bmp_data[28];
    header->bits_per_pixel += bmp_data[29] << 8;

    header->compression_method = bmp_data[30];
    header->compression_method += bmp_data[31] << 8;
    header->compression_method += bmp_data[32] << 8;
    header->compression_method += bmp_data[33] << 8;

    header->imageSize = bmp_data[34];
    header->imageSize += bmp_data[35] << 8;
    header->imageSize += bmp_data[36] << 8;
    header->imageSize += bmp_data[37] << 8;

    header->horizontal_resolution = bmp_data[38];
    header->horizontal_resolution += bmp_data[39] << 8;
    header->horizontal_resolution += bmp_data[40] << 8;
    header->horizontal_resolution += bmp_data[41] << 8;

    header->vertical_resolution = bmp_data[42];
    header->vertical_resolution += bmp_data[43] << 8;
    header->vertical_resolution += bmp_data[44] << 8;
    header->vertical_resolution += bmp_data[45] << 8;

    header->palette_color_count = bmp_data[46];
    header->palette_color_count += bmp_data[47] << 8;
    header->palette_color_count += bmp_data[48] << 8;
    header->palette_color_count += bmp_data[49] << 8;

    header->important_color_count = bmp_data[50];
    header->important_color_count += bmp_data[51] << 8;
    header->important_color_count += bmp_data[52] << 8;
    header->important_color_count += bmp_data[53] << 8;


    // Sanity check
    if (header->file_size != bmp_data.size()) {
        std::cout << "[WARN] : BMP encoded size does not match buffer size read." << std::endl;
        std::cout << "header->file_size = " << header->file_size << std::endl;
        std::cout << "bmp_data.size() = " << bmp_data.size() << std::endl;
        std::cout << "" << std::endl;
    }

    return header;
}


void BMP::load_header_BITMAPINFOHEADER() {

}

unsigned int BMP_Header_BITMAPINFOHEADER_size = 54;
bool BMP::save(std::filesystem::path filePath) {
    // Need to set bitmap pointer before saving it!
    if(bitmap == nullptr){
        return false;
    }


    // Utility data
    bytesPerPixel = 3;
    bytesPerImageRow = bytesPerPixel * bitmap->width;
    bytesPerImageRow_padded = bytesPerImageRow;
    if (bytesPerPixel != 4) { // Each row is padded to 4-byte alignment
        unsigned int paddingSize = 4 - (bytesPerImageRow % 4);
        bytesPerImageRow_padded = bytesPerImageRow + paddingSize;
    }


    unsigned int output_file_size = BMP_Header_BITMAPINFOHEADER_size + bitmap->height * bytesPerImageRow_padded;
    std::vector<unsigned char> bmp_data(output_file_size);

    

    // load_header_BITMAPINFOHEADER();
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
    bmp_data[18] = (bitmap->width & 0x000000FF);
    bmp_data[19] = (bitmap->width & 0x0000FF00) >> 8;
    bmp_data[20] = (bitmap->width & 0x00FF0000) >> 16;
    bmp_data[21] = (bitmap->width & 0xFF000000) >> 24;

    // height
    bmp_data[22] = (bitmap->height & 0x000000FF);
    bmp_data[23] = (bitmap->height & 0x0000FF00) >> 8;
    bmp_data[24] = (bitmap->height & 0x00FF0000) >> 16;
    bmp_data[25] = (bitmap->height & 0xFF000000) >> 24;

    // Color planes - copied existing file
    bmp_data[26] = 1;
    bmp_data[27] = 0;

    // bits per pixel - let's use the traditional bmp value, ignoring transparency
    bmp_data[28] = 24;
    bmp_data[29] = 0;

    // Compression method - 0 = none
    bmp_data[30] = 0;
    bmp_data[31] = 0;
    bmp_data[32] = 0;
    bmp_data[33] = 0;



    // Image size including row padding
    unsigned int image_size = bytesPerImageRow_padded * bitmap->height;
    bmp_data[34] =  image_size & 0x000000FF;
    bmp_data[35] = (image_size & 0x0000FF00) >> 8;
    bmp_data[36] = (image_size & 0x00FF0000) >> 16;
    bmp_data[37] = (image_size & 0xFF000000) >> 24;

    // 96 DPI * 39.27
    unsigned int hori_res = 3780;
    bmp_data[38] =  hori_res & 0x000000FF;
    bmp_data[39] = (hori_res & 0x0000FF00) >> 8 ;
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


    // LOAD ACTUAL IMAGE DATA
    for(unsigned int y = 0; y < bitmap->height; y++){
        for (unsigned int x = 0; x < bitmap->height; x++) {
            // Pixel location within raw BMP file buffer
            unsigned int pixelLocation = 0;
            pixelLocation += first_pixel_location;    // first pixel
            pixelLocation += bytesPerImageRow_padded * y;   // first pixel of row
            pixelLocation += x * bytesPerPixel;             // offset in row

            Pixel px = bitmap->get_pixel(x, y);

            bmp_data[pixelLocation+0] = px.B;
            bmp_data[pixelLocation+1] = px.G;
            bmp_data[pixelLocation+2] = px.R;
        }
    }

    
    plib::FS_STATUS write_status = plib::fs_echo_bin(filePath.string(), bmp_data);
    if (write_status == plib::FsError){
        std::cout << "EROROROROROOR"  << std::endl;
        return false;
    }
        
    return true;
}


::pimage::Bitmap* BMP::load(std::filesystem::path filePath) {


    std::vector<unsigned char> bmp_data = plib::fs_cat_bin(filePath.string());
    if (bmp_data.size() < 54){ // header size
        return nullptr;
    }
    

    extract_header_BITMAPINFOHEADER(bmp_data);

    

    // Image Utility Dimensions
    bytesPerPixel = header->bits_per_pixel / 8;
    bytesPerImageRow = bytesPerPixel * header->width; 
    bytesPerImageRow_padded = bytesPerImageRow;
    
    if (bytesPerPixel != 4) { // Each row is padded to 4-byte alignment
        unsigned int paddingSize = 4 - (bytesPerImageRow % 4);
        bytesPerImageRow_padded = bytesPerImageRow + paddingSize;
    }



    // Create Physimos RGBA Bitmap and copy bmp file data into it
    bitmap = new Bitmap(header->width, header->height);

    // NOTE: very much NOT optimized for performance
    // 
    for (unsigned int y = 0; y < header->height; y++) {

        for (unsigned int x = 0; x < header->width; x++){

            // Pixel location within raw BMP file buffer
            unsigned int pixelLocation = 0;
            pixelLocation += header->first_pixel_location;    // first pixel
            pixelLocation += bytesPerImageRow_padded * y;   // first pixel of row
            pixelLocation += x * bytesPerPixel;             // offset in row
            
            Pixel newPixel = Pixel {
                bmp_data[pixelLocation + 2],
                bmp_data[pixelLocation + 1],
                bmp_data[pixelLocation + 0],
                255, // No transparancy by default
            };

            bitmap->set_pixel(x, y, newPixel);
        }

    }

    return bitmap;

}

void BMP::print_header(){
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "BMP HEADER PRINT:" << std::endl;
    std::cout << "" << std::endl;
    
    std::cout << "BM                        =    \"" << (char)(header->BM >> 8) << (char) header->BM << "\"" << std::endl;
    std::cout << "file_size                 =    " << header->file_size                << "        [byte]" << std::endl;
    std::cout << "reserved_1                =    " << header->reserved_1               << std::endl;
    std::cout << "reserved_2                =    " << header->reserved_2               << std::endl;
    std::cout << "first_pixel_location      =    " << header->first_pixel_location     << "        [byte]" << std::endl;

    std::cout << "header_size               =    " << header->header_size              << "        [byte]" << std::endl;
    std::cout << "width                     =    " << header->width                    << std::endl;
    std::cout << "height                    =    " << header->height                   << std::endl;
    std::cout << "color_planes              =    " << header->color_planes             << std::endl;
    std::cout << "bits_per_pixel            =    " << header->bits_per_pixel           << std::endl;
    std::cout << "compression_method        =    " << header->compression_method       << std::endl;
    std::cout << "imageSize                 =    " << header->imageSize                << "        [byte]" << std::endl;
    std::cout << "horizontal_resolution     =    " << header->horizontal_resolution    << std::endl;
    std::cout << "vertical_resolution       =    " << header->vertical_resolution      << std::endl;
    std::cout << "palette_color_count       =    " << header->palette_color_count      << std::endl;
    std::cout << "important_color_count     =    " << header->important_color_count    << std::endl;

    std::cout << "" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
}


}