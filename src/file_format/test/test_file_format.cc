
#include "test/test.hh"

#include "file_format/test/test_bmp.hh"


int main()
{
    println("\nStarting File Format tests.\n");


    TestModule core_module {"BMP"};
        core_module.add_collection(UnitTestCollection{"one_black_pixel", one_black_pixel});
        core_module.add_collection(UnitTestCollection{"endian_swap", endian_swap});
    core_module.run();

    // std::cout << sizeof(BMP::FileHeader) << std::endl;
    // std::cout << sizeof(BMP::BITMAPINFOHEADER) << std::endl;
    

    println("\nEnd File Format tests.\n");
    return 0;
}

