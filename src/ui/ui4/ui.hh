#pragma once

#include "lib/arr.hh"
#include "lib/bitmap.hh"
#include "lib/llist.hh"

#include "math/vecmat.hh"

#include "io/input/user_input.hh"


struct Box
{
    d2 pos; // position
    d2 size; // size

    Box() {}
    Box(d2 _pos, d2 _size) : pos {_pos}, size {_size} {}

    bool contains(d2 _p)
    {
        bool x_check = _p.x > pos.x && _p.x < pos.x + size.x;
        bool y_check = _p.y > pos.y && _p.y < pos.y + size.y;

        return x_check && y_check;
    }
};  


struct UINode
{
    UINode* parent = nullptr;
    UINode* child = nullptr;

    void (*handle_click)(UINode*, void*) = nullptr;
    void (*handle_hover)(UINode*, void*) = nullptr;
    void (*handle_unhover)(UINode*, void*) = nullptr;
    

    Box box;
    PX32 color = 0x558855FF;

    UINode() {}
    UINode(d2 _pos, d2 _size) : box {_pos, _size} {}

    void dim() { color = 0x335533FF; }
    void undim() { color = 0x558855FF; }
};


template <typename T>
struct Tree
{
    T value;
    T parent;
    Arr<T> children;

    Tree() {}
    Tree(T& _parent) : parent { _parent } {}

    bool is_leaf() {return children.count() == 0 ? true : false; }
    uint child_count() { return children.count(); }
};



struct UI
{

    UINode root;

    d2 current_pointer_pos = {0.0, 0.0};
    UINode* current_hover_target = nullptr; 

    UI()
    {
        root.box = Box({300, 300}, {50, 50});
    }

    // void add_node(UINode _node)
    // {
    //     LLNode<UINode> * llnode = nodes.append();
    //     llnode->value = _node;
    // }

    // void* event(UIEventData* _data)
    // {

    //     return nullptr;
    // }

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
        if(root.box.contains(current_pointer_pos))
            return &root;
        else
            return nullptr;
    }


    /** When no events are being handled by the ui, we undo all the highting that was added when the UI was the input target */
    void set_no_active_events()
    {
        root.undim();
    }
};


