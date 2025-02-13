#include <stdlib.h> 

#include <iostream>
#include <fstream>
#include <vector>


#include "bmp.hh"


namespace pimage {

// NO LONGER UPDATED
Bitmap::Bitmap(unsigned char* data, unsigned int dataSize, unsigned int height, unsigned int width, unsigned char bytesPerPixel) {

    // Data checks
    if (dataSize % bytesPerPixel != 0) {
        std::cout << "Bitmap data is not aligned to bytesPerPixel = " << bytesPerPixel << std::endl;
    }
    if (height * width * bytesPerPixel > dataSize) {
        std::cout << "New bitmap data not consistent:  height * width * bytesPerPixel > dataSize "<< std::endl;
        std::cout << "height * width * bytesPerPixel = " << height * width * bytesPerPixel << std::endl;
        std::cout << "dataSize =                       " << dataSize << std::endl;
    }

    this->data = data;
    this->size = dataSize;

    
    this->pixels = (pimage::Pixel*)data;
    
}

Bitmap::Bitmap(unsigned int height, unsigned int width) {
    const unsigned int newBitmapSize = height * width * 4;
    this->size = newBitmapSize;
    this->data = (unsigned char*)malloc(newBitmapSize * sizeof(unsigned char));

    this->height = height;
    this->width = width;
    
    this->pixels = (Pixel*) this->data;
    this->pixelcount = height * width;
}
Bitmap::~Bitmap(){
    std::cout << "      ~Bitmap()" << std::endl;
    
    // this->pixels = nullptr;
    // free(this->data);
}


void Bitmap::set_pixel(unsigned int x, unsigned int y, Pixel pixel) {

    unsigned int pixelIndex = this->width * 4 * y + 4 * x;

    data[pixelIndex + 0] = pixel.R;
    data[pixelIndex + 1] = pixel.G;
    data[pixelIndex + 2] = pixel.B;
    data[pixelIndex + 3] = pixel.A;

}

Pixel Bitmap::set_pixel(unsigned int x, unsigned int y) {

    unsigned int pixelIndex = this->width * 4 * y + 4 * x;

    return Pixel{   data[pixelIndex + 0], 
                    data[pixelIndex + 1], 
                    data[pixelIndex + 2],
                    data[pixelIndex + 3],
    };
}


}



namespace pimage::io {

BMP::BMP(){
    header = new BMP_Header();
    header->first_pixel_location = 0;
    header->width = 0;
    header->height = 0;
    header->imageSize = 0;
    header->bits_per_pixel = 0;
}


bool save(std::filesystem::path filePath){
    
}


::pimage::Bitmap* BMP::load(std::filesystem::path filePath) {


    // FILE OPERATIONS
    std::ifstream file(filePath, std::ios::binary);

    // Check if the file was opened successfully
    if (!file.is_open()) {
        // std::cerr << "Error: Could not open file " << filePath << std::endl;
        return nullptr;
    }

    // Determine the size of the file
    file.seekg(0, std::ios::end);
    std::streamsize fileStreamSize = file.tellg();
    file.seekg(0, std::ios::beg);
    // unsigned int file_size = (unsigned int)fileStreamSize;

    // Create a vector to hold the file data
    std::vector<unsigned char> fileData(fileStreamSize);

    if (!file.read(reinterpret_cast<char*>(fileData.data()), fileStreamSize)) {
        std::cerr << "Error: Could not read file data" << std::endl;
        return nullptr;
    }

    file.close();

    // Converts the ssize_t of "std::streamsize" to string for logging
    std::stringstream sizeStringStream;
    sizeStringStream << fileStreamSize;
    std::string sizeString = sizeStringStream.str();




    // BUFFERS
    std::vector<unsigned char> rawBmpFileData;
    for (char byte : fileData) {
        rawBmpFileData.push_back(byte);
    }

    // 24 bit rgb value buffer
    // std::vector<unsigned char> imageDataBuffer;

    // rawBmpFileData.clear();
    // imageDataBuffer.clear();

    std::cout << "sizeof(BMP_Header) = " << sizeof(BMP_Header) << std::endl;
    

    // BMP HEADER
    // https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header

    header->BM = rawBmpFileData[0] << 8;
    header->BM += rawBmpFileData[1];
    
    header->file_size  = rawBmpFileData[2];
    header->file_size += rawBmpFileData[3] << 8;
    header->file_size += rawBmpFileData[4] << 8;
    header->file_size += rawBmpFileData[5] << 8;

    header->reserved_1 = rawBmpFileData[6];
    header->reserved_1 += rawBmpFileData[7] << 8;

    header->reserved_2 = rawBmpFileData[8];
    header->reserved_2 += rawBmpFileData[9] << 8;

    header->first_pixel_location = rawBmpFileData[10];
    header->first_pixel_location += rawBmpFileData[11] << 8;
    header->first_pixel_location += rawBmpFileData[12] << 8;
    header->first_pixel_location += rawBmpFileData[13] << 8;



    // BITMAPINFOHEADER begins here

    header->header_size =  rawBmpFileData[14];
    header->header_size += rawBmpFileData[15] << 8;
    header->header_size += rawBmpFileData[16] << 8;
    header->header_size += rawBmpFileData[17] << 8;

    header->width = rawBmpFileData[18];
    header->width += rawBmpFileData[19] << 8;
    header->width += rawBmpFileData[20] << 8;
    header->width += rawBmpFileData[21] << 8;

    header->height = rawBmpFileData[22];
    header->height += rawBmpFileData[23] << 8;
    header->height += rawBmpFileData[24] << 8;
    header->height += rawBmpFileData[25] << 8;

    header->color_planes = rawBmpFileData[26];
    header->color_planes += rawBmpFileData[27] << 8;

    header->bits_per_pixel = rawBmpFileData[28];
    header->bits_per_pixel += rawBmpFileData[29] << 8;

    header->compression_method =  rawBmpFileData[30];
    header->compression_method += rawBmpFileData[31] << 8;
    header->compression_method += rawBmpFileData[32] << 8;
    header->compression_method += rawBmpFileData[33] << 8;

    header->imageSize =  rawBmpFileData[34];
    header->imageSize += rawBmpFileData[35] << 8;
    header->imageSize += rawBmpFileData[36] << 8;
    header->imageSize += rawBmpFileData[37] << 8;

    header->horizontal_resolution =  rawBmpFileData[38];
    header->horizontal_resolution += rawBmpFileData[39] << 8;
    header->horizontal_resolution += rawBmpFileData[40] << 8;
    header->horizontal_resolution += rawBmpFileData[41] << 8;

    header->vertical_resolution =  rawBmpFileData[42];
    header->vertical_resolution += rawBmpFileData[43] << 8;
    header->vertical_resolution += rawBmpFileData[44] << 8;
    header->vertical_resolution += rawBmpFileData[45] << 8;

    header->palette_color_count =  rawBmpFileData[46];
    header->palette_color_count += rawBmpFileData[47] << 8;
    header->palette_color_count += rawBmpFileData[48] << 8;
    header->palette_color_count += rawBmpFileData[49] << 8;

    header->important_color_count =  rawBmpFileData[50];
    header->important_color_count += rawBmpFileData[51] << 8;
    header->important_color_count += rawBmpFileData[52] << 8;
    header->important_color_count += rawBmpFileData[53] << 8;



    // CALCULATED IMAGE SIZE
    // header->imageSize = (unsigned int)fileStreamSize - header->first_pixel_location;

    this->bytesPerPixel = header->bits_per_pixel / 8;
    bytesPerImageRow = bytesPerPixel * header->width;
    bytesPerImageRow_padded = bytesPerImageRow;
    // Each row is padded to 4-byte alignment
    if (bytesPerPixel != 4){
        unsigned int paddingSize = 4 - (bytesPerImageRow % 4);
        bytesPerImageRow_padded = bytesPerImageRow + paddingSize;
    }

    // std::cout << "file_size = " << file_size << std::endl;
    // std::cout << "header->first_pixel_location = " << header->first_pixel_location << std::endl;
    // std::cout << "header->imageSize = " << header->imageSize << std::endl;
    // std::cout << "header->bits_per_pixel = " << header->bits_per_pixel << std::endl;
    // std::cout << "bytesPerPixel = " << bytesPerPixel << std::endl;
    // std::cout << "bytesPerImageRow = " << bytesPerImageRow << std::endl;
    // std::cout << "bytesPerImageRow_padded = " << bytesPerImageRow_padded << std::endl;


    //  BITMAP
    bitmap = new Bitmap(header->width, header->height);

    // loop through pixels
    // NOTE: very much NOT optimized for performance
    for (unsigned int y = 0; y < header->height; y++) {

        for (unsigned int x = 0; x < header->width; x++){

            // Pixel location within raw BMP file buffer
            unsigned int pixelLocation = 0;
            pixelLocation += header->first_pixel_location;    // first pixel
            pixelLocation += bytesPerImageRow_padded * y;   // first pixel of row
            pixelLocation += x * bytesPerPixel;             // offset in row
            
            Pixel newPixel = Pixel {
                rawBmpFileData[pixelLocation + 2],
                rawBmpFileData[pixelLocation + 1],
                rawBmpFileData[pixelLocation + 0],
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