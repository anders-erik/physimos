#pragma once

#include "lib/str.hh"

// Forward declares

struct Err;


namespace Pretty
{
Str Spaces(int count);

Str StrSL(bool _bool);
Str StrML(unsigned char indent_count, bool _bool);

Str StrSL(unsigned char uch);
Str StrSL(long long _integer);
Str StrSL(int _integer);


Str StrSL(const Err& err);
Str StrML(unsigned char indent_count, const Err& err);


/** Single line pretty string */
template <typename T>
Str StrSL( T t)
{
    return StrSL(t);
}

/** Multiple line pretty string */
template <typename T>
Str StrML(unsigned char indent_count, T t)
{
    return Spaces(indent_count) + StrML(t);
}

}