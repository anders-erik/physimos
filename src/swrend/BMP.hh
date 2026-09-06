#pragma once

#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>

#include "swrend/bitmap.hh"






#include <sys/stat.h>

int file_echo(const char* _file_path_c, void* data, uint _data_length)
{
    remove(_file_path_c);

    int fd = open(_file_path_c, O_WRONLY | O_CREAT );
    if(fd < 0)
    {
        Print::ln("Error: Failed to open file for echo function.");
        return -1;
    }
    fchmod(fd, 0777);


    int write_ret = write(fd, data, _data_length);
    if(write_ret < 0)
    {
        Print::ln("Error: Failed to write to file in echo function.");
        return -1;
    }


    fd = close(fd);
    if(fd < 0)
    {
        Print::ln("Error: Failed to close file in echo function.");
        return -1;
    }

    return 0;
}

Vec<uint8_t> file_cat(const char* _file_path_c)
{

    int fd = open(_file_path_c, O_RDONLY);
    if(fd < 0)
    {
        Print::ln("Error: Failed to open file in file_cat() call.");
        return {};
    }


    
    struct stat st;
    stat(_file_path_c, &st);
    long int size = st.st_size;

    uint file_size = (uint) size;
    Vec<uint8_t> vec {file_size};
    
    // fseek(fp, 0L, SEEK_END);
    // sz = ftell(fp);


    int read_ret = read(fd, vec.data_mut(), file_size);
    if(read_ret < 0)
    {
        Print::ln("Error: Failed to read file in cat-function.");
        return {};
    }


    fd = close(fd);
    if(fd < 0)
    {
        Print::ln("Error: Failed to close file in cat function.");
        return {};
    }

    return vec;
}


struct BMPFileInfoHeader
{
    Vec<uint8_t> BM {2};
    uint32_t file_size;
    Vec<uint8_t> reserved_1 {2};
    Vec<uint8_t> reserved_2 {2};
    uint32_t data_offset;

    Vec<uint8_t> buff {14};

    BMPFileInfoHeader() {}
    BMPFileInfoHeader(const Bitmap& _bitmap)
    {
        // set_BM();
        // set_file_size(_bitmap);
        // set_reserved_1();
        // set_reserved_2();
        // set_first_pixel_location();

        

        load_from_bitmap(_bitmap);
    }

    // Buffer in this case is the 14-byte-long raw header data
    void set_from_buffer(Vec<uint8_t> _buf)
    {
        BM[0] = 'B';
        BM[1] = 'M';

        memcpy(&(this->file_size), _buf.data()+2, 4);

        reserved_1[0] = 0;
        reserved_1[1] = 0;

        reserved_2[0] = 0;
        reserved_2[1] = 0;

        // data_offset = 54;
        memcpy(&(this->data_offset), _buf.data()+10, 4);
    }

    void load_from_bitmap(const Bitmap& _bitmap)
    {
        BM[0] = 'B';
        BM[1] = 'M';

        file_size = 54 + get_padded_BPM_stride(_bitmap) * _bitmap.h();

        reserved_1[0] = 0;
        reserved_1[1] = 0;

        reserved_2[0] = 0;
        reserved_2[1] = 0;

        data_offset = 54;
    }

    Vec<uint8_t> get_header_buff()
    {
        Vec<uint8_t> buffer {14};

        bool arch_is_big_endian = false;

        if(arch_is_big_endian)
        {
            // flip_integer_endianess
        }

        buffer[0] = BM[0];
        buffer[1] = BM[1];

        memcpy(buffer.data_mut() + 2, &file_size, 4);

        buffer[6] = reserved_1[0];
        buffer[7] = reserved_1[1];

        buffer[8] = reserved_2[0];
        buffer[9] = reserved_2[1];

        memcpy(buffer.data_mut() + 10, &data_offset, 4);

        return buffer;
    }

    void set_BM()
    {
        buff[0] = 'B';
        buff[1] = 'M';
    }
    Vec<uint8_t> get_BM_buff()
    {
        return Vec<uint8_t>(2, (buff.data_mut()+0));
    }

    void set_file_size(const Bitmap& _bitmap)
    {
        uint file_size = 54 + get_padded_BPM_stride(_bitmap) * _bitmap.h();
        buff[2] = (file_size & 0x000000FF) >> 0;
        buff[3] = (file_size & 0x0000FF00) >> 8;
        buff[4] = (file_size & 0x00FF0000) >> 16;
        buff[5] = (file_size & 0xFF000000) >> 24;
    }
    Vec<uint8_t> get_file_size_buff()
    {
        return Vec<uint8_t>(4, (buff.data_mut()+2));
    }
    uint get_file_size()
    {
        uint file_size = 0;
        file_size += (buff[2] << 24);
        file_size += (buff[3] << 16);
        file_size += (buff[4] << 8 );
        file_size += (buff[5] << 0 );
        return file_size;
    }

    void set_reserved_1()
    {
        buff[6] = 0x00;
        buff[7] = 0x00;
    }
    Vec<uint8_t> get_reserved_1_buff()
    {
        return Vec<uint8_t>(2, (buff.data_mut()+6));
    }

    void set_reserved_2()
    {
        buff[8] = 0x00;
        buff[9] = 0x00;
    }
    Vec<uint8_t> get_reserved_2_buff()
    {
        return Vec<uint8_t>(2, (buff.data_mut()+8));
    }

    void set_first_pixel_location()
    {
        uint first_pixel_location = 54;
        buff[10] = (first_pixel_location & 0x000000FF) >> 0;
        buff[11] = (first_pixel_location & 0x0000FF00) >> 8;
        buff[12] = (first_pixel_location & 0x00FF0000) >> 16;
        buff[13] = (first_pixel_location & 0xFF000000) >> 24;
    }
    Vec<uint8_t> get_first_pixel_location_buff()
    {
        return Vec<uint8_t>(4, (buff.data_mut()+10));
    }



    /** Returns number of bytes */
    static uint get_padded_BPM_stride(const Bitmap& _bitmap)
    {
        uint stride_byte_count = _bitmap.w() * 3;
        uint padding_count = 4 - (stride_byte_count % 4);
        return stride_byte_count + padding_count;
    }


    // bool operator!=(const BMPFileInfoHeader& rhs) { return *this == rhs ? false : true;};
    // bool operator==(const BMPFileInfoHeader& rhs)
    // {
    //     return  BM                      == rhs.BM                   &&
    //             file_size               == rhs.file_size            &&
    //             reserved_1              == rhs.reserved_1           &&
    //             reserved_2              == rhs.reserved_2           &&
    //             first_pixel_location    == rhs.first_pixel_location;
    // }

    Str to_str()
    {
        Str info_header_str;
        info_header_str += "info_header_str: \n   ";

        info_header_str += Str::CH(BM[0]);
        info_header_str += Str::CH(BM[1]) + ", "; 

        info_header_str += Str::UI(file_size)  + ", ";

        info_header_str += Str::CH(reserved_1[0]);
        info_header_str += Str::CH(reserved_1[1])  + ", ";

        info_header_str += Str::CH(reserved_2[0]);
        info_header_str += Str::CH(reserved_2[1])  + ", ";

        info_header_str += Str::UI(data_offset);

        info_header_str += "\n";

        // return  Str("FileHeader: ") + Str::UI(BM) + ", " +
        //         Str::UI(get_file_size()) + ", " +
        //         Str::UI(reserved_1) + ", " +
        //         Str::UI(reserved_2) + ", " +
        //         Str::UI(first_pixel_location);
        
        return info_header_str;
    }
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

    BMPDIBHeader()
    {
    }

    BMPDIBHeader(Bitmap& _bitmap)
    {
        load_from_bitmap(_bitmap);
    }

    void load_from_bitmap(Bitmap& _bitmap)
    {
        DIB_bytes = 40;
        width_pixels = _bitmap.w();
        height_pixels = _bitmap.h();
        planes = 1;
        bits_per_pixel = 24;
        compression = 0;
        data_bytes = _bitmap.h() * get_padded_BPM_stride(_bitmap);
        res_hori = 2835;
        res_vert = 2835;
        color_count = 0;
        important_colors = 0;
    }

    void set_from_buffer(Vec<uint8_t> _buf)
    {
        memcpy(this, _buf.data(), 40);
    }

    Vec<uint8_t> get_DIB_buffer()
    {
        Vec<uint8_t> ret_buff {DIB_bytes};

        memcpy(ret_buff.data_mut(), this, DIB_bytes);

        return ret_buff;
    }

    /** Returns number of bytes */
    static uint get_padded_BPM_stride(const Bitmap& _bitmap)
    {
        uint stride_byte_count = _bitmap.w() * 3;
        uint padding_count = 4 - (stride_byte_count % 4);
        return stride_byte_count + padding_count;
    }
};


// Class used for exporting Bitmaps as BMP-files
// This class is not intended for client use, but is a utility class used by the static BMPIO::Export-method
class BMPExporter
{
public:



};


class BMPUtil
{
public:

    static uint get_padded_stride(Bitmap& _bitmap)
    {
        uint stride_byte_count = _bitmap.w() * 3;
    
        uint padding_count;

        // BMP always pads the stride to multiples of 4 bytes
        if( (stride_byte_count % 4) == 0)
            padding_count = 0;
        else
            padding_count = 4 - (stride_byte_count % 4);

        return stride_byte_count + padding_count;
    }

    static Vec<uint8_t> pad_bitmap_data(Bitmap& _bitmap)
    {
        uint stride = _bitmap.stride();
        uint padded_stride = get_padded_stride(_bitmap);

        Vec<uint8_t> padded_data {_bitmap.h() * padded_stride};
        padded_data.set(0x00);

        // uint8_t* bitmap_base_ptr = bitmap.get_data_mut();
        // uint8_t* padded_base_ptr = padded_data.data_mut();

        for(uint i = 0; i < _bitmap.h(); i++)
        {
            // uint index_start_of_row = i * padded_stride;
            memcpy( padded_data.data_mut() + i * padded_stride,
                    _bitmap.get_data_mut() + i * stride,
                    stride                                  );
        }

        return padded_data;
    }


    static uint get_padding(uint _unpadded_stride_bytes)
    {
        uint padding_count;

        // BMP always pads the stride to multiples of 4 bytes
        if( (_unpadded_stride_bytes % 4) == 0)
            padding_count = 0;
        else
            padding_count = 4 - (_unpadded_stride_bytes % 4);

        return padding_count;
    }


    static Vec<uint8_t> unpad_bitmap_data(Vec<uint8_t>& _padded_buf, uint _unpadded_stride, uint _bitmap_height)
    {
        uint padded_stride = _unpadded_stride + BMPUtil::get_padding(_unpadded_stride);

        uint unpadded_buffer_size = _unpadded_stride * _bitmap_height;

        Vec<uint8_t> unpadded_buf {unpadded_buffer_size};


        for(uint i = 0; i < _bitmap_height; i++)
        {
            // uint index_start_of_row = i * padded_stride;
            memcpy( unpadded_buf.data_mut() + i * _unpadded_stride,
                    _padded_buf.data_mut() + i * padded_stride,
                    _unpadded_stride                                  );
        }

        return unpadded_buf;
    }
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


    void set_bitmap(Bitmap& _bitmap)
    {
        bitmap = _bitmap;
        finfo_header.load_from_bitmap(_bitmap);
        DIB_Header.load_from_bitmap(_bitmap);
    }

    static void SExport(Str _file_path, Bitmap& _bitmap)
    {
        BMPFileInfoHeader finfo_header {_bitmap};
        BMPDIBHeader DIB_Header {_bitmap};
        Vec<uint8_t> padded_data_buf = BMPUtil::pad_bitmap_data(_bitmap);

        Vec<uint8_t> info_header_buf = finfo_header.get_header_buff();
        Vec<uint8_t> DIB_header_buf = DIB_Header.get_DIB_buffer();

        Vec<uint8_t> export_buff {finfo_header.file_size};

        memcpy( export_buff.data_mut() + 0 , 
                info_header_buf.data_mut(), 
                14);
        memcpy( export_buff.data_mut() + 14, 
                DIB_header_buf.data_mut(), 
                40);
        memcpy( export_buff.data_mut() + finfo_header.data_offset, 
                padded_data_buf.data_mut(), 
                DIB_Header.data_bytes);

        file_echo(  _file_path.to_c_str(), 
                    export_buff.data_mut(), 
                    finfo_header.file_size);
    }


    static Bitmap SImport(Str _file_path)
    {
        Vec<uint8_t> file_data = file_cat(_file_path.to_c_str());

        uint info_header_size = 14;
        
        Vec<uint8_t> info_header_buf {info_header_size};
        memcpy( info_header_buf.data_mut(),
                file_data.data() + 0,
                info_header_buf.size_byte() );
        BMPFileInfoHeader iheader;
        iheader.set_from_buffer(info_header_buf);


        uint dib_header_size = iheader.data_offset - info_header_size;

        Vec<uint8_t> DIB_header_buf {dib_header_size};
        memcpy( DIB_header_buf.data_mut(),
                file_data.data() + 14,
                DIB_header_buf.size_byte() );
        BMPDIBHeader dheader;
        dheader.set_from_buffer(DIB_header_buf);


        uint padded_data_size = file_data.size_byte() - info_header_buf.size_byte() - DIB_header_buf.size_byte();
        Vec<uint8_t> padded_data_buf {padded_data_size};
            
        memcpy( padded_data_buf.data_mut(),
                file_data.data() + iheader.data_offset,
                padded_data_buf.size_byte() );


        // Calculate image stride
        BMPDIBHeader dib_header;
        dib_header.set_from_buffer(DIB_header_buf);
        uint bits_per_pixel = dib_header.bits_per_pixel;
        uint bitmap_width = dib_header.width_pixels;
        uint bitmap_height = dib_header.height_pixels;
        uint stride = (bits_per_pixel / 8) * bitmap_width;

        // Unpad the image data
        Vec<uint8_t> unpadded_image_data = BMPUtil::unpad_bitmap_data(padded_data_buf, stride, bitmap_height);

        // Create the Bitmap object and return
        Bitmap bmp {    dib_header.width_pixels, 
                        dib_header.height_pixels    };

        memcpy( bmp.get_data_mut(),
                unpadded_image_data.data(),
                bmp.count_bytes()           );

        return bmp;
    }

    void Export(Str _file_path)
    {
        Vec<uint8_t> export_buff {finfo_header.file_size};

        Vec<uint8_t> info_header_buf = finfo_header.get_header_buff();
        Vec<uint8_t> DIB_header_buf = DIB_Header.get_DIB_buffer();
        Vec<uint8_t> padded_data_buf = pad_bitmap_data();

        // uint8_t* info_header_ptr = finfo_header.get_header_buff().data_mut();
        // uint8_t* DIB_header_ptr = DIB_Header.get_DIB_buffer().data_mut();
        // uint8_t* padded_data_ptr = pad_bitmap_data().data_mut();

        memcpy( export_buff.data_mut() + 0 , 
                info_header_buf.data_mut(), 
                14);
        memcpy( export_buff.data_mut() + 14, 
                DIB_header_buf.data_mut(), 
                40);
        memcpy( export_buff.data_mut() + finfo_header.data_offset, 
                padded_data_buf.data_mut(), 
                DIB_Header.data_bytes);

        file_echo(  _file_path.to_c_str(), 
                    export_buff.data_mut(), 
                    finfo_header.file_size);
    }

    Bitmap Import(Str _file_path)
    {
        Vec<uint8_t> file_data = file_cat(_file_path.to_c_str());

        Vec<uint8_t> info_header_buf {14};
        Vec<uint8_t> DIB_header_buf {40};
        uint padded_data_size = file_data.size_byte() - info_header_buf.size_byte() - DIB_header_buf.size_byte();
        Vec<uint8_t> padded_data_buf {padded_data_size};


        memcpy( info_header_buf.data_mut(),
                file_data.data() + 0,
                info_header_buf.size_byte() );

        memcpy( DIB_header_buf.data_mut(),
                file_data.data() + 14,
                DIB_header_buf.size_byte() );
            
        memcpy( padded_data_buf.data_mut(),
                file_data.data() + 54,
                padded_data_buf.size_byte() );

        DIB_Header.set_from_buffer(DIB_header_buf);
        finfo_header.set_from_buffer(info_header_buf);

        
        Vec<uint8_t> unpadded_image_data = unpad_bitmap_data(padded_data_buf);

        Bitmap bmp {    DIB_Header.width_pixels, 
                        DIB_Header.height_pixels    };

        memcpy( bmp.get_data_mut(),
                unpadded_image_data.data(),
                bmp.count_bytes()           );

        return bmp;
    }
    
    Vec<uint8_t> unpad_bitmap_data(Vec<uint8_t>& _padded_buf)
    {
        uint unpadded_stride = (DIB_Header.bits_per_pixel / 3) * DIB_Header.width_pixels;

        uint padded_stride = unpadded_stride + get_padding(unpadded_stride);

        uint unpadded_buffer_size = unpadded_stride * DIB_Header.height_pixels;

        Vec<uint8_t> unpadded_buf {unpadded_buffer_size};


        for(uint i = 0; i < DIB_Header.height_pixels; i++)
        {
            // uint index_start_of_row = i * padded_stride;
            memcpy( unpadded_buf.data_mut() + i * unpadded_stride,
                    bitmap.get_data_mut() + i * padded_stride,
                    unpadded_stride                                  );
        }

        return unpadded_buf;
    }

    Vec<uint8_t> pad_bitmap_data()
    {
        uint stride = bitmap.stride();
        uint padded_stride = get_padded_BPM_stride(bitmap);

        Vec<uint8_t> padded_data {bitmap.h() * padded_stride};
        padded_data.set(0x00);

        // uint8_t* bitmap_base_ptr = bitmap.get_data_mut();
        // uint8_t* padded_base_ptr = padded_data.data_mut();

        for(uint i = 0; i < bitmap.h(); i++)
        {
            // uint index_start_of_row = i * padded_stride;
            memcpy( padded_data.data_mut() + i * padded_stride,
                    bitmap.get_data_mut() + i * stride,
                    stride                                  );
        }

        return padded_data;
    }

    /** Returns number of bytes */
    static uint get_padding(uint _unpadded_stride_bytes)
    {
        uint padding_count;

        // BMP always pads the stride to multiples of 4 bytes
        if( (_unpadded_stride_bytes % 4) == 0)
            padding_count = 0;
        else
            padding_count = 4 - (_unpadded_stride_bytes % 4);

        return padding_count;
    }

    /** Returns number of bytes */
    static uint get_padding(uint bits_per_pixel, uint image_width)
    {
        uint stride_byte_count = image_width * (bits_per_pixel /8);
    
        uint padding_count;

        // BMP always pads the stride to multiples of 4 bytes
        if( (stride_byte_count % 4) == 0)
            padding_count = 0;
        else
            padding_count = 4 - (stride_byte_count % 4);

        return padding_count;
    }

    /** Returns number of bytes */
    static uint get_padded_BPM_stride(const Bitmap& _bitmap)
    {
        uint stride_byte_count = _bitmap.w() * 3;
    
        uint padding_count;

        // BMP always pads the stride to multiples of 4 bytes
        if( (stride_byte_count % 4) == 0)
            padding_count = 0;
        else
            padding_count = 4 - (stride_byte_count % 4);

        return stride_byte_count + padding_count;
    }
};


