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
    Box box;
    

    PX32 color = 0x558855FF;

    UINode() {}
    UINode(d2 _pos, d2 _size) : box {_pos, _size} {}

    /** 
        Params: state available to te ui node
        Return: state changes made by the current ui node & 'instructions' for the caller */
    void* event(void* _data)
    {

        return nullptr;
    }

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


struct UIEventData
{
    UserInput* i_event = nullptr;
    void* data; // arbitrary data made available to the UI
};

struct UI
{
    LList<UINode> nodes;
    UIEventData event_data;

    void add_node(UINode _node)
    {
        LLNode<UINode> * llnode = nodes.append();
        llnode->value = _node;
    }

    void* event(UIEventData* _data)
    {

        return nullptr;
    }

    /** Entry points for passing input events to the ui  */
    void* event(UserInput _ievent)
    {
        if(_ievent.event_type == UserInputType::MouseMove)
        {
            MouseMovement& move = _ievent.event_data.move;

            UINode& node = nodes.back()->value;
            if(node.box.contains(move.new_pos))
            {
                Print::ln("INSIDE!");
                node.dim();
            }
            else
            {
                node.undim();
            }
        }

        return nullptr;
    }

    bool contains_pointer_pos(d2 _pointer_pos)
    {
        UINode& node = nodes.back()->value;
        if(node.box.contains(_pointer_pos))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /** When no events are being handled by the ui, we undo all the highting that was added when the UI was the input target */
    void reset()
    {
        nodes.back()->value.undim();
    }
};


