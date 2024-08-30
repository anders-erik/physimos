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
    unsigned int firstPixelLocation;
    // unsigned char rawFileBuffer[100000];
    // 24 bit rgb value buffer
    std::vector<unsigned char> imageDataBuffer;

    void loadBMPFile(const char* path) {
        std::cout << "BMP file: " << path << ". ";

        // https://cplusplus.com/forum/general/276214/
        
        using byte = std::uint8_t;

        const std::size_t nbytes = 40000;
        std::vector<char> buff(nbytes); // allocate a buffer of nbytes characters

        // try to open the file for input, in binary mode
        if (std::ifstream file{ path, std::ios::binary })
        {
            
            
            if (file.read(buff.data(), buff.size())) // try to read in nbytes
            {
                const auto nread = file.gcount(); // number of bytes that were actually read

                // from the characters that were read, initialise a vector of nbytes bytes
                std::vector<byte> bytes(buff.begin(), buff.begin() + nread);

                // print out the values of the bytes (two hex digits per byte)
                // std::cout << "the first " << nbytes << " bytes in the file '"
                //     << path << "' are hex:\n";
                // for (byte b : bytes) std::cout << std::hex << int(b) << ' ';
                // std::cout << '\n';
            }
        }

        for(char byte : buff){
            rawBmpFileData.push_back(byte);
        }

        firstPixelLocation = rawBmpFileData[10];

        std::cout << "File done. " << std::endl;

        // std::cout << "first pixel @ " << int(buff[10]) << std::endl;

        // assume pixel first pixel is not found after byte # 256!
        if (firstPixelLocation == 138) {
            std::cout << "first pixel @ " << firstPixelLocation << std::endl;
        }
        else {
            std::cout << "first pixel location is WRONG!! " << " (" << __FILE__ << "::" << __LINE__ << ")" << std::endl;
        }

        // Move into imageDataBuffer
        int byteCount = 0;
        // forced to manually set image size
        for (std::size_t i = firstPixelLocation; i < 30000 + firstPixelLocation; i++) {
            byteCount++;
            imageDataBuffer.push_back(rawBmpFileData[i]);
        }
        std::cout << "byteCount: " << byteCount << std::endl;

        // std::cout << "reading file: " << path << ". ";
        // // this->modelPath = path;

        // std::ifstream imageFile;
        // std::stringstream modelStream;
        // std::string modelString;

        // imageFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        // try
        // {

        //     imageFile.open(path, std::ios_base::openmode::_S_bin);

        //     if (imageFile.read(&rawFileBuffer, sizeof(unsigned char)));


        // }
        // catch (std::ifstream::failure& e)
        // {
        //     std::cout << "ERROR::READING_BMP_FILE" << e.what() << std::endl;
        // }

    }
};


