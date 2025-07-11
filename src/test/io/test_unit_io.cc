
#include "lib/file.hh"

#include "test/test.hh"

#include "test/io/bmp/test_bmp.hh"
#include "test/io/json/tlib_json.hh"


int main()
{
    println("\nStarting File Format tests.\n");

    TCModule bmp_module = {
        "BMP", {
            {"one_black_pixel", one_black_pixel},
            {"endian_swap", endian_swap},
        }
    };
    bmp_module.run();

    // std::cout << sizeof(BMP::FileHeader) << std::endl;
    // std::cout << sizeof(BMP::BITMAPINFOHEADER) << std::endl;


    // TestModule json_module {"JSON"};
    //     json_module.add_collection(UnitTestCollection{"json_test_setup", json_test_setup});
    // json_module.run();

    println("\nEnd File Format tests.\n");
    return 0;
}

