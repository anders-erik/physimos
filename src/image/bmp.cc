
#include "bmp.hh"

#include <iostream>
#include <fstream>

namespace pimage {

Bitmap::Bitmap(unsigned char data, unsigned int size){

}
Bitmap::Bitmap(unsigned int size) {

}

}




namespace pimage::loaders {


::pimage::loaders::BMP_Result BMP_load(std::filesystem::path filePath) {

try {

    std::ifstream file(filePath, std::ios::binary);
    char s[100];
    std::streamsize ss = 100;
    file.read(s, ss);
    return BMP_Result{ nullptr, LoadStatus::Ok };
    
}
catch(const std::exception& e){

    std::cerr << e.what() << '\n';

    return BMP_Result {nullptr, LoadStatus::ErrorOpeningFile };
}

}


}