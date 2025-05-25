
#pragma once

#include "str.hh"


/** Level. */
enum class err_l {
    Critical,
    Error,
    Warn,
};

/** Which physimos module. */
enum class err_m {
    Phile,
    Dir,
};

/** Type of error.  */
enum class err_t {
    ERRNO,
    parsing,
    filesystem,
};


/** Error info wrapper. */
struct Err {
    err_l   level = err_l::Error; // Error level
    err_m   module; // Which module the error took place in
    err_t   type; // error type
    Str     message; // error message

    int ERRNO = 0; // store ERRNO from C APIs

    Err(err_m module, err_t type, Str message): module{module}, type{type}, message{message} {};
};
