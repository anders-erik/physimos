
#pragma once

template <typename T>
class Opt {

    T t;
    bool value = false;

public:
    Opt()                   = default; // value = false
    Opt(Opt const& other)   = default;
    Opt(Opt&& other)        = default;
   ~Opt()                   = default;
    
    
    Opt(const T& _t){
        std::cout << "Opt constructed by T copy." << std::endl;
        t = _t;
        value = true;
        
    };
    Opt(T&& _t){
        std::cout << "Opt constructed by T move." << std::endl;
        t = std::move(_t);
        value = true;
        
    };



    Opt& operator=(const T& _t) {
        std::cout << "Opt got new value by Copy." << std::endl;
        t = _t;
        value = true;
        return *this;
    }
    Opt& operator=(T&& _t) {
        std::cout << "Opt got new value by Move." << std::endl;
        value = true;
        t = std::move(_t);
        return *this;
    }



    T&& consume(){
        return std::move(t);
    };
    bool has_value(){
        return value;
    }

};

