#pragma once


#include "lib/bitmap.hh"
#include "lib/llist.hh"

#include "math/vecmat.hh"

#include "io/input/user_input.hh"

#include "ui_node.hh"
#include "ui_event.hh"
#include "ui_allocator.hh"
#include "ui_font_bitmap.hh"


struct UI
{
    UINode root;
    UIAllocator allocator;
    // Arr<Bitmap> bitmap_assets;
    // static Bitmap font;

    d2 current_pointer_pos = {0.0, 0.0};

    UINode* current_hover_target = nullptr; // used to track when the pointer enters a new UINode
    UINode* current_grab_target = nullptr; // the node that returned the most recent mouse grab callback return, and not yet returned a release
    UINode* current_keyboard_target = &root; // the node that is currently recieving keyboard input

    UI(unsigned long _max_number_ui_nodes)
        : allocator {_max_number_ui_nodes}
    {
        // init_bitmap_assets();
        // current_keyboard_target = &root;
    }

    UI()
        : allocator { 100 }
    {
        root.parent = nullptr;
        // init_bitmap_assets();
    }

    

    
    /** Appends a new node to the provided parent node. Returns new node. */
    UINode* new_node(UINode* _parent)
    {
        UINode* child = allocator.new_node();

        *child = UINode(); // initialize memory to prevent garbage data

        child->parent = _parent;

        _parent->children.push_back(child);

        return child;
    }

    /** Recursively deletes all descendants from the allocator. Leaves the current node unchanged. */
    void delete_node_descendants(UINode* _node)
    {

        for(uint i = 0; i < _node->children.count(); i++)
        {
            UINode* child = _node->children[i];
            if(child->children.count() > 0)
            {
                delete_node_descendants(child);
            }
            
            allocator.delete_node(child);
        }

        _node->children.clear();

        return;
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

            // Short circuit the input handling if ui is still waiting for the release of the left mouse button
            if(current_grab_target != nullptr)
            {
                UIEvent mouse_move_event {UIEvent::mouse_move};
                d2 mouseMovementDelta = move.delta();
                mouse_move_event.value.mouse_move = UIMouseMove{mouseMovementDelta.x, mouseMovementDelta.y};
                mouse_move_event.target_is_grabbed = true;

                
                if(current_grab_target->event_callback == nullptr)
                    return;
                
                UIEventReturn event_return = current_grab_target->event_callback(current_grab_target, mouse_move_event, _data);

                return;
            }

            UINode* new_hover_target = get_current_pointer_target();
            bool hover_target_changed = new_hover_target != current_hover_target;

            if(!hover_target_changed) // only trigger callbacks when a change in target took place!
            {
                // Print::ln("UI move without target change");
                UIEvent ui_event {UIEvent::mouse_move};
                d2 mouseMovementDelta = move.delta();
                ui_event.value.mouse_move = UIMouseMove{mouseMovementDelta.x, mouseMovementDelta.y};

                if(current_hover_target == nullptr)
                    return;
                
                if(current_hover_target->event_callback == nullptr)
                    return;
                
                UIEventReturn event_return = current_hover_target->event_callback(current_hover_target, ui_event, _data);


                return;
            }
            else
            {
                // Print::ln("UI target change");
                // Print::buf("New hover target: ");
                // Print::ln(Str::UI((unsigned long long)new_hover_target));

                bool current_hover_target_exists = current_hover_target != nullptr;
                
                // unhover old target
                if(current_hover_target_exists && hover_target_changed)
                {
                    bool hover_target_has_unhover_handler = current_hover_target->handle_mouse_exit != nullptr;
                    if(hover_target_has_unhover_handler)
                    {
                        current_hover_target->handle_mouse_exit(current_hover_target, _user_input, _data);
                    }
                    if(current_hover_target->event_callback)
                    {
                        UIEvent exit_event {UIEvent::mouse_exit};
                        current_hover_target->event_callback(current_hover_target, exit_event, _data);
                    }
                }
                

                // hover new target
                bool new_hover_target_exists = new_hover_target != nullptr;
                if(new_hover_target_exists)
                {
                    if(new_hover_target->handle_mouse_enter != nullptr)
                    {
                        new_hover_target->handle_mouse_enter(new_hover_target, _user_input, _data);
                    }
                    if(new_hover_target->event_callback != nullptr)
                    {
                        UIEvent enter_event {UIEvent::mouse_enter};
                        new_hover_target->event_callback(current_hover_target, enter_event, _data);
                    }
                }

                current_hover_target = new_hover_target;
            }
        }
        else if(_user_input.event_type == UserInputType::MouseClick)
        {
            MouseClick& click = _user_input.event_data.mouse_click;
            MouseButtonAction button_action = click.action;


            if(button_action == MouseButtonAction::Release)
            {
                if(current_grab_target != nullptr)
                {
                    if(current_grab_target->handle_mouse_release != nullptr)
                    {
                        current_grab_target->handle_mouse_release(current_grab_target, _user_input, _data);
                    }

                    if(current_grab_target->event_callback != nullptr)
                    {
                        UIEvent mouse_release_event {UIEvent::mouse_release};
                        current_grab_target->event_callback(current_grab_target, mouse_release_event, _data);
                    }
                    current_grab_target = nullptr;
                }

                return;
            }


            UINode* new_click_target = get_current_pointer_target();

            // Try to click new target
            if(new_click_target != nullptr)
            {
                if(new_click_target->handle_mouse_click != nullptr)
                {
                    new_click_target->handle_mouse_click(new_click_target, _user_input, _data);
                }

                if(new_click_target->event_callback != nullptr)
                {
                    UIEvent mouse_press_event {UIEvent::mouse_press};
                    UIEventReturn return_value = new_click_target->event_callback(new_click_target, mouse_press_event, _data);

                    if(return_value.type == UIEventReturn::node_keyboard_grab)
                        current_keyboard_target = new_click_target;

                }
            }
            current_grab_target = new_click_target;
            
            Print::buf("New click target: ");
            Print::ln(Str::UI((unsigned long long)new_click_target));
        }
        else if(_user_input.event_type == UserInputType::KeyInput)
        {
            if(current_keyboard_target == nullptr)
                return;

            { // New event callback
                if(current_keyboard_target->event_callback != nullptr)
                {
                    UIEvent key_event {UIEvent::key_press};
                    UIEventReturn return_value = current_keyboard_target->event_callback(current_keyboard_target, key_event, _data);
                }
            }
            
            { // New old user input callback
                if(current_keyboard_target->handle_key_press != nullptr)
                {
                    current_keyboard_target->handle_key_press(current_keyboard_target, _user_input, _data);
                }
            }

            return;
        }
        else if(_user_input.event_type == UserInputType::ScrollInput)
        {
            UINode* new_hover_target = get_current_pointer_target();

            if(current_hover_target == nullptr)
                return;
            
            if(current_hover_target->handle_scroll == nullptr)
                return;
            
            current_hover_target->handle_scroll(current_keyboard_target, _user_input, _data);
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

    /* Requery the UI tree for current target */
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

            str_bitmap.paste2(bmp_letter, {(double)i*char_width-100, -10.0});
            str_bitmap.paste2(bmp_letter, {(double)i*char_width, 20.0});
            str_bitmap.paste2(bmp_letter, {(double)i*char_width+20, 0.0});
        }

    }
};
