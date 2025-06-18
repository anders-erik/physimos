#pragma once

#include <cstring>


template <typename T>
struct Vec
{
    T* t_p = nullptr;
    size_t count = 0;

    Vec() = default;
    /** Uninitialized memory? */
    Vec(size_t count) : count {count} { t_p = new T[count*sizeof(T)];}
    /** Construct with vector filled with specific value. */
    Vec(size_t count, const T& inital_value) : count {count} { t_p = new T[count*sizeof(T)]; set(inital_value);}
    /** Copy construct. */
    Vec(const Vec& vec) : count {vec.count} { t_p = new T[count*sizeof(T)]; memcpy(t_p, vec.t_p, count*sizeof(T));}

    ~Vec() { if(t_p != nullptr) delete t_p; }

    Vec& operator=(const Vec& rhs) { count = rhs.count; t_p = new T[count*sizeof(T)]; memcpy(t_p, rhs.t_p, count); }

    bool operator!=(const Vec& rhs) { return *this == rhs ? false : true; }
    bool operator==(const Vec& rhs)
    {
        if(count != rhs.count)
            return false;
        
        for(size_t i = 0; i < count; i++)
        {
            if((*this)[i] != rhs[i])
                return false;
        }

        return true;
    }

    T& operator[](size_t index) const
    {
        return *(t_p + index);
    }


    Vec& operator*(T factor)
    {
        for(size_t i = 0; i < count; i++)
            *(t_p + i) *= factor;

        return *this;
    }

    Vec& operator/(T factor)
    {
        for(size_t i = 0; i < count; i++)
            *(t_p + i) /= factor;

        return *this;
    }

    Vec& operator+(T factor)
    {
        for(size_t i = 0; i < count; i++)
            *(t_p + i) += factor;

        return *this;
    }

    Vec& operator-(T factor)
    {
        for(size_t i = 0; i < count; i++)
            *(t_p + i) -= factor;

        return *this;
    }

    Vec& set(T value)
    {
        for(size_t i = 0; i < count; i++)
            *(t_p + i) = value;
        
        return *this;
    }


};