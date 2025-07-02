
#pragma once

#include "opt.hh"
#include "err.hh"


/** 
    Result : value or error.
    Move only.
 */
template <typename Type>
class ResMove
{
    OptMove<Type> opt_value;
    OptMove<Err> opt_error;

public:

    ResMove(Type&& new_value)
    {
        opt_value = MV(new_value);
    }
    ResMove(Err&& new_error)
    {
        opt_error = MV(new_error);
    }

    bool has_value()
    {
        return opt_value.has_value();
    }

    Type&& consume_value() 
    {
        return opt_value.consume();
    }

    bool has_error()
    {
        return opt_error.has_value();
    }

    Err&& consume_error() 
    {
        return opt_error.consume();
    }
};





/** 
    Result : value or error.
    Move only.
 */
template <typename Type>
class ResCopy
{
    OptCopy<Type>   opt_value;
    OptCopy<Err>    opt_error;

public:

    bool has_value()
    {
        return opt_value.has_value();
    }

    Type get_value() 
    {
        return opt_value.get_value();
    }

    bool has_error()
    {
        return opt_error.has_value();
    }

    Err get_error() 
    {
        return opt_error.get_value();
    }
};