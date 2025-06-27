
#include "lib/file.hh"

#include "test/test.hh"

#include "test/file_format/bmp/test_bmp.hh"
#include "test/file_format/json/test_json.hh"


int main()
{
    println("\nStarting File Format tests.\n");

    TestModule bmp_module {"BMP"};
        bmp_module.add_collection(UnitTestCollection{"one_black_pixel", one_black_pixel});
        bmp_module.add_collection(UnitTestCollection{"endian_swap", endian_swap});
    bmp_module.run();

    // std::cout << sizeof(BMP::FileHeader) << std::endl;
    // std::cout << sizeof(BMP::BITMAPINFOHEADER) << std::endl;


    TestModule json_module {"JSON"};
        json_module.add_collection(UnitTestCollection{"json_strings", json_strings});
    json_module.run();

    println("\nEnd File Format tests.\n");
    return 0;
}

