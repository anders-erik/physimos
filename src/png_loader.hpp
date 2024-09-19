#ifndef PNG_LOADER_HPP
#define PNG_LOADER_HPP

#include <string>

namespace png {

    void loadFile(std::string filePath);

    std::vector<unsigned char> getImageDataBuffer();
    unsigned int getWidth();
    unsigned int getHeight();

    void prettyPrint();
}


#endif