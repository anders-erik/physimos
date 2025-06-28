
#pragma once

#include "str.hh"


// struct err_s {
//     enum {
//         Critical,
//         Error,
//         Warn,
//     } type = Error;

// };

/** Severity. */
enum class err_s {
    Critical,
    Error,
    Warn,
};

/** Which physimos module. */
enum class err_m {
    Lib,
};

/** Type of error.  */
enum class err_t {
    ERRNO,
    parsing,
    filesystem,
};


/** Error info wrapper. */
struct Err {
    err_s   severity = err_s::Error; // Error level
    err_m   module; // Which module the error took place in
    err_t   type; // error type
    int     ERRNO = 0; // store ERRNO from C APIs
    Str     message; // error message

    Str     file; // File path in which the error took place


    Err() = default;
    Err(err_s severity, err_m module, err_t type, Str message)
        :   severity {severity},
            module  {module}, 
            type    {type}, 
            message {message} 
    {
    };
    /** With ERRNO */
    Err(err_s severity, err_m module, err_t type, int ERRNO, Str message)
        :   severity {severity},
            module  {module}, 
            type    {type},
            ERRNO   {ERRNO},
            message {message}
    {
    };

    /** Convert error object to string. */
    Str str();

};
