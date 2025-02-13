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
    
    std::string subcommand = "ls";
    std::string subcommand_output = run_subcommand(subcommand);
    if (subcommand_output.size() > 0){
        // std::cout << "subcommand_output = " << subcommand_output << std::endl;
        // std::cout << "subcommand_output.size() = " << subcommand_output.size() << std::endl;
    }

    // Result testing
    plib::Result physimosRepoDirResult = get_physimos_root_dir();
    std::string physimosRepoDir;
    if (physimosRepoDirResult.status)
        physimosRepoDir = *( (std::string*)physimosRepoDirResult.pointer );
    else
        plib::handleResult((plib::ResultInfo*)physimosRepoDirResult.pointer);
    
    // std::cout << "physimosRepoDir = " << physimosRepoDir << std::endl;
    
    

    // Test Objects
    Bitmap* dummyBMP;
    std::filesystem::path dummyFilePath("/dev/null/nonexistent");
    
    Bitmap* triangleBMP;
    std::filesystem::path triangleFilePath = physimosRepoDir + "/resources/models/triangle/triangle.bmp";


    // 
    // FILESYSTEM
    std::cout << "Starting: filesystem" << std::endl;
    dummyBMP = BMP_load(dummyFilePath);
    assertTrue(dummyBMP == nullptr, "dummyBMP == nullptr");

    triangleBMP = BMP_load(triangleFilePath);
    assertTrue(triangleBMP != nullptr, "triangleBMP != nullptr");
    // unsigned char c;
    // int ci = 0;
    // while (ci < triangleBMP->size ){
    //     c = triangleBMP->data[ci++];
    //     std::cout << "c = " << c << std::endl;

    // }
    std::cout << "triangleBMP->size = " << triangleBMP->size << std::endl;

    std::cout << "Done    : filesystem" << std::endl << std::endl;




    std::cout << std::endl << "-------------------------" << std::endl;
    return 0;
}
