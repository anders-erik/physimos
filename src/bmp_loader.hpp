#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class BMP_loader
{
public:
    BMP_loader() {};

    // loaded file
    std::vector<unsigned char> rawBmpFileData;
    
    std::string imagePath;
    unsigned int firstPixelLocation;
    unsigned int width;
    unsigned int height;
    unsigned int imageSize;
    unsigned int bitsPerPixel;

    // 24 bit rgb value buffer
    std::vector<unsigned char> imageDataBuffer;

    void loadBMPFile(std::string path) {
        std::cout << "BMP file: " << path << ". ";
        imagePath = path;


        rawBmpFileData.clear();
        imageDataBuffer.clear();
        

        const std::size_t nbytes = 40000;
        std::vector<char> buff(nbytes); // allocate a buffer of nbytes characters


        std::ifstream file(path, std::ios::binary);

        // Check if the file was opened successfully
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << path << std::endl;
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


        for (char byte : fileData) {
            rawBmpFileData.push_back(byte);
        }

        
        firstPixelLocation = rawBmpFileData[10];
        firstPixelLocation += rawBmpFileData[11] << 8;
        firstPixelLocation += rawBmpFileData[12] << 8;
        firstPixelLocation += rawBmpFileData[13] << 8;
        
        width = rawBmpFileData[18];
        width += rawBmpFileData[19] << 8;
        width += rawBmpFileData[20] << 8;
        width += rawBmpFileData[21] << 8;

        height = rawBmpFileData[22];
        height += rawBmpFileData[23] << 8;
        height += rawBmpFileData[24] << 8;
        height += rawBmpFileData[25] << 8;

        bitsPerPixel = rawBmpFileData[28];
        bitsPerPixel += rawBmpFileData[29] << 8;


        // std::cout << "first pixel @ " << int(buff[10]) << std::endl;

        

        // Move into imageDataBuffer
        int byteCount = 0;
        // forced to manually set image size
        for (std::size_t i = firstPixelLocation; i < imageSize + firstPixelLocation; i+=3) {
            byteCount++;
            // BMP all pixel-'words' are stored in little endian format, so we reverse order here
            imageDataBuffer.push_back(rawBmpFileData[i+2]);
            imageDataBuffer.push_back(rawBmpFileData[i+1]);
            imageDataBuffer.push_back(rawBmpFileData[i]);
        }
        // std::cout << "byteCount: " << byteCount << std::endl;

        file.close();

    }



    void prettyPrint(){
        std::cout << "--------BMP PRINT-------" << std::endl;
        std::cout << "imagePath     : " << imagePath << std::endl;
        std::cout << "size (B)      : " << imageSize << std::endl;
        std::cout << "First pixel @ : " << firstPixelLocation << std::endl;
        std::cout << "width         : " << width << std::endl;
        std::cout << "height        : " << height << std::endl;
        std::cout << "------------------------" << std::endl;
    }

};


