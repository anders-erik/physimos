
#include <string>

#include <iostream>

#include <fstream>
#include <streambuf>
#include <sstream>


#include "fs.hh"
#include "process.hh"
#include "log.hh"

namespace plib {

std::string fs_cat_rel(std::string path_std_string){
    std::string relative_path_str = physimos_root_dir_or_die();
    return fs_cat(relative_path_str + "/" + path_std_string);
}
std::vector<unsigned char> fs_cat_bin_rel(std::string path_std_string){
    std::string relative_path_str = physimos_root_dir_or_die();
    return fs_cat_bin(relative_path_str + "/" + path_std_string);
}

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
std::vector<unsigned char> fs_cat_bin(std::string path_std_string){
    std::vector<unsigned char> returnVector;

    try
    {
        std::ifstream _ifstream(path_std_string, std::ios::binary);

        if (!_ifstream.is_open()) 
            throw std::runtime_error("Could not open file");

        returnVector = std::vector<unsigned char>(std::istreambuf_iterator<char>(_ifstream), {});
        
        _ifstream.close();
        
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: Could not read file " << path_std_string << std::endl;
        plib::plog_error("FS, ", "fs_cat_bin, ", "Error: Could not read file " + path_std_string);
        returnVector = std::vector<unsigned char> {};
    }

    // Alternative read and check used by old BMP loader
    // if (!file.read(reinterpret_cast<char*>(fileData.data()), fileStreamSize)) {
    //     std::cerr << "Error: Could not read file data" << std::endl;
    //     return nullptr;
    // }
    return returnVector;
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

FS_STATUS fs_echo_append(std::string path_std_string, std::string new_contents){
    
    std::ofstream _ofstream;

    std::string current_contents = fs_cat(path_std_string);
    try
    {
        _ofstream.open(path_std_string, std::ios_base::app);
        _ofstream <<  new_contents << "\n";
        _ofstream.close();
    }
    catch (const std::exception& e)
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
