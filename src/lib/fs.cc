
#include <string>

#include <iostream>

#include <fstream>
#include <streambuf>
#include <sstream>


#include "fs.hh"

namespace plib {

std::string fs_cat(std::string path_std_string){
    std::string string;

    try
    {
        std::ifstream _ifstream(path_std_string);
        std::stringstream buffer;
        buffer << _ifstream.rdbuf();
        string = buffer.str();
    }
    catch(const std::exception& e)
    {
        string = std::string("");
    }
    
    return string;
}

// Creates a new char vector with contents equal to file contents. Empty vector on failed read.
std::vector<unsigned char>& fs_cat_bin(std::string path_std_string){
    std::vector<unsigned char>* returnVector;

    try
    {
        std::ifstream _ifstream(path_std_string, std::ios::binary);
        if (!_ifstream.is_open()) {
            // std::cerr << "Error: Could not open file " << filePath << std::endl;
            returnVector = new std::vector<unsigned char> {};
            return *returnVector;
        }

        returnVector = new std::vector<unsigned char>(std::istreambuf_iterator<char>(_ifstream), {});
        
        _ifstream.close();
        return *returnVector;
    }
    catch (const std::exception& e)
    {
        returnVector = new std::vector<unsigned char> {};
        return *returnVector;
    }

    // Alternative read and check used by old BMP loader
    // if (!file.read(reinterpret_cast<char*>(fileData.data()), fileStreamSize)) {
    //     std::cerr << "Error: Could not read file data" << std::endl;
    //     return nullptr;
    // }

}




FS_STATUS fs_echo(std::string path_std_string, std::string contents){

    std::ofstream _ofstream;

    try
    {
        _ofstream.open(path_std_string);
        _ofstream << contents;
        _ofstream.close();
    }
    catch(const std::exception& e)
    {
        return FS_STATUS::FsError;
    }
    
    

    return FS_STATUS::FsOk;
}


FS_STATUS fs_echo_bin(std::string path_std_string, std::vector<unsigned char>& bin_contents){

    std::ofstream _ofstream;

    try
    {
        _ofstream.open(path_std_string, std::ios::binary);
        // reinterpret_cast<const char*>(v1.data())
        _ofstream.write(reinterpret_cast<const char*>(bin_contents.data()), bin_contents.size());
        // _ofstream.write(bin_contents.data(), bin_contents.size());
        _ofstream.close();

        return FsOk;

    }
    catch(const std::exception& e)
    {
        return FsError;
    }


}


}
