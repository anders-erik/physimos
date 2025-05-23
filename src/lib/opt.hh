
#pragma once

template <typename T>
class Opt {
    T t;
    bool value = false;

public:
    Opt(T _t){
        t = _t;
        value = true;
    };
    Opt() {};


    T get_value(){
        return t;
    };
    bool has_value(){
        return value;
    }
};