#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <filesystem>
#include <vector>

#include "ptest.hh"

#include "bmp.hh"

#include "lib/result.hh"
#include "lib/string.hh"
#include "lib/fs.hh"

#include "lib/process.hh"

using namespace pimage;
using namespace pimage::io;

using namespace ptest;

void full_bpm_generation_test();


void full_bpm_generation_test(){
    std::string physimos_root_dir = physimos_root_dir_or_die();
    std::string output;

    // std::string generate_images_command = "python3 " + physimos_root_dir + "/ptest/echo.py";
    std::string generate_images_command = "python3 " + physimos_root_dir + "/ptest/ptest_pil.py -g";
    output = run_subcommand_redir_stderr_to_stdout(generate_images_command);
    std::cout << "generate_images_command.output = " << output << std::endl;
    
    
}

int main(int argc, char** argv) {
    std::cout << "--------------------------------------------------------------" << std::endl << std::endl;
        

    // ---------------------------------------------------------------------------------
    // START PLIB PROCESS TESTS



    bool with_h_flag = argument_flag_exists(argc, argv, "-h");
    if (with_h_flag)
        std::cout << "H FLAG!" << std::endl;

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
    if (physimos_root_dir.size() == 0)
        std::cout << "[ERROR] physimos_root_dir.size() = 0 after physimos_root_dir_or_die()" << std::endl;
    

    
    // char* PHYSIMOS_TEST_DATA_OUT_DIR = std::getenv("PHYSIMOS_TEST_DATA_OUT_DIR");
    // if (!plib::cstr_is_empty_or_null(PHYSIMOS_TEST_DATA_OUT_DIR))
    //     std::cout << PHYSIMOS_TEST_DATA_OUT_DIR << std::endl;
        
    // if (PHYSIMOS_TEST_DATA_OUT_DIR.size() > 0)
    //     std::cout << "" << PHYSIMOS_TEST_DATA_OUT_DIR << std::endl;
        
    // printf("%s\n", PHYSIMOS_TEST_DATA_OUT_DIR);
    


    // ---------------------------------------------------------------------------------
    // START PIMAGE TESTS

        

    bool g_flag_exists = argument_flag_exists(argc, argv, "-g");
    if (g_flag_exists){
        std::cout << "G FLAG!" << std::endl;
        full_bpm_generation_test();
        std::cout << "G FLAG IMAGE TESTS DONE. EXITING." << std::endl;
        return 0;
    }


    // Test Objects
    BMP dummyBMPLoader = BMP();
    Bitmap* dummyBitmap;
    std::filesystem::path dummyFilePath("/dev/null/nonexistent");

    BMP _2x2BMPLoader = BMP();
    Bitmap* _2x2bitmap;
    std::filesystem::path BMP_2x2_read_path = physimosRepoDir + "/ptest/testdata/2x2.bmp";
    std::filesystem::path BMP_2x2_write_path = physimosRepoDir + "/tmp/2x2.bmp";
    
    BMP triangleBMPLoader = BMP();
    Bitmap* triangleBitmap;
    std::filesystem::path triangle_read_path = physimosRepoDir + "/resources/models/triangle/triangle.bmp";
    std::filesystem::path triangle_write_path = physimosRepoDir + "/tmp/triangle.bmp";

    // LARGE FILE TEST
    // BMP _10kx10kBMPLoader = BMP();
    // Bitmap* _10kx10kBitmap;
    // std::filesystem::path _10kx10kread_path = "/home/anders/Desktop/10kx10k.bmp";
    // std::filesystem::path _10kx10kwrite_path = "/home/anders/Desktop/10kx10k_write.bmp";
    // _10kx10kBitmap = _10kx10kBMPLoader.load(_10kx10kread_path);
    // _10kx10kBMPLoader.from_bitmap(_10kx10kBitmap);
    // _10kx10kBMPLoader.write(_10kx10kwrite_path);

    std::cout << "Starting: BMP" << std::endl;

    // DUMMY LOAD SHOULD FAIL
    dummyBitmap = dummyBMPLoader.load(dummyFilePath);
    assertTrue(dummyBitmap == nullptr, "dummyBitmap == nullptr");

    // 2 x 2 BMP
    _2x2bitmap = _2x2BMPLoader.load(BMP_2x2_read_path);
    assertTrue(_2x2bitmap != nullptr, "_2x2bitmap != nullptr");
    
    _2x2BMPLoader.from_bitmap(_2x2bitmap);
    bool write_ok_2x2 = _2x2BMPLoader.write(BMP_2x2_write_path);
    assertTrue(write_ok_2x2, "_2x2BMPLoader.write(BMP_2x2_write_path)");


    // TRIANGLE
    triangleBitmap = triangleBMPLoader.load(triangle_read_path);
    assertTrue(triangleBitmap != nullptr, "triangleBitmap != nullptr");
    // Write the loaded bitmap object
    triangleBMPLoader.from_bitmap(triangleBitmap); 
    bool write_ok_triangle = triangleBMPLoader.write(triangle_write_path);
    assertTrue(write_ok_triangle, "triangleBMPLoader.write(triangle_write_path)");
    

    // _2x2BMPLoader.print_header();
    // triangleBMPLoader.print_header();

    std::cout << "Done    : BMP" << std::endl << std::endl;




    std::cout << std::endl << "-------------------------" << std::endl;
    return 0;
}
