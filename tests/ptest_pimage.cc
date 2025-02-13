#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <filesystem>

#include "ptest.hh"

#include "bmp.hh"

#include "lib/result.hh"
#include "lib/string.hh"
#include "lib/fs.hh"

#include "process_info.hpp"

using namespace pimage;
using namespace pimage::io;

using namespace ptest;


int main(){
    std::cout << "--------------------------------------------------------------" << std::endl << std::endl;


    // ---------------------------------------------------------------------------------
    // START PLIB PROCESS TESTS

    // test ls
    // std::string subcommand = "ls";
    // std::string subcommand = "git rev-parse --is-inside-work-tree 2>&1";
    // std::string subcommand = "basename `git rev-parse --show-toplevel` 2>&1";
    // std::string subcommand = "git rev-parse --show-toplevel 2>&1";
    // std::string subcommand_output = run_subcommand_redir_stderr_to_stdout(subcommand);
    // if (subcommand_output.size() > 0){
    //     // std::cout << "subcommand_output = " << subcommand_output << std::endl;
    //     // std::cout << "subcommand_output.size() = " << subcommand_output.size() << std::endl;
    // }

    // Result testing
    plib::Result physimosRepoDirResult = get_physimos_root_dir();
    std::string physimosRepoDir;
    if (physimosRepoDirResult.status)
        physimosRepoDir = *( (std::string*)physimosRepoDirResult.pointer );
    else
        plib::handleResult((plib::ResultInfo*)physimosRepoDirResult.pointer);
    
    // std::cout << "physimosRepoDir = " << physimosRepoDir << std::endl;

    std::string physimos_root_dir = physimos_root_dir_or_die();
    // std::cout << "physimos_root_dir = " << physimos_root_dir << std::endl;
    

    
    char* PHYSIMOS_TEST_DATA_OUT_DIR = std::getenv("PHYSIMOS_TEST_DATA_OUT_DIR");
    if (!plib::cstr_is_empty_or_null(PHYSIMOS_TEST_DATA_OUT_DIR))
        std::cout << PHYSIMOS_TEST_DATA_OUT_DIR << std::endl;
        
    // if (PHYSIMOS_TEST_DATA_OUT_DIR.size() > 0)
    //     std::cout << "" << PHYSIMOS_TEST_DATA_OUT_DIR << std::endl;
        
    // printf("%s\n", PHYSIMOS_TEST_DATA_OUT_DIR);
    




    // ---------------------------------------------------------------------------------
    // START PIMAGE TESTS
    


    // Test Objects
    BMP dummyBMPLoader = BMP();
    Bitmap* dummyBitmap;
    std::filesystem::path dummyFilePath("/dev/null/nonexistent");

    BMP _2x2BMPLoader = BMP();
    Bitmap* _2x2bitmap;
    std::filesystem::path BMP_2x2_path = physimosRepoDir + "/tests/testdata/2x2.bmp";
    
    BMP triangleBMPLoader = BMP();
    Bitmap* triangleBitmap;
    std::filesystem::path triangleFilePath = physimosRepoDir + "/resources/models/triangle/triangle.bmp";


    // 
    // FILESYSTEM
    std::cout << "Starting: filesystem" << std::endl;
    dummyBitmap = dummyBMPLoader.load(dummyFilePath);
    assertTrue(dummyBitmap == nullptr, "dummyBitmap == nullptr");

    _2x2bitmap = _2x2BMPLoader.load(BMP_2x2_path);
    assertTrue(_2x2bitmap != nullptr, "_2x2bitmap != nullptr");
    std::cout << "_2x2bitmap->size = " << _2x2bitmap->size << std::endl;
    // _2x2BMPLoader.print_header();
    
    // std::cout << "_2x2bitmap->data = " << _2x2bitmap->data << std::endl;
    // delete _2x2bitmap;
    _2x2bitmap->~Bitmap();
    // std::cout << "_2x2bitmap->data = " << _2x2bitmap->data << std::endl;

    triangleBitmap = triangleBMPLoader.load(triangleFilePath);
    assertTrue(triangleBitmap != nullptr, "triangleBitmap != nullptr");
    // triangleBMPLoader.print_header();

    // unsigned char c;
    // int ci = 0;
    // while (ci < triangleBitmap->size ){
    //     c = triangleBitmap->data[ci++];
    //     std::cout << "c = " << c << std::endl;

    // }
    std::cout << "triangleBitmap->size = " << triangleBitmap->size << std::endl;

    std::cout << "Done    : filesystem" << std::endl << std::endl;




    std::cout << std::endl << "-------------------------" << std::endl;
    return 0;
}
