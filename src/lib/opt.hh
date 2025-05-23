
#pragma once

template <typename T>
struct Opt {
    T t;
    bool value = false;
    Opt(T _t): t {_t} {value = true;};
    Opt() {};
};