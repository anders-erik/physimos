
#pragma once

#include <cmath>

// 8 bit output for now
template<typename From, typename To>
To hash(From i){

    // Get upper and lower half of bits
    size_t H_bitcount = sizeof(To) * 8;
    size_t bitcount_half = H_bitcount / 2;
    From low = i << bitcount_half;
    low = low >> bitcount_half;
    From high = i >> bitcount_half;

    // Swap low and high
    From h = low;
    h = h << bitcount_half;
    h += high;

    h = ~i;

    h += 2472214324233;
    h /= 7;
    h *= 0b101010101;
    h >>= 8;
    h /= 3;
    h += 1;

    
    // hash = hash << H_bits_over_2;

    return (To) h;
}