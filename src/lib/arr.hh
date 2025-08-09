#pragma once

#include "lib/str.hh"

#include <iostream>

#include <cstring>

using SizeArr = uint;


template <typename T>
class Arr
{
    T* t_p = nullptr;
    uint reserved = 0;
    
    uint elem_count    = 0;

public:

    Arr() = default;
    /** Uninitialized memory? */
    Arr(uint reserve_size)
    { 
        allocate(reserve_size);
    }
    /** Construct with array filled with specific value. */
    Arr(uint elem_count, const T& inital_value)
    {
        allocate(elem_count);
        set(inital_value);
    }
    /** Copy construct. */
    Arr(const Arr<T>& arr) 
    {
        allocate(arr.count());
        elem_count = arr.count();
        for(SizeArr i=0; i < elem_count; i++)
            (*this)[i] = arr[i];
    }

    ~Arr() 
    { 
        deallocate();
    }

    


    Arr<T>& operator=(const Arr<T>& rhs) 
    {
        if(this == &rhs)
        {
            std::cout << "WARN: this == &rhs in Arr copy assignment." << std::endl;
            return *this;
        }

        if(elem_count < rhs.capacity())
        {
            reallocate(rhs.capacity());
        }
        elem_count = rhs.count();
        for(SizeArr i=0; i < elem_count; i++)
            (*this)[i] = rhs[i];
        return *this;
    }


    bool operator!=(const Arr<T>& rhs) { return *this == rhs ? false : true; }
    bool operator==(const Arr<T>& rhs)
    {
        if(elem_count != rhs.count())
            return false;
        
        for(uint i = 0; i < elem_count; i++)
        {
            if((*this)[i] != rhs[i])
                return false;
        }

        return true;
    }

    T& operator[](uint index) const
    {
        return *(t_p + index);
    }

    T& at(uint index) const
    {
        if(index >= reserved)
            throw std::runtime_error("Index out of bounds in Arr.at()");
        return *(t_p + index);
    }

    T& last() const
    {
        return *(t_p + elem_count - 1);
    }

    void pop_back()
    {
        elem_count--;
        t_p[elem_count].~T();
    }

    Arr<T>& set(const T& value)
    {
        elem_count = reserved;
        for(uint i = 0; i < elem_count; i++)
            *(t_p + i) = value;

        return *this;
    }




    T*        data_mut()                {return t_p ;}
    const T*  data()            const   {return t_p ;}
    uint      capacity()        const   {return reserved ;}
    uint      capacity_bytes()  const   {return (reserved * sizeof(T)) ;}
    uint      count()           const   {return elem_count ;}
    uint      count_byte()      const   {return (elem_count * sizeof(T)) ;}

    void reserve(uint new_count) 
    {
        if(t_p == nullptr)
            allocate(new_count);
        else if(new_count > elem_count) 
            reallocate(new_count);
    }

    void push_back(const T& t)
    {
        if(elem_count >= reserved)
            expand();
        (*this)[elem_count++] = t;
    }

    void expand()
    {
        if(t_p == nullptr)
            allocate(1);
        
        uint new_reserved = reserved * 2;
        while(new_reserved <= elem_count)
            new_reserved = reserved * 2;

        reallocate(new_reserved);
    }


    Str to_str()
    {
        Str str = "[";
        for(uint i = 0; i < elem_count; i++)
        {
            str += (*this)[i] + ", ";
        }
        str += "]";
        return str;
    }


private:

    void allocate(uint reserved)
    {
        t_p = new T[reserved*sizeof(T)];
        this->reserved = reserved;
    }

    void reallocate(uint new_reserved)
    {
        if(new_reserved == reserved)
            return;

        T* new_t_p = new T[new_reserved*sizeof(T)];

        if(new_reserved > reserved)
        {
            for(uint i = 0; i < elem_count; i++)
                *(new_t_p + i) = *(t_p + i);
        }
        else // shrink Arr
        {
            for(uint i = 0; i < new_reserved; i++)
                *(new_t_p + i) = *(t_p + i);
        }

        delete[] t_p;

        t_p   = new_t_p;
        reserved = new_reserved;
    }

    void deallocate()
    {
        if(t_p != nullptr)
        {
            delete[] t_p; 
            t_p = nullptr;
        }
        this->reserved = 0;
        this->elem_count = 0;
    }
};