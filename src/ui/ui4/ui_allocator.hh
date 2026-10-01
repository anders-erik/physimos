#pragma once

#include <cstdlib>

#include "lib/arr.hh"

struct UIAllocator
{
    UINode* nodes;
    unsigned long max_node_count;
    unsigned long node_count = 0;
    Arr<bool> occupied_slots; // value of true if occupied

    UIAllocator(unsigned long _max_node_count)
        :   occupied_slots {Arr<bool> {_max_node_count, false}}
    {
        max_node_count = _max_node_count;
        nodes = (UINode*) malloc(sizeof(UINode) * max_node_count);
        occupied_slots.set(0);
    }

    UINode* new_node()
    {
        for(uint i = 0; i < occupied_slots.count(); i++)
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

        for(uint i = 0; i < occupied_slots.count(); i++)
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



    ~UIAllocator()
    {
        free(nodes);
        nodes = nullptr;
    }
};
