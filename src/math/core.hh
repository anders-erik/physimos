#pragma once


/** x^p */
template <typename T>
T pow(T x, size_t p)
{
    if(p == 0)
        return 1.0;

    T result = 1.0;
    for(size_t i = 0; i < p; i++)
        result *= x;
    
    return result;
}