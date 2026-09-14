#pragma once

#include "lib/arr.hh"
#include "lib/llist.hh"

#include "math/vecmat.hh"

#include "swrend/bitmap.hh"

struct UINode
{
    d2 pos;
    d2 size;

    PX32 color = 0x558855FF;

    UINode() {}
    UINode(d2 _pos, d2 _size) : pos {_pos}, size {_size} {}
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
    LList<UINode> nodes;


    void add_node(UINode _node)
    {
        LLNode<UINode> * llnode = nodes.append();
        llnode->value = _node;
    }
};

