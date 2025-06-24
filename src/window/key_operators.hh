#pragma once

#include "lib/str.hh"

#include "window/key.hh"




inline Key 
operator|(Key lhs, Key rhs)
{
    return static_cast<Key>(
        static_cast<unsigned char>(lhs) | static_cast<unsigned char>(rhs)
    );
}

inline Key& 
operator|=(Key& lhs, const Key& rhs)
{
    lhs = lhs | rhs;
    return lhs;
}


inline Key 
operator&(Key lhs, Key rhs)
{
    return static_cast<Key>(
        static_cast<unsigned char>(lhs) & static_cast<unsigned char>(rhs)
    );
}

inline Key& 
operator&=(Key& lhs, const Key& rhs)
{
    lhs = lhs & rhs;
    return lhs;
}


inline Key 
operator^(Key lhs, Key rhs)
{
    return static_cast<Key> (
        static_cast<unsigned char>(lhs) ^ static_cast<unsigned char>(rhs)
    );
}

inline Key& 
operator^=(Key& lhs, const Key& rhs)
{
    lhs = lhs ^ rhs;
    return lhs;
}




