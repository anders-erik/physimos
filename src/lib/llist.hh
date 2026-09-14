
#pragma once


/** 
    Linked list.
    Allocates each node individually on the heap. Fixed address.
 */
template <typename T>
struct LLNode {

    T value;
    LLNode<T>* next = nullptr;

    LLNode() = default;

    /** Create a new node with default T constructor and attaches it to back(). */
    LLNode<T>* append()
    {
        if(next != nullptr)
        {
            // TODO: Handle error resonably when trying to append to a node that is not at the back of the llist
            throw;
        }

        next = new LLNode<T>;

        return next;
    }

private: 

};


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
};