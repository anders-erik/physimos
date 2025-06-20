#pragma once

#include <iostream>

#include <cstring>


template <typename T>
class Vec
{
    size_t count = 0;

public:
    T* t_p = nullptr;

    Vec() = default;
    /** Uninitialized memory? */
    Vec(size_t count)
    { 
        allocate(count);
    }
    /** Construct with vector filled with specific value. */
    Vec(size_t count, const T& inital_value)
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


    Vec<T>& operator*(T factor)
    {
        for(size_t i = 0; i < count; i++)
            *(t_p + i) *= factor;

        return *this;
    }

    Vec<T>& operator/(T factor)
    {
        for(size_t i = 0; i < count; i++)
            *(t_p + i) /= factor;

        return *this;
    }

    Vec<T>& operator+(T factor)
    {
        for(size_t i = 0; i < count; i++)
            *(t_p + i) += factor;

        return *this;
    }

    Vec<T>& operator-(T factor)
    {
        for(size_t i = 0; i < count; i++)
            *(t_p + i) -= factor;

        return *this;
    }

    Vec<T>& set(T value)
    {
        for(size_t i = 0; i < count; i++)
            *(t_p + i) = value;
        
        return *this;
    }

    T*          data_mut()          {return t_p ;}
    const T*    data()              {return t_p ;} const
    size_t      size() const        {return count ;}
    size_t      size_byte() const   {return (count * sizeof(T)) ;}


private:

    void allocate(size_t count)
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