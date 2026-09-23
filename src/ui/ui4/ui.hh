#pragma once

#include <cstdlib>

#include "lib/arr.hh"
#include "lib/bitmap.hh"
#include "lib/llist.hh"

#include "math/vecmat.hh"

#include "io/input/user_input.hh"
#include "io/bmp2/BMP.hh"

#include "box.hh"

// struct Bitmap;

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
        value.bitmap = new Bitmap(20, 30);
        value.bitmap->clear(0xFFFFFFFF);
    }

    void set_bitmap(uint _width, uint _height)
    {
        try_delete_bitmap();

        type = BITMAP;
        value.bitmap = new Bitmap(_width, _height);
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

    Box box;
    UINodeVisibility visibility;
    // PX32 color = 0x558855FF;

    void (*handle_click)(UINode*, void*) = nullptr;
    void (*handle_hover)(UINode*, void*) = nullptr;
    void (*handle_unhover)(UINode*, void*) = nullptr;
    

    UINode() { init();}
    UINode(d2 _pos, d2 _size) : box {_pos, _size} {init();}

    void init()
    {
        visibility.type = UINodeVisibility::COLOR;
        visibility.value.color = 0x558855FF;
    }
};



struct UIAllocator
{
    UINode* nodes;
    unsigned long node_count;
    Arr<bool> occupied_slots; // value of true if occupied

    UIAllocator(unsigned long _node_count)
        :   occupied_slots {Arr<bool> {_node_count, false}}
    {
        node_count = _node_count;
        nodes = (UINode*) malloc(sizeof(UINode) * node_count);
        occupied_slots.set(0);
    }

    UINode* new_node()
    {
        for(uint i = 0; i < occupied_slots.count(); i++)
        {

            if(!occupied_slots[i])
            {
                occupied_slots[i] = true;
                return (nodes + i);
            }
        }

        Print::ln("ERROR: requested new UINode from allocator but there were no available space. ");

        return nullptr;
    }



    ~UIAllocator()
    {
        free(nodes);
        nodes = nullptr;
    }
};

struct UI
{
    UINode root;
    UIAllocator allocator;
    // Arr<Bitmap> bitmap_assets;
    static Bitmap font;

    d2 current_pointer_pos = {0.0, 0.0};
    UINode* current_hover_target = nullptr; 

    UI(unsigned long _max_number_ui_nodes)
        : allocator {_max_number_ui_nodes}
    {
        init_bitmap_assets();
    }

    UI()
        : allocator { 2 }
    {
        root.parent = nullptr;
        init_bitmap_assets();
    }

    static void init_bitmap_assets();
    static Bitmap& get_bitmap_assets();

    
    /** Appends a new node to the provided parent node. Returns new node. */
    UINode* new_node(UINode* _parent)
    {
        UINode* child = allocator.new_node();
        _parent->children.push_back(child);
        return child;
    }



    /** 
        Update the internal state of the ui based on user input.
        The data field will be passed to the callbacks.
    */
    void process_user_input(UserInput _user_input, void* _data)
    {

        if(_user_input.event_type == UserInputType::MouseMove)
        {
            MouseMovement& move = _user_input.event_data.move;
            current_pointer_pos = move.new_pos;

            UINode* new_hover_target = get_current_pointer_target();

            Print::buf("New hover target: ");
            Print::ln(Str::UI((unsigned long long)new_hover_target));

            bool hover_target_changed = new_hover_target != current_hover_target;
            if(!hover_target_changed) // only trigger callbacks when a change in target took place!
                return;

            bool current_hover_target_exists = current_hover_target != nullptr;
            
            // unhover old target
            if(current_hover_target_exists && hover_target_changed)
            {
                bool hover_target_has_unhover_handler = current_hover_target->handle_unhover != nullptr;
                if(hover_target_has_unhover_handler)
                    current_hover_target->handle_unhover(current_hover_target, _data);
            }
            

            // hover new target
            bool new_hover_target_exists = new_hover_target != nullptr;
            if(new_hover_target_exists)
            {
                if(new_hover_target->handle_hover != nullptr)
                    new_hover_target->handle_hover(new_hover_target, _data);
            }

            current_hover_target = new_hover_target;
        }
        else if(_user_input.event_type == UserInputType::MouseClick)
        {
            MouseClick& click = _user_input.event_data.mouse_click;
            MouseButtonAction button_action = click.action;

            UINode* hover_target = get_current_pointer_target();

            if(hover_target == nullptr)
                return;

            if(hover_target->handle_click == nullptr)
                return;

            // Only handle presses, not releases
            if(button_action == MouseButtonAction::Press)
                hover_target->handle_click(hover_target, _data);
        }
    }



    UINode* get_element_at_pos(d2 _pointer_pos)
    // bool contains_pointer_pos(d2 _pointer_pos)
    {
        // UINode& node = nodes.back()->value;
        if(root.box.contains(_pointer_pos))
            return &root;
        
        return nullptr;
    }

    UINode* get_current_pointer_target()
    {
        if(!root.box.contains(current_pointer_pos))
            return nullptr;
        
        // Confirmed inside root box
        // Now check descendants. If all descrendants are false, then return root
        bool start_button_check = root.children[0]->box.contains(current_pointer_pos);
        bool stop_button_check = root.children[1]->box.contains(current_pointer_pos);

        if(start_button_check)
            return root.children[0];
        else if(stop_button_check)
            return root.children[1];
        else
            return &root;
    }


};



struct UIString: public UINode
{
    Str str;
    uint font_size = 12;

    UIString() {}
    UIString(Str _str) : str {_str} {}
    UIString(Str _str, d2 _pos)
    {
        box.pos = _pos;
        visibility.set_bitmap();
        Bitmap& bitmap = get_string_bitmap();
        set_str(_str);
    }

    Bitmap& get_string_bitmap()
    {
        return *visibility.value.bitmap;
    }

    void set_str(Str _str)
    {
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

        Bitmap& font_bitmap = UI::get_bitmap_assets();

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
            str_bitmap.paste(bmp_letter, {(double)i*char_width, 0.0});
        }

        

    }
};
