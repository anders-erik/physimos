
#pragma once


/** 
    Linked List Node
*/
template <typename T>
struct LLNode
{
    T value;
    LLNode<T>* next = nullptr;
};


/* Linked list */
template <typename T>
struct LList
{
    LLNode<T>* head = nullptr;

    /** Count the number of nodes, including itself and the last one. */
    int count()
    {
        if(head == nullptr)
            return 0;

        int count = 1;
        LLNode<T>* current_node = head;

        while(current_node->next != nullptr)
        {
            current_node = current_node->next;
            ++count;
        }

        return count;
    }

    /** Get the last element in the linked list. */
    LLNode<T>* back()
    {
        LLNode<T>* current_back = head; // 

        while(current_back->next != nullptr)
            current_back = current_back->next;
        
        return current_back;
    }


    /** Create a new node with default T constructor and attaches it to back(). */
    LLNode<T>* append()
    {
        if(count() == 0)
        {
            head = new LLNode<T>;
            return head;
        }


        LLNode<T>* current_back = back();

        current_back->next = new LLNode<T>;

        return current_back->next;
    }

    /** Unsafe access. If the user specify a value not correspnding to an actual LiteNode, an exception is thrown. */
    T* operator[](int target_index)
    {
        int _count = count();
        if(target_index >= _count)
        {
            Print::ln("Accessing linked list byond the number of available elements.");
            throw;
        }

        // if(target_index == 0)
        //     return &head->value;


        LLNode<T>* current_back = head;

        for(int i = 0; i < target_index; i++)
        {
            current_back = current_back->next;
        }

        // while(current_back->next != nullptr)
        // {
        //     current_back = current_back->next;
        //     if(i == target_index)
        //     {
        //         return &current_back->value;
        //     }

        //     i++;
        // }
        
        return &current_back->value;
    }
};