
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

std::vector<unsigned char> fs_cat_bin(std::string path_std_string){

    try
    {
        std::ifstream _ifstream(path_std_string, std::ios::binary);
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(_ifstream), {});
        return buffer;
    }
    catch (const std::exception& e)
    {
        return std::vector<unsigned char> {};
    }

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


FS_STATUS fs_echo_bin(std::string path_std_string, std::vector<unsigned char> bin_contents){

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
