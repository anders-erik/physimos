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

    // Result test
    plib::Result physimosRepoDirResult = get_physimos_root_dir();
    std::string physimosRepoDir;
    if (physimosRepoDirResult.status)
        physimosRepoDir = *( (std::string*)physimosRepoDirResult.pointer );
    else
        plib::handleResult((plib::ResultInfo*)physimosRepoDirResult.pointer);
    
    std::cout << "physimosRepoDir = " << physimosRepoDir << std::endl;
    
    

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
