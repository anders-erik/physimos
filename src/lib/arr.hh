#pragma once

#include <iostream>

#include <cstring>


template <typename T>
class Arr
{
    T* t_p = nullptr;
    size_t count = 0;

public:

    Arr() = default;
    /** Uninitialized memory? */
    Arr(size_t count)
    { 
        allocate(count);
    }
    /** Construct with array filled with specific value. */
    Arr(size_t count, const T& inital_value)
    {
        allocate(count);
        set(inital_value);
    }
    /** Copy construct. */
    Arr(const Arr<T>& vec) 
    {
        allocate(vec.size());
        memcpy(t_p, vec.data(), count*sizeof(T));
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

        if(count != rhs.size())
        {
            deallocate();
            count = rhs.size(); 
        }
        memcpy(t_p, rhs.data(), count*sizeof(T));
        return *this;
    }


    bool operator!=(const Arr<T>& rhs) { return *this == rhs ? false : true; }
    bool operator==(const Arr<T>& rhs)
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

    Arr<T>& set(T value)
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