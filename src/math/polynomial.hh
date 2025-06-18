
#pragma once

#include <iostream>
#include <vector>

#include <concepts> // std::is_same_v

#include "math/core.hh"


template<typename T>
concept FloatType = std::is_same_v<T, float> || std::is_same_v<T, double>;


/** Polynomial using floating representation.
    Coefficient order is equivalent to array index. */
template <FloatType T>
class Polynomial 
{
public:

    std::vector<T> coefficients;

    Polynomial() = default;

    Polynomial(std::vector<T> new_coefficients)
    {  
        for(T coefficient : new_coefficients)
            coefficients.push_back(coefficient);
    }

    T operator()(T x)
    {
        return evaluate(x);
    }

    bool is_valid()
    {
        return coefficients.size() > 0 ? true : false;
    }

    size_t order()
    {
        return (coefficients.size() - 1);
    }

    T evaluate(T x)
    {
        T sum = 0.0;

        for(size_t i = 0; i < coefficients.size(); i++)
        {
            sum += coefficients[i] * pow(x, i);
        }

        return sum;
    }


};