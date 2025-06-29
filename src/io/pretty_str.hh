#pragma once

#include "lib/str.hh"

#include "math/vec.hh"
#include "math/number.hh"

// Forward declares

struct Err;
template <typename T>
struct Vec;
template <typename T>
struct R2;

namespace Pretty
{

Str Spaces(int count);

Str StrS(bool _bool);
Str StrS(unsigned char uch);
Str StrS(long long _integer);
Str StrS(int _integer);

Str StrS(const Err& err);

Str StrM(Str& indent, bool _bool);
Str StrM(Str& indent, char ch);
Str StrM(Str& indent, int _int);
Str StrM(Str& indent, uint _uint);
Str StrM(Str& indent, double dbl);
Str StrM(Str& indent, const Err& err);
Str StrM(Str& indent, Str& str);

template <typename T>
Str StrM(Str& indent, Vec<T>& vec)
{
    return indent + vec.to_str() + "\n";
}

template <typename T>
Str StrM(Str& indent, R2<T>& r2)
{
    return indent + r2.to_str() + "\n";
}


/** Single line pretty string */
template <typename T>
Str StrS( T t)
{
    return StrS(t);
}

/** Multiple line pretty string */
template <typename T>
Str StrM(unsigned int indent_count, T t)
{
    Str indent {indent_count, ' '};
    return StrM(indent, t);
}


}