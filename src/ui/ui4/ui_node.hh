
#pragma once

#include "lib/bitmap.hh"
#include "lib/arr.hh"

#include "ui_box.hh"
#include "ui_event.hh"
#include "ui_font_bitmap.hh"

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


// NOT POSSIBLE FOR MY USE CASE-- unable to store in the same container
// INSTEAD WE TYPE THE UICs
template <typename T>
struct UINodeT
{
    // NORMAL STUFF

    T* value;
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
        f64,
        f32,
        i32,
        Str,
        None,
    } data_type = DataType::None;
    enum class DataLifetime
    {
        Forever, // the owner of bound data will always outlive the UI
        Frame,   // The lifetime of the is only guaranteed during the current frame
    } data_lifetime = DataLifetime::Frame;
    void* data = nullptr;
    

    UINodeVisibility visibility;
    bool custom_render = false; // This should stop the current rendering tree traversal and rely on external call for rendering
    enum class NodeType
    {
        Base,
        String
    } type = NodeType::Base;
    Str str; // used for rendering a string NOT bound using the data type. e.g. a label


    // TODO: implement the below behavior
    // enum event_type {mouse_click, mouse_unclick, handle_hover, etc. etc.. };
    // Map<event_type, callback_fn> callbacks;
    
    // 
    UIEventReturn (*event_callback)(UINode*, UIEvent _ui_event, void*) = nullptr;

    void (*handle_mouse_click)(UINode*, UserInput _user_input, void*) = nullptr;
    void (*handle_mouse_release)(UINode*, UserInput _user_input, void*) = nullptr;

    void (*handle_mouse_enter)(UINode*, UserInput _user_input, void*) = nullptr;
    void (*handle_mouse_exit)(UINode*, UserInput _user_input, void*) = nullptr;

    void (*handle_scroll)(UINode*, UserInput _user_input, void*) = nullptr;
    void (*handle_key_press)(UINode*, UserInput _user_input, void*) = nullptr;

    
    // Experiment with variadic event handlers
    // template<typename... Args>
    // void (*handle_variatic)(UINode*, UserInput _user_input, Args...) = nullptr;

    template<typename... Args>
    void func(Args... args) // recursive variadic function
    {
        func(args...) ;
    }


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

        handle_mouse_click = nullptr;
        handle_mouse_release = nullptr;
        handle_mouse_enter = nullptr;
        handle_mouse_exit = nullptr;
        handle_scroll = nullptr;
        handle_key_press = nullptr;
    }

    void set_color(PX32 _color)
    {
        if(type == NodeType::String)
            type = NodeType::Base;

        visibility.set_color(_color);
    }

    void set_data(DataType _data_type, void* _data_ptr)
    {
        data_type = _data_type;
        data = _data_ptr;
        data_lifetime = DataLifetime::Frame;
    }

    bool is_type(DataType _data_type)
    {
        if(_data_type != data_type)
            return false;
        if(data == nullptr)
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