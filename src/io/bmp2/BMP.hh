#pragma once

#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>

#include "lib/bitmap.hh"
#include "lib/io.hh"




class BMPUtil
{
public:

    static uint get_padded_stride(Bitmap& _bitmap);
    static uint get_padded_stride_from_24bit_stride(uint _stride_24bit);
    static uint get_padded_stride_PX32RGBA(const Bitmap& _bitmap);

    static Vec<uint8_t> pad_bitmap_data(Bitmap& _bitmap);;

    static Vec<uint8_t> remove_alpha(Bitmap& _bitmap);


    static Vec<uint8_t> bitmap_to_bmp_buf(Bitmap& _bitmap);


    static uint get_padding(uint _unpadded_stride_bytes);

    static Vec<uint32_t> RGB_to_RGBA_buf(Vec<uint8_t>& _RGB_buf);

    static Vec<uint8_t> unpad_bitmap_data(Vec<uint8_t>& _padded_buf, uint _unpadded_stride, uint _bitmap_height);
};



struct BMPFileInfoHeader
{
    Vec<uint8_t> BM {2};
    uint32_t file_size;
    Vec<uint8_t> reserved_1 {2};
    Vec<uint8_t> reserved_2 {2};
    uint32_t data_offset;

    Vec<uint8_t> buff {14};

    BMPFileInfoHeader() {}
    BMPFileInfoHeader(const Bitmap& _bitmap);

    // Buffer in this case is the 14-byte-long raw header data
    void set_from_buffer(Vec<uint8_t> _buf);

    void load_from_bitmap(const Bitmap& _bitmap);

    Vec<uint8_t> get_header_buff();

    void set_BM();
    Vec<uint8_t> get_BM_buff();

    void set_file_size(const Bitmap& _bitmap);
    Vec<uint8_t> get_file_size_buff();
    uint get_file_size();

    void set_reserved_1();
    Vec<uint8_t> get_reserved_1_buff();

    void set_reserved_2();
    Vec<uint8_t> get_reserved_2_buff();

    void set_first_pixel_location();
    Vec<uint8_t> get_first_pixel_location_buff();



    /** Returns number of bytes */
    static uint get_padded_BPM_stride(const Bitmap& _bitmap);


    // bool operator!=(const BMPFileInfoHeader& rhs) { return *this == rhs ? false : true;};
    // bool operator==(const BMPFileInfoHeader& rhs)
    // {
    //     return  BM                      == rhs.BM                   &&
    //             file_size               == rhs.file_size            &&
    //             reserved_1              == rhs.reserved_1           &&
    //             reserved_2              == rhs.reserved_2           &&
    //             first_pixel_location    == rhs.first_pixel_location;
    // }

    Str to_str();
};


class BMPDIBHeader
{
public:

    uint32_t DIB_bytes;
    uint32_t width_pixels;
    uint32_t height_pixels;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t data_bytes;
    uint32_t res_hori;
    uint32_t res_vert;
    uint32_t color_count;
    uint32_t important_colors;

    BMPDIBHeader();

    BMPDIBHeader(Bitmap& _bitmap);

    void load_from_bitmap(Bitmap& _bitmap);

    void set_from_buffer(Vec<uint8_t> _buf);

    Vec<uint8_t> get_DIB_buffer();

    /** Returns number of bytes */
    static uint get_padded_BPM_stride(const Bitmap& _bitmap);
};




class BMPIO
{
public:

    Bitmap bitmap;
    BMPFileInfoHeader finfo_header;
    BMPDIBHeader DIB_Header;

    BMPIO(Bitmap& _bitmap)
        :   bitmap {_bitmap},
            finfo_header { BMPFileInfoHeader(bitmap) },
            DIB_Header { BMPDIBHeader(bitmap) }
    {
    }


    void set_bitmap(Bitmap& _bitmap);


    static void SExport_PX32RGBA(Str _file_path, Bitmap& _bitmap);

    static Bitmap SImport_PX32(Str _file_path);

    
    Vec<uint8_t> unpad_bitmap_data(Vec<uint8_t>& _padded_buf);

    Vec<uint8_t> pad_bitmap_data();

    /** Returns number of bytes */
    static uint get_padding(uint _unpadded_stride_bytes);

    /** Returns number of bytes */
    static uint get_padding(uint bits_per_pixel, uint image_width);

    /** Returns number of bytes */
    static uint get_padded_BPM_stride(const Bitmap& _bitmap);
};


