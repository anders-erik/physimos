
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "png_loader.hpp"

namespace png {


    std::vector<unsigned char> fileData;

    std::string imagePath;

    unsigned int imageSize;

    unsigned int bitsPerPixel;

    // raw image data
    std::vector<unsigned char> imageDataBuffer;


    // IHDR DATA
    unsigned int width;
    unsigned int height;
    unsigned int bitDepth;
    unsigned int colorType;
    unsigned int compression;
    unsigned int filter;
    unsigned int interlace;

    // IDAT chunk
    int idat_size = 0;
    int deflate = 0;
    int zlib_fcheck = 0;
    std::vector<unsigned int> imgData_chunk;
    int zlib_check_value = 0;
    int crc = 0;


    std::vector<unsigned char> getImageDataBuffer() {
        return imageDataBuffer;
    }
    unsigned int getWidth() {
        return width;
    }
    unsigned int getHeight() {
        return height;
    }



    void loadFile(std::string filePath){
        std::cout << "LOADING PNG : " << "\"" << filePath << "\"" << std::endl;
        
        std::cout << "BMP file: " << filePath << ". ";
        imagePath = filePath;


        fileData.clear();
        imageDataBuffer.clear();


        const std::size_t nbytes = 40000;
        std::vector<char> buff(nbytes); // allocate a buffer of nbytes characters


        std::ifstream file(filePath, std::ios::binary);

        // Check if the file was opened successfully
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filePath << std::endl;
            return;
        }

        // Determine the size of the file
        file.seekg(0, std::ios::end);
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        imageSize = (unsigned int)fileSize;

        std::cout << "Filesize: " << fileSize << std::endl;

        // Create a vector to hold the file data
        std::vector<unsigned char> fileData(fileSize);

        if (!file.read(reinterpret_cast<char*>(fileData.data()), fileSize)) {
            std::cerr << "Error: Could not read file data" << std::endl;
            return;
        }

        file.close();

        // for (char byte : fileData) {
        //     fileData.push_back(byte);
        // }


        // PNG HEADER
        //https://en.wikipedia.org/wiki/PNG#File_header

        // FIND 'IHDR' and extract the metadata
        for (int i = 0; i < fileSize; i++){
            if (fileData[i] == 'I' && fileData[i+1] == 'H' && fileData[i+2] == 'D' && fileData[i+3] == 'R'){
                std::cout << "Match at i = " << i << std::endl;

                int offs = i + 4;

                width  = fileData[offs + 0] << 24;
                width += fileData[offs + 1] << 16;
                width += fileData[offs + 2] << 8;
                width += fileData[offs + 3] << 0;
                // std::cout << "width = " << width << std::endl;

                height  = fileData[offs + 4] << 24;
                height += fileData[offs + 5] << 16;
                height += fileData[offs + 6] << 8;
                height += fileData[offs + 7] << 0;
                // std::cout << "height = " << height << std::endl;

                bitDepth = fileData[offs + 8];
                colorType = fileData[offs + 9];
                compression = fileData[offs + 10];
                filter = fileData[offs + 11];
                interlace = fileData[offs + 12];
                
                break;
            }

        }


        // FIND 'IDAT'
        // https://en.wikipedia.org/wiki/PNG#Examples
        

        for (int i = 0; i < fileSize; i++) {
            if (fileData[i] == 'I' && fileData[i + 1] == 'D' && fileData[i + 2] == 'A' && fileData[i + 3] == 'T') {
                std::cout << "Match of IDAT at i = " << i << std::endl;


                idat_size += fileData[i - 4] << 24;
                idat_size += fileData[i - 3] << 16;
                idat_size += fileData[i - 2] << 8;
                idat_size += fileData[i - 1] << 0;
                // std::cout << "width = " << width << std::endl;
                

                deflate     = fileData[i + 4];
                // std::cout << "" << deflate << std::endl;
                
                zlib_fcheck = fileData[i + 5];
                
                // add the chunk to imgData_chunk
                for (int j = i+6; j < i + 4 + idat_size - 8; j++){
                    imgData_chunk.push_back(fileData[j]);
                }

                zlib_check_value += fileData[i + idat_size - 7] << 24;
                zlib_check_value += fileData[i + idat_size - 6] << 16;
                zlib_check_value += fileData[i + idat_size - 5] << 8;
                zlib_check_value += fileData[i + idat_size - 4] << 0;

                crc += fileData[i + idat_size - 3] << 24;
                crc += fileData[i + idat_size - 2] << 16;
                crc += fileData[i + idat_size - 1] << 8;
                crc += fileData[i + idat_size - 0] << 0;


                // height += fileData[offs + 2] << 8;
                // height += fileData[offs + 3] << 0;
                // // std::cout << "height = " << height << std::endl;

                // bitDepth = fileData[offs + 8];
                // colorType = fileData[offs + 9];
                // compression = fileData[offs + 10];
                // filter = fileData[offs + 11];
                // interlace = fileData[offs + 12];

                break;
            }

        }



        // firstPixelLocation = rawBmpFileData[10];
        // firstPixelLocation += rawBmpFileData[11] << 8;
        // firstPixelLocation += rawBmpFileData[12] << 8;
        // firstPixelLocation += rawBmpFileData[13] << 8;

        // width = rawBmpFileData[18];
        // width += rawBmpFileData[19] << 8;
        // width += rawBmpFileData[20] << 8;
        // width += rawBmpFileData[21] << 8;

        // height = rawBmpFileData[22];
        // height += rawBmpFileData[23] << 8;
        // height += rawBmpFileData[24] << 8;
        // height += rawBmpFileData[25] << 8;

        // bitsPerPixel = rawBmpFileData[28];
        // bitsPerPixel += rawBmpFileData[29] << 8;


        // std::cout << "first pixel @ " << int(buff[10]) << std::endl;



        // // Move into imageDataBuffer
        // int byteCount = 0;
        // // forced to manually set image size
        // for (std::size_t i = firstPixelLocation; i < imageSize + firstPixelLocation; i += 3) {
        //     byteCount++;
        //     // BMP all pixel-'words' are stored in little endian format, so we reverse order here
        //     imageDataBuffer.push_back(rawBmpFileData[i + 2]);
        //     imageDataBuffer.push_back(rawBmpFileData[i + 1]);
        //     imageDataBuffer.push_back(rawBmpFileData[i]);
        // }
        // // std::cout << "byteCount: " << byteCount << std::endl;

        // file.close();

    }



    void prettyPrint() {
        std::cout << "-------- PNG PRINT -------" << std::endl;
        std::cout << "imagePath     : " << imagePath << std::endl;
        // IHDR
        std::cout << "IHDR : " << std::endl;
        std::cout << "width         : " << width << std::endl;
        std::cout << "height        : " << height << std::endl;
        std::cout << "bitDepth      : " << bitDepth << std::endl;
        std::cout << "colorType     : " << colorType << std::endl;
        std::cout << "compression   : " << compression << std::endl;
        std::cout << "filter        : " << filter << std::endl;
        std::cout << "interlace     : " << interlace << std::endl;

        std::cout << std::endl;
        std::cout << "IDAT : " << std::endl;
        std::cout << "idat_size                 : " << idat_size << std::endl;
        std::cout << "deflate                   : " << deflate << std::endl;
        std::cout << "zlib_fcheck               : " << zlib_fcheck << std::endl;
        std::cout << "imgData_chunk.size()      : " << imgData_chunk.size() << std::endl;
        std::cout << "zlib_check_value          : " << zlib_check_value << std::endl;
        std::cout << "crc                       : " << crc << std::endl;
        
        std::cout << std::endl;
        std::cout << "size (B)      : " << imageSize << std::endl;
        
        std::cout << "--------------------------" << std::endl;
    }

}

