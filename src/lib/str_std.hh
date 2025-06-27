#pragma once

#include <string>

#include "str.hh"

struct StrStd
{

    static std::string to_string(const Str& str)
    {
        return std::string(str.data(), str.size()); 
    }
    /** Reference-like string without copying. */
    static std::string_view to_view(const Str& str)
    {
        return std::string_view(str.data(), str.size());
    }

};