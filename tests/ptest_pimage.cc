#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <filesystem>

#include "ptest.hh"

#include "bmp.hh"

#include "lib/result.hh"
#include "process_info.hpp"

using namespace pimage;
using namespace pimage::loaders;

using namespace ptest;


int main(){
    std::cout << "--------------------------------------------------------------" << std::endl << std::endl;


    // Two different ways of getting the physimos root directory
    char* PHYSIMOS_ROOT_DIR = std::getenv("PHYSIMOS_ROOT_DIR");
    // std::string physimosRepoDir = getGitRepoRootDir();

    plib::Result physimosRepoDirResult = get_physimos_root_dir();
    std::string physimosRepoDir;
    if (physimosRepoDirResult.status)
        physimosRepoDir = *( (std::string*)physimosRepoDirResult.pointer );
    else
        plib::handleResult((plib::ResultInfo*)physimosRepoDirResult.pointer);
    
    
    

    // Test Objects
    BMP_Result dummyBMP;
    BMP_Result triangleBMP;
    std::filesystem::path dummyFilePath("/dev/null/nonexistent");
    std::filesystem::path triangleFilePath = physimosRepoDir + "/resources/models/triangle/triangle.obj";
    

    // 
    // FILESYSTEM
    std::cout << "Starting: filesystem" << std::endl;
    dummyBMP = BMP_load(dummyFilePath);
    triangleBMP = BMP_load(triangleFilePath);

    assertTrue(dummyBMP.loadStatus == LoadStatus::ErrorOpeningFile, "dummyBMP.loadStatus == LoadStatus::ErrorOpeningFile");
    assertTrue(triangleBMP.loadStatus == LoadStatus::Ok, "triangleBMP.loadStatus == LoadStatus::Ok");

    std::cout << "Done    : filesystem" << std::endl << std::endl;




    std::cout << std::endl << "-------------------------" << std::endl;
    return 0;
}
