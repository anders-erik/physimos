#pragma once

#include <cstdlib>

#include "lib/arr.hh"

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
