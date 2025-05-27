#pragma once

// #include <optional>

/** A move-only optional container. */
template <typename T>
class Opt {

    T *t; // object owned by Opt
    bool value = false;

public:
    Opt(Opt const& other)   = delete;
    Opt(Opt&& other)        = delete;

    Opt() {
        std::cout << "Opt constructed without value." << std::endl;
    };
   ~Opt() {
        if(value)
            deallocate_T();
    };
    
    
    // Opt(const T& _t){
    //     std::cout << "Opt constructed by T copy." << std::endl;
    //     t = _t;
    //     value = true;
    // };

    // Only constructed by moving 
    Opt(T&& _t){
        std::cout << "Opt constructed by T move." << std::endl;
        // t = std::move(_t);
        allocate_T();
        *t = std::move(_t);
        // t = &_t;
        value = true;
        
    };



    // Opt& operator=(const T& _t) {
    //     std::cout << "Opt got new value by Copy." << std::endl;
    //     t = _t;
    //     value = true;
    //     return *this;
    // }
    Opt& operator=(T&& _t) {
        std::cout << "Opt got new value by Move." << std::endl;
        allocate_T();
        *t = std::move(_t);
        value = true;
        // t = std::move(_t);
        return *this;
    }



    [[nodiscard]] T&& consume(){
        value = false;
        return std::move(*t);
    };
    bool has_value(){
        return value;
    }

private:

    void allocate_T(){
        t = new T;
    }
    void deallocate_T(){
        delete t;
    }
};

