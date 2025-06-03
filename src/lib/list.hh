
#pragma once


/** 
    Linked list.
    Allocates each node individually on the heap. Fixed address.
 */
template <typename T>
struct LListNode {

    T value;
    LListNode<T>* next = nullptr;

    LListNode() = default;

    /** Create a new node with default T constructor and attaches it to back(). */
    LListNode<T>* append(){

        LListNode<T>* current_back = back();

        current_back->next = new LListNode<T>;

        return current_back->next;
    }

    /** Get the last element in the linked list. */
    LListNode<T>* back(){

        LListNode<T>* current_back = this;

        while(current_back->next != nullptr)
            current_back = current_back->next;
        
        return current_back;
    }

    /** Count the number of nodes, including itself and the last one. */
    int count_back(){
        int count = 1;

        LListNode<T>* current_back = this;

        while(current_back->next != nullptr){
            current_back = current_back->next;
            ++count;
        }

        return count;
    }

private: 

};