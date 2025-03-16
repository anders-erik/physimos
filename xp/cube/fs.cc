
#include "fs.hh"

#include <filesystem>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace std::filesystem;

path ROOT_DIR;

bool fs_init(){
    char* ROOT_DIR_c = std::getenv("XP_ROOT_DIR");

    if(!ROOT_DIR_c) {
        std::cout << "ERROR: XP_ROOT_DIR not found. Exiting." << std::endl;
        return false;
    };
    
    ROOT_DIR = path(ROOT_DIR_c);

    return true;
}

std::string cat(std::string path_str){

    path path_abs = ROOT_DIR;

    path_abs.append(path_str);

    std::ifstream _ifstream(path_abs);

    std::stringstream _sstream;

    _sstream << _ifstream.rdbuf();

    return _sstream.str();

}


