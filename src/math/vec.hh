#pragma once

#include "lib/str.hh"

#include <iostream>

#include <cstring>

typedef unsigned int uint;
using VecType = unsigned int;

/** Vector container used for numeric types. */
template <typename T>
class Vec
{
    uint alloc_elem_count = 0; // number of allocated elements

public:
    T* t_p = nullptr;

    Vec() = default;

    /** Uninitialized memory? */
    explicit
    Vec(uint _alloc_elem_count)
    { 
        allocate(_alloc_elem_count);
    }
    /** Size and value to fill with. */
    explicit
    Vec(uint _alloc_elem_count, T inital_value)
    {
        allocate(_alloc_elem_count);
        set(inital_value);
    }
    explicit
    Vec(uint _alloc_elem_count, T* _data)
    {
        allocate(_alloc_elem_count);
        memcpy(t_p, _data, alloc_elem_count*sizeof(T));
    }
    /** Copy construct. */
    Vec(const Vec<T>& vec) 
    {
        allocate(vec.size());
        memcpy(t_p, vec.t_p, alloc_elem_count*sizeof(T));
    }

    /** Beware: implicit conversion to type <T> is done by initializer list! */
    // Vec(std::initializer_list<T> init) 
    // {
    //     allocate((uint) init.size());
    //     std::copy(init.begin(), init.end(), t_p);
    // }


    ~Vec() 
    { 
        deallocate();
    }

    


    Vec<T>& operator=(const Vec<T>& rhs) 
    {
        if(this == &rhs)
            std::cout << "WARN: this == &rhs in Vec copy assignment." << std::endl;

        if(alloc_elem_count != rhs.size())
            set_size(rhs.size());

        memcpy(t_p, rhs.t_p, alloc_elem_count*sizeof(T));
        return *this;
    }


    bool operator!=(const Vec<T>& rhs) { return *this == rhs ? false : true; }
    bool operator==(const Vec<T>& rhs)
    {
        if(alloc_elem_count != rhs.size())
            return false;
        
        for(uint i = 0; i < alloc_elem_count; i++)
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


    Vec<T>& operator*=(T factor)
    {
        for(uint i = 0; i < alloc_elem_count; i++)
            *(t_p + i) *= factor;

        return *this;
    }

    Vec<T>& operator/=(T factor)
    {
        for(uint i = 0; i < alloc_elem_count; i++)
            *(t_p + i) /= factor;

        return *this;
    }

    Vec<T>& operator+=(T factor)
    {
        for(uint i = 0; i < alloc_elem_count; i++)
            *(t_p + i) += factor;

        return *this;
    }

    Vec<T>& operator-=(T factor)
    {
        for(uint i = 0; i < alloc_elem_count; i++)
            *(t_p + i) -= factor;

        return *this;
    }

    /** Vec[n] == value */
    Vec<T>& set(T value)
    {
        for(uint i = 0; i < alloc_elem_count; i++)
            *(t_p + i) = value;
        
        return *this;
    }


    T*          data_mut()          {return t_p ;}
    const T*    data()              {return t_p ;} const
    uint        size() const        {return alloc_elem_count ;}
    uint        count() const       {return alloc_elem_count ;}
    uint        size_byte() const   {return (alloc_elem_count * sizeof(T)) ;}
    uint        count_bytes() const {return (alloc_elem_count * sizeof(T)) ;}

    /** removes any existing data, allocates the requested size without inizalizing the data. */
    uint set_size(uint _alloc_size)
    {
        deallocate();
        allocate(_alloc_size);

        return alloc_elem_count;
    }


    Str to_str()
    {
        Str str = "[";
        for(uint i = 0; i < alloc_elem_count; i++)
        {
            str += Str::Num(t_p[i]);
            if(i+1 != alloc_elem_count)
                str += ", ";
        }
        str += "]";
        return str;
    }

private:

    void allocate(uint _alloc_elem_count)
    {
        t_p = new T[_alloc_elem_count*sizeof(T)];
        alloc_elem_count = _alloc_elem_count;
    }

    void deallocate()
    {
        if(t_p != nullptr)
        {
            delete[] t_p; 
            t_p = nullptr;
        }
        alloc_elem_count = 0;
    }
};