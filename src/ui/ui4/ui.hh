#pragma once

#include <cstdlib>

#include "lib/arr.hh"
#include "lib/bitmap.hh"
#include "lib/llist.hh"

#include "math/vecmat.hh"

#include "io/input/user_input.hh"

#include "box.hh"


struct UINode
{
    UINode* parent = nullptr;
    Arr<UINode*> children;

    Box box;
    PX32 color = 0x558855FF;

    void (*handle_click)(UINode*, void*) = nullptr;
    void (*handle_hover)(UINode*, void*) = nullptr;
    void (*handle_unhover)(UINode*, void*) = nullptr;
    

    UINode() {}
    UINode(d2 _pos, d2 _size) : box {_pos, _size} {}

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
    }
};

struct UI
{
    UINode root;
    UIAllocator allocator;

    d2 current_pointer_pos = {0.0, 0.0};
    UINode* current_hover_target = nullptr; 

    UI(unsigned long _max_number_ui_nodes)
        : allocator {_max_number_ui_nodes}
    {
    }

    UI()
        : allocator { 2 }
    {
        root.parent = nullptr;
    }

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


