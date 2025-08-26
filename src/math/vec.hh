#pragma once

#include "lib/str.hh"

#include <iostream>

#include <cstring>

typedef unsigned int uint;
using VecType = unsigned int;


template <typename T>
class Vec
{
    uint count = 0;

public:
    T* t_p = nullptr;

    Vec() = default;

    /** Uninitialized memory? */
    explicit
    Vec(uint count)
    { 
        allocate(count);
    }
    /** Size and value to fill with. */
    explicit
    Vec(uint count, T inital_value)
    {
        allocate(count);
        set(inital_value);
    }
    /** Copy construct. */
    Vec(const Vec<T>& vec) 
    {
        allocate(vec.size());
        memcpy(t_p, vec.t_p, count*sizeof(T));
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
        {
            std::cout << "WARN: this == &rhs in Vec copy assignment." << std::endl;
        }

        if(count != rhs.size())
        {
            deallocate();
            count = rhs.size(); 
        }
        memcpy(t_p, rhs.t_p, count*sizeof(T));
        return *this;
    }


    bool operator!=(const Vec<T>& rhs) { return *this == rhs ? false : true; }
    bool operator==(const Vec<T>& rhs)
    {
        if(count != rhs.size())
            return false;
        
        for(uint i = 0; i < count; i++)
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
        for(uint i = 0; i < count; i++)
            *(t_p + i) *= factor;

        return *this;
    }

    Vec<T>& operator/=(T factor)
    {
        for(uint i = 0; i < count; i++)
            *(t_p + i) /= factor;

        return *this;
    }

    Vec<T>& operator+=(T factor)
    {
        for(uint i = 0; i < count; i++)
            *(t_p + i) += factor;

        return *this;
    }

    Vec<T>& operator-=(T factor)
    {
        for(uint i = 0; i < count; i++)
            *(t_p + i) -= factor;

        return *this;
    }

    /** Vec[n] == value */
    Vec<T>& set(T value)
    {
        for(uint i = 0; i < count; i++)
            *(t_p + i) = value;
        
        return *this;
    }

    T*          data_mut()          {return t_p ;}
    const T*    data()              {return t_p ;} const
    uint      size() const        {return count ;}
    uint      size_byte() const   {return (count * sizeof(T)) ;}


    Str to_str()
    {
        Str str = "[";
        for(uint i = 0; i < count; i++)
        {
            str += Str::Num(t_p[i]);
            if(i+1 != count)
                str += ", ";
        }
        str += "]";
        return str;
    }

private:

    void allocate(uint count)
    {
        t_p = new T[count*sizeof(T)];
        this->count = count;
    }

    void deallocate()
    {
        if(t_p != nullptr)
        {
            delete[] t_p; 
            t_p = nullptr;
        }
        this->count = 0;
    }
};