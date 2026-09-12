#pragma once


template <typename T>
T& min(T& a, T& b)
{
    return a > b ? b : a;
}

template <typename T>
T& Min(T& a, T& b)
{
    return a > b ? b : a;
}

template <typename T>
T& Min(T& a, T& b, T& c)
{
    T& first = min(a, b);
    return first < c ? first : c;
}

template <typename T>
T& Max(T& a, T& b)
{
    return a > b ? a : b;
}

template <typename T>
T& Max(T& a, T& b, T& c)
{
    T& first = Max(a, b);
    return first > c ? first : c;
}

