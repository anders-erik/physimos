#pragma once

// #include <optional>

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
    [[nodiscard]] inline T& get_ptr(){
        return t;
    };
    [[nodiscard]] inline bool is_null(){
        return t == nullptr ? true : false;
    }
    [[nodiscard]] inline bool has_value(){
        return t == nullptr ? false : true;
    }

};





/** A move-only optional container. */
template <typename T>
class OptMove {

    T *t; // object owned by OptMove
    bool value = false;

public:
    OptMove(OptMove const& other)   = delete;
    OptMove(OptMove&& other)        = delete;

    OptMove() {
        std::cout << "OptMove constructed without value." << std::endl;
    };
   ~OptMove() {
        if(value)
            deallocate_T();
    };

    // Only constructed by moving 
    OptMove(T&& _t){
        std::cout << "OptMove constructed by T move." << std::endl;
        // t = std::move(_t);
        allocate_T();
        *t = std::move(_t);
        // t = &_t;
        value = true;
        
    };

    OptMove& operator=(T&& _t) {
        std::cout << "OptMove got new value by Move." << std::endl;
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

