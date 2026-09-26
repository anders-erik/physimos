#pragma once


#include "lib/bitmap.hh"
#include "lib/llist.hh"

#include "math/vecmat.hh"

#include "io/input/user_input.hh"

#include "ui_node.hh"
#include "ui_allocator.hh"


struct UI
{
    UINode root;
    UIAllocator allocator;
    // Arr<Bitmap> bitmap_assets;
    // static Bitmap font;

    d2 current_pointer_pos = {0.0, 0.0};
    UINode* current_hover_target = nullptr;
    UINode* current_keyboard_target = &root;

    UI(unsigned long _max_number_ui_nodes)
        : allocator {_max_number_ui_nodes}
    {
        init_bitmap_assets();
        // current_keyboard_target = &root;
    }

    UI()
        : allocator { 100 }
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

        *child = UINode(); // initialize memory to prevent garbage data

        child->parent = _parent;

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
                    current_hover_target->handle_unhover(current_hover_target, _user_input, _data);
            }
            

            // hover new target
            bool new_hover_target_exists = new_hover_target != nullptr;
            if(new_hover_target_exists)
            {
                if(new_hover_target->handle_hover != nullptr)
                    new_hover_target->handle_hover(new_hover_target, _user_input, _data);
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
                hover_target->handle_click(hover_target, _user_input, _data);
        }
        else if(_user_input.event_type == UserInputType::KeyPress)
        {
            // Print::ln("Keypress recieved in the UI!");
            if(current_keyboard_target == nullptr)
                return;
            
            if(current_keyboard_target->handle_key_press == nullptr)
                return;
            
            current_keyboard_target->handle_key_press(current_keyboard_target, _user_input, _data);
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
        
        UINode* matched_node = search_tree(&root);
        
        return matched_node;
    }

private:

    bool contains_pointer(UINode* _node)
    {
        return _node->box.contains(current_pointer_pos);
    }

    /* Returns the matched child. If there is no match the provided parent node is returned instead. */
    UINode* search_tree(UINode* _node)
    {
        for(uint i = 0; i < _node->children.count(); i++)
        {
            if(contains_pointer(_node->children[i]))
            {
                return search_tree(_node->children[i]);
            }
        }

        return _node;
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
            // str_bitmap.paste(bmp_letter, {(double)i*char_width, 0.0});

            str_bitmap.paste2(bmp_letter, {(double)i*char_width-100, -10.0});
            str_bitmap.paste2(bmp_letter, {(double)i*char_width, 20.0});
            str_bitmap.paste2(bmp_letter, {(double)i*char_width+20, 0.0});
        }

    }
};
