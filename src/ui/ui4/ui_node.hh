
#pragma once

#include "lib/bitmap.hh"
#include "lib/arr.hh"

#include "ui_box.hh"

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
    // PX32 color = 0x558855FF;

    bool custom_render = false; // This should stop the current rendering tree traversal and rely on external call for rendering

    enum class Type
    {   
        Base,
        String
    } type = Type::Base;

    void (*handle_click)(UINode*, void*) = nullptr;
    void (*handle_hover)(UINode*, void*) = nullptr;
    void (*handle_unhover)(UINode*, void*) = nullptr;


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

};