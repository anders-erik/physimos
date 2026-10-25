#pragma once


#include "lib/bitmap.hh"
#include "lib/llist.hh"
#include "lib/arr.hh"

#include "math/vecmat.hh"

#include "io/input/user_input.hh"

#include "ui_node.hh"
#include "ui_event.hh"
#include "ui_font_bitmap.hh"


#define MAX_UI_NODES 100
#define FONT_SIZE_PX 30


struct UIAllocator
{
    UINode nodes[MAX_UI_NODES];
    bool occupied_slots[MAX_UI_NODES] {0};

    unsigned long max_node_count = MAX_UI_NODES;
    unsigned long node_count = 0;

    UIAllocator()
    {
        // occupied_slots.capacity
        // occupied_slots.set(false);
    }

    UINode* new_node()
    {
        for(uint i = 0; i < MAX_UI_NODES; i++)
        {

            if(!occupied_slots[i])
            {
                occupied_slots[i] = true;
                node_count++;
                return (nodes + i);
            }
        }

        Print::ln("ERROR: requested new UINode from allocator but there were no available space. ");

        return nullptr;
    }

    /** Logically clears a slot in the memory, and calls the UINode destructor to clear any held resources. */
    void delete_node(UINode* _node)
    {
        bool node_deletion_confirmed = false;

        for(uint i = 0; i < MAX_UI_NODES; i++)
        {
            if(occupied_slots[i])
            {
                if(_node == (nodes + i))
                {
                    occupied_slots[i] = false;
                    node_count--;
                    _node->~UINode();
                    node_deletion_confirmed = true;
                    break;
                }
            }
        }

        if(!node_deletion_confirmed)
            Print::ln("ERROR: requested UINode deletion from allocator failed. Unable to find a matching UINode in memory.");

        return;
    }

};


struct UI
{
    UINode root;

    d2 current_pointer_pos = {0.0, 0.0};

    UINode* current_hover_target = nullptr; // used to track when the pointer enters a new UINode
    UINode* current_grab_target = nullptr; // the node that returned the most recent mouse grab callback return, and not yet returned a release
    UINode* current_keyboard_target = &root; // the node that is currently recieving keyboard input

    // Arr<Bitmap> bitmap_assets;
    // static Bitmap font;

    UIAllocator allocator;

        
    /** Appends a new node to the provided parent node. Returns new node. */
    UINode* new_node(UINode* _parent)
    {
        UINode* child = allocator.new_node();

        *child = UINode(); // initialize memory to prevent garbage data

        child->parent = _parent;
        child->box.pos = _parent->box.pos;
        child->box.size = {10, 10};

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
                mouse_move_event.key_mods = _user_input.key_modifiers;
                d2 mouseMovementDelta = move.delta();
                mouse_move_event.value.mouse_move = UIMouseMove{    mouseMovementDelta.x, 
                                                                    mouseMovementDelta.y,
                                                                    move.new_pos.x,
                                                                    move.new_pos.y      };
                mouse_move_event.target_is_grabbed = true;

                
                if(current_grab_target->event_callback == nullptr)
                    return;
                
                UIEventReturn event_return = current_grab_target->event_callback(current_grab_target, mouse_move_event, _data);

                return;
            }

            UINode* new_hover_target = get_current_pointer_target();
            bool hover_target_changed = new_hover_target != current_hover_target;

            Print::buf("Hover target: ");
            Print::ln(Str::UI((unsigned long long)new_hover_target));

            if(!hover_target_changed) // only trigger callbacks when a change in target took place!
            {
                // Print::ln("UI move without target change");
                UIEvent ui_event {UIEvent::mouse_move};
                ui_event.key_mods = _user_input.key_modifiers;
                d2 mouseMovementDelta = move.delta();
                ui_event.value.mouse_move = UIMouseMove{    mouseMovementDelta.x, 
                                                            mouseMovementDelta.y,
                                                            move.new_pos.x,
                                                            move.new_pos.y      };

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

                bool current_hover_target_exists = current_hover_target != nullptr;
                
                // unhover old target
                if(current_hover_target_exists && hover_target_changed)
                {
                    if(current_hover_target->event_callback != nullptr)
                    {
                        UIEvent exit_event {UIEvent::mouse_exit};
                        exit_event.key_mods = _user_input.key_modifiers;
                        current_hover_target->event_callback(current_hover_target, exit_event, _data);
                    }
                }
                

                // hover new target
                bool new_hover_target_exists = new_hover_target != nullptr;
                if(new_hover_target_exists)
                {
                    if(new_hover_target->event_callback != nullptr)
                    {
                        UIEvent enter_event {UIEvent::mouse_enter};
                        enter_event.key_mods = _user_input.key_modifiers;
                        new_hover_target->event_callback(new_hover_target, enter_event, _data);
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
                    if(current_grab_target->event_callback != nullptr)
                    {
                        UIEvent mouse_release_event {UIEvent::mouse_release};
                        mouse_release_event.key_mods = _user_input.key_modifiers;
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
                if(new_click_target->event_callback != nullptr)
                {
                    UIEvent mouse_press_event {UIEvent::mouse_press};
                    mouse_press_event.key_mods = _user_input.key_modifiers;
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
            
            UIEvent key_event;
            key_event.key_mods = _user_input.key_modifiers;

            if(_user_input.event_data.key_input.action == KeyButtonAction::Press)
                key_event.type = UIEvent::key_press;
            else if(_user_input.event_data.key_input.action == KeyButtonAction::Release)
                key_event.type = UIEvent::key_release;



            { // New event callback
                if(current_keyboard_target->event_callback != nullptr)
                {
                    key_event.value.key_press.key = _user_input.event_data.key_input.key;
                    UIEventReturn return_value = current_keyboard_target->event_callback(current_keyboard_target, key_event, _data);
                }
            }

            return;
        }
        else if(_user_input.event_type == UserInputType::ScrollInput)
        {
            UIEvent scroll_event {UIEvent::mouse_scroll};
            scroll_event.key_mods = _user_input.key_modifiers;

            if(_user_input.event_data.scroll_input.scroll_direction == ScrollDirection::Up)
                scroll_event.value.scroll.direction = UIScroll::Up;
            else if(_user_input.event_data.scroll_input.scroll_direction == ScrollDirection::Down)
                scroll_event.value.scroll.direction = UIScroll::Down;

            if(current_hover_target == nullptr)
                return;
            
            if(current_hover_target->event_callback == nullptr)
                return;
            
            current_hover_target->event_callback(current_hover_target, scroll_event, _data);
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

