
#pragma once

#include "lib/bitmap.hh"
#include "lib/arr.hh"

#include "ui_box.hh"
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

struct UINode
{
    UINode* parent = nullptr;
    Arr<UINode*> children;

    UIBox box;
    UINodeVisibility visibility;
    Str str; // used for rendering a string
    // PX32 color = 0x558855FF;

    bool custom_render = false; // This should stop the current rendering tree traversal and rely on external call for rendering

    enum class NodeType
    {   
        Base,
        String
    } type = NodeType::Base;

    void (*handle_click)(UINode*, UserInput _user_input, void*) = nullptr;
    void (*handle_hover)(UINode*, UserInput _user_input, void*) = nullptr;
    void (*handle_unhover)(UINode*, UserInput _user_input, void*) = nullptr;
    void (*handle_key_press)(UINode*, UserInput _user_input, void*) = nullptr;
    


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

        handle_click = nullptr;
        handle_hover = nullptr;
        handle_unhover = nullptr;
    }

    void set_color(PX32 _color)
    {
        if(type == NodeType::String)
            type = NodeType::Base;

        visibility.set_color(_color);
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

        // Bitmap& font_bitmap = UI::get_bitmap_assets();
        Bitmap& font_bitmap = get_font_bitmap();

        for(uint i = 0; i < str.size(); i++)
        {
            // char letter = 'a';
            char letter = str[i];
            uint letter_height_offset = (letter - 30) * 150;
            u2 pos = {0, letter_height_offset};
            u2 size = {80, 150};

            Bitmap bmp_letter = font_bitmap.get_subbitmap(pos, size).scale(size_factor);

            // TODO: Negative y-value-pastes are not pasted with offset.
            // I believe that the problem is that I am not sampling the pasted bitmap at the proper indeces after persforming intersection tests!
            // For intersections we need not only return the interection box, but also the sample offset where we begin the sampling of the pasted bitmap!!

            // str_bitmap.paste(bmp_letter, {(double)i*char_width, -30.0});
            // str_bitmap.paste(bmp_letter, {(double)i*char_width, 0.0});

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