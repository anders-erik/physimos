
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

    T x_stretch = 1.0;
    T x_shift = 0.0;
    T y_stretch = 1.0;
    T y_shift = 0.0;

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
            T x_transformed = (x / x_stretch) - x_shift;
            sum += coefficients[i] * pow(x_transformed, i);
        }

        return (sum * y_stretch) + y_shift;
    }

    Polynomial<T> derivative()
    {
        std::vector<T> new_coeff;

        for(size_t i=1; i<coefficients.size(); i++)
        {
            T coeff = i * coefficients[i];
            new_coeff.emplace_back(coeff);
        }

        return {new_coeff};
    }

};