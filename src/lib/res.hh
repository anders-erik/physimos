
#pragma once

#include "err.hh"


template <typename Type>
struct Res {

    union ResUnion {
        Type type;
        Err error;
    } error_or_type;

    // ResUnion error_or_type;
    bool has_error = true;


    Res(Type type) : error_or_type { Type(type) },  has_error {false} {}
    Res(Err error) : error_or_type {error}, has_error {true} {}

    ~Res() {
        // if(has_error)
        //     type
        // ~ResUnion() {};
    };

};