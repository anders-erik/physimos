
#pragma once

#include "lib/bitmap.hh"
#include "lib/arr.hh"
#include "lib/defs.hh"

#include "ui_box.hh"
#include "ui_event.hh"
#include "ui_font_bitmap.hh"
#include "ui_defs.hh"

struct UINodeVisibility
{
    enum Type
    {
        COLOR,
        BITMAP,
        NONE,
    } type;

    union Value
    {
        PX32 color;
        Bitmap* bitmap;
        void* null;
    } value;

    UINodeVisibility() = default;

    void set_color(PX32 _color)
    {
        try_delete_bitmap();

        type = COLOR;
        value.color = _color;
    }

    void set_none()
    {
        try_delete_bitmap();

        type = NONE;
        value.null = nullptr;
    }

    void set_bitmap()
    {
        try_delete_bitmap();

        type = BITMAP;
        value.bitmap = new Bitmap(10, 10);
        value.bitmap->set_format(PX32F::ARGB);
        value.bitmap->clear(0xFFFFFFFF);
    }

    void set_bitmap(uint _width, uint _height)
    {
        try_delete_bitmap();

        type = BITMAP;
        value.bitmap = new Bitmap(_width, _height);
        value.bitmap->set_format(PX32F::ARGB);
        value.bitmap->clear(0xFFFFFFFF);
    }

    void try_delete_bitmap()
    {
        if(type == BITMAP)
            delete value.bitmap;
    }


    ~UINodeVisibility()
    {
        try_delete_bitmap();
    }
};


// Provides the information necessary to invoke and provide data for an event callback
template <typename A, typename C>
struct CallbackContext
{
    UIEventReturn (*callback_fn)(UIEvent, CallbackContext<A, C>*) = nullptr;
    void* app = nullptr;
    void* ui = nullptr;
    void* uic = nullptr;
    void* node = nullptr;
};


// template <typename T>
struct UINode
{
    UINode* parent = nullptr;
    Arr<UINode*> children;
    UIBox box;

    /* Used for typed nodes that provide data pointers for basic types */
    enum class DataType
    {
        ptr,
        f64_ptr,
        i32_ptr,
        Str_ptr,
        i64,
        None,
    } data_type = DataType::None;
    enum class DataLifetime
    {
        Forever, // the owner of bound data will always outlive the UI
        Frame,   // The lifetime of the is only guaranteed during the current frame
    } data_lifetime = DataLifetime::Frame;
    
    union Data
    {
        void* PTR;
        f64* F64_PTR;
        Str* STR_PTR;
        i64  I64;
        u8 NONE[8] = {0};
    } data_;
    static_assert(sizeof(Data) == 8);

    

    UINodeVisibility visibility;
    bool custom_render = false; // This should stop the current rendering tree traversal and rely on external call for rendering
    enum class NodeType
    {
        Base,
        String
    } type = NodeType::Base;
    Str str; // used for rendering a string NOT bound using the data type. e.g. a label


    // Arr<callback_fn> callbacks;

    UIEventReturn (*event_callback)(UINode*, UIEvent _ui_event, void*) = nullptr;
    // Provides a pointer to the component that controls this particular node
    void* uic = nullptr;
    // CallbackContext<int, bool> callback_context;

    UINode()
    {
        init();
    }
    UINode(d2 _pos, d2 _size)
        :   box {_pos, _size}
    {
        init();
    }

    void init()
    {
        visibility.type = UINodeVisibility::COLOR;
        visibility.value.color = 0x558855FF;
    }

    void set_color(PX32 _color)
    {
        if(type == NodeType::String)
            type = NodeType::Base;

        visibility.set_color(_color);
    }

    void* get_data_ptr() {if(data_type != DataType::ptr) throw; return data_.PTR;}
    f64*  get_data_f64_ptr() {if(data_type != DataType::f64_ptr) throw; return data_.F64_PTR;}
    Str*  get_data_Str_ptr() {if(data_type != DataType::Str_ptr) throw; return data_.STR_PTR;}
    i64   get_data_i64() {if(data_type != DataType::i64) throw; return data_.I64;}
    
    template <typename T>
    void set_data(DataType _data_type, T _data)
    {
        data_type = _data_type;

        switch (data_type)
        {
        case DataType::ptr:
            data_.PTR = (void*) _data;
            break;
        case DataType::f64_ptr:
            data_.F64_PTR = (f64*) _data;
            break;
        case DataType::Str_ptr:
            data_.STR_PTR = (Str*)_data;
            break;
        case DataType::i64:
            data_.I64 = (i64)_data;
            break;
        
        default:
            break;
        }

        data_lifetime = DataLifetime::Frame;
    }

    // void set_data(DataType _data_type, i64 _integer)
    // {
    //     data_type = _data_type;
    //     data_ = _integer;
    //     data_lifetime = DataLifetime::Frame;
    // }

    bool is_type(DataType _data_type)
    {
        if(_data_type != data_type)
            return false;

        return true;
    }

    void set_str(Str _str)
    {
        type = NodeType::String;
        // double char_width = 20;
        double char_height = 30;
        // int font_pixel_size = font_size * 1.5;

        double size_factor =  (char_height / 150.0);
        double char_width = char_height * (80.0 / 150.0);

        str = _str;
        box.size.x = char_width * str.size();
        box.size.y = char_height;

        visibility.set_bitmap(box.size.x, box.size.y);
        Bitmap& str_bitmap = *visibility.value.bitmap;

        for(uint i = 0; i < str.size(); i++)
        {
            Bitmap bmp_letter = get_char_bitmap(str[i]).scale(size_factor);
            str_bitmap.paste2(bmp_letter, {(double)i*char_width, 0.0});

            // Paste tests
            if(false)
            {
                str_bitmap.paste2(bmp_letter, {(double)i*char_width-100, -10.0});
                str_bitmap.paste2(bmp_letter, {(double)i*char_width, 20.0});
                str_bitmap.paste2(bmp_letter, {(double)i*char_width+20, 0.0});
            }
        }

    }
};