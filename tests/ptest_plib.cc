#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <filesystem>

#include "process_info.hpp"


#include "ptest.hh"

#include "lib/result.hh"
#include "lib/string.hh"
#include "lib/fs.hh"
#include "lib/toml.hh"


using namespace ptest;


int main(){
    std::cout << "--------------------------------------------------------------" << std::endl << std::endl;

    std::string physimos_root_dir = physimos_root_dir_or_die();

    


    // CAT & ECHO
    std::cout << "Starting: cat & echo" << std::endl;

    std::string cat_spath = physimos_root_dir + "//README.md";
    std::string cat_content = plib::fs_cat(cat_spath);
    assertTrue(cat_content.size() > 0, "cat_content.size() > 0");

    std::string echo_spath = physimos_root_dir + "/tmp/README.md";
    plib::FS_STATUS echo_status = plib::fs_echo(echo_spath, cat_content);
    assertTrue(echo_status == plib::FsOk, "echo_status == plib::FsOk");
    // std::cout << cat_content << std::endl;

    std::cout << "Done    : cat & echo" << std::endl << std::endl;




    // CAT_BIN & ECHO_BIN
    std::cout << "Starting: cat_bin & echo_bin" << std::endl;

    std::string cat_bin_spath = physimos_root_dir + "//tests/testdata/2x2.bmp";
    std::vector<unsigned char>& cat_bin_contents = plib::fs_cat_bin(cat_bin_spath);
    assertTrue(cat_bin_contents.size() == 70, "cat_bin_contents.size() == 70");

    std::string echo_bin_spath = physimos_root_dir + "/tmp/2x2.bmp";
    plib::FS_STATUS echo_bin_status = plib::fs_echo_bin(echo_bin_spath, cat_bin_contents);
    assertTrue(echo_bin_status == plib::FsOk, "echo_bin_status == plib::FsOk");

    std::cout << "Done    : cat_bin & echo_bin" << std::endl << std::endl;


    // TOML PARSE
    std::cout << "Starting: toml parsing" << std::endl;

    std::string toml_test_file_spath = physimos_root_dir + "/tests/testdata/plib_toml.toml";

    // First table
    plib::TOML toml;
    toml.load(toml_test_file_spath);
    plib::TOML_Table* toml_first_table = toml.find_table("first_table");
    assertTrue(toml_first_table != nullptr, "toml_first_table != nullptr");
    assertTrue(toml_first_table->name == "first_table", "toml_first_table->name == \"first_table\"");
    

    // pimage_gen table -- 100x100 paths
    plib::TOML toml_pimage_gen;
    toml_pimage_gen.load(toml_test_file_spath);
    // Find table
    plib::TOML_Table toml_pimage_gen_table = toml_pimage_gen["pimage_gen"];
    assertTrue(toml_pimage_gen_table.name != "ROOT_TABLE", "toml_pimage_gen_table[\"pimage_gen\"] != nullptr");
    // Find correct KV
    plib::TOML_KV _100x100_key = toml_pimage_gen_table["100x100_rand"];
    assertTrue(_100x100_key.value.type == plib::TOML_ValueType::ARRAY, "_100x100_key.value.type == plib::TOML_ValueType::ARRAY");
    // Verify that the array values are equal
    plib::TOML_Value _100x100_array_from_file = _100x100_key.value;
    plib::TOML_Value _100x100_manual_array = plib::TOML_Value("[\"_100x100_rand_generated.bmp\", \"_100x100_rand_test_output.bmp\"]");
    assertTrue(_100x100_array_from_file == _100x100_manual_array, "_100x100_array_from_file == _100x100_manual_array");
    

    std::cout << "Done    : toml parsing" << std::endl << std::endl;


    std::cout << std::endl << "-------------------------" << std::endl;
    return 0;
}
