#pragma once

// #include <optional>
#include <iostream>

#define MV std::move


/** A an optional pointer object to enforce nullptr check before use. */
template <typename T>
class OptPtr {

    T *t = nullptr;

public:

    OptPtr() = default;
 
    OptPtr(T* t) : t {t} {};

    [[nodiscard]] inline T& get_ref(){
        return *t;
    };
    [[nodiscard]] inline T* get_ptr(){
        return t;
    };
    [[nodiscard]] inline bool is_null(){
        return t == nullptr;
    }
    [[nodiscard]] inline bool has_value(){
        return t == nullptr ? false : true;
    }

};





/** A move-only optional container. */
template <typename T>
class OptMove
{
    T *t = nullptr; // object owned by OptMove
    bool value = false;

public:

    OptMove()                       = default;
    OptMove(OptMove const& other)   = delete;
    OptMove(OptMove&& other)        = delete;
    OptMove(T&& _t)
    {
        t = new T;
        *t = MV(_t);
        value = true;
    };
    ~OptMove() 
    {
        if(value)
        {
            delete t;
        }
    };

    OptMove& 
    operator=(T&& _t)
    {
        t = new T;
        *t = MV(_t);
        value = true;
        return *this;
    }

    [[nodiscard]] T&& 
    consume()
    {
        value = false;
        return MV(*t);
    };

    bool 
    has_value()
    {
        return value;
    }

};


/** A copy-only optional container. */
template <typename T>
class OptCopy {

    alignas(T) std::byte buffer[sizeof(T)];
    T t; // object owned by OptCopy
    bool value = false;

public:

    OptCopy(OptCopy const& other)   = delete;
    OptCopy(OptCopy&& other)        = delete;

    OptCopy() noexcept {
        std::cout << "OptCopy constructed without value." << std::endl;
    };
    ~OptCopy()                      = default;
    
    
    OptCopy(const T& _t){
        std::cout << "OptCopy constructed by T copy." << std::endl;
        new (buffer) T(value);
        value = true;
    };


    // OptCopy& operator=(const T& _t) {
    //     std::cout << "OptCopy got new value by Copy." << std::endl;
    //     t = _t;
    //     value = true;
    //     return *this;
    // }



    [[nodiscard]] T consume(){
        if (!value) throw std::runtime_error("No value");
        return t;
    };
    bool has_value(){
        return value;
    }

};

