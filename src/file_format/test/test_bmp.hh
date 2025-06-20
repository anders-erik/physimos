
#pragma once

#include <cstring> // memcmp

#include "test/test.hh"

#include "rend/bitmap.hh"
#include "file_format/bmp/bmp.hh"






std::vector<UnitTestFn> one_black_pixel = {


    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "1 black pixel : file_header";

        Bitmap bitmap {{1, 1}, {0, 0, 0, 255}};

        BMP::File bmp_loader {bitmap};

        BMP::FileHeader file_header_target;
        file_header_target.BM                   = 0x6677;
        file_header_target.file_size            = 58;
        file_header_target.reserved_1           = 0x0000;
        file_header_target.reserved_2           = 0x0000;
        file_header_target.first_pixel_location = 54;


        return test_info.assert(bmp_loader.file_header() == file_header_target);
    },


    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "1 black pixel : info_header";

        Bitmap bitmap {{1, 1}, {0, 0, 0, 255}};

        BMP::File bmp_loader {bitmap};

        BMP::BITMAPINFOHEADER info_header_target;
        info_header_target.header_size           = 40;
        info_header_target.width                 = 1;
        info_header_target.height                = 1;
        info_header_target.color_planes          = 1; // always 1
        info_header_target.bits_per_pixel        = 24;
        info_header_target.compression_method    = 0; // none
        info_header_target.image_size             = 4; // padded size
        info_header_target.horizontal_resolution = 3780;
        info_header_target.vertical_resolution   = 3780;
        info_header_target.palette_color_count   = 0;
        info_header_target.important_color_count = 0;


        return test_info.assert(bmp_loader.info_header() == info_header_target);
    },


    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "1 black pixel : padded bitmap data";

        Bitmap bitmap {{1, 1}, {0, 0, 0, 255}};

        BMP::File bmp_loader {bitmap};

        // DATA SIZE CALLS

        ui2 padded_data_indeces = bmp_loader.padded_data_indeces();
        unsigned int padded_data_size = padded_data_indeces.y - padded_data_indeces.x;

        if(padded_data_size != 4)
            return test_info.fail();
        if(bmp_loader.padded_data_size() != 4)
            return test_info.fail();


        // DATA CONTENT

        std::vector<unsigned char> target_padded_data {4};
        target_padded_data[0] = 0;
        target_padded_data[1] = 0;
        target_padded_data[2] = 0;
        target_padded_data[3] = 0; // padded byte should be zero when generated from bitmap

        int cmp_result = memcmp(    bmp_loader.padded_data(), 
                                    target_padded_data.data(), 
                                    4 );
        
        return test_info.assert(cmp_result == 0);
    },

};






std::vector<UnitTestFn> endian_swap = {


    []() -> UnitTestInfo 
    {
        UnitTestInfo test_info = "1 black pixel : endian double swap";

        Bitmap bitmap {{1, 1}, {0, 0, 0, 255}};

        BMP::File bmp_loader {bitmap};

        BMP::Header header_a = bmp_loader.header;
        BMP::Header header_b = bmp_loader.header;

        header_a.endian_swap();

        if(header_a.file_header == header_b.file_header)
            return test_info.fail();
        if(header_a.info_header == header_b.info_header)
            return test_info.fail();

        header_a.endian_swap();


        return test_info.assert(       header_a.file_header == header_b.file_header
                                    && header_a.info_header == header_b.info_header);
    },


};


