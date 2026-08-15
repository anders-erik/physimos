
#pragma once

#include <cmath>

#include "lib/print.hh"
#include "lib/str.hh"
#include "lib/arr.hh"

void hello()
{
    println("Hello from audio header!");
}

Arr<int> create_sine_buffer()
{
    struct SineGeneratorConfig {
        uint duration = 1; // Total duration of the generated data in seconds
        uint sample_rate = 48; // samples per second
        uint sample_depth = 16; // 2^(sample_depth) number of available discrete values during sampling
        uint wave_freq = 48;
    };

    SineGeneratorConfig conf;

    uint total_sample_count = conf.duration * conf.sample_rate;
    double sample_dt = 1.0 / ((double)conf.sample_rate);


    Arr<int> buff {(uint)total_sample_count, (int)0};

    double sin_value = std::sin(1.67);
    print(Str::FL(sin_value, 3, Str::FloatRep::Fixed));
    print(Str::FL(sample_dt, 3, Str::FloatRep::Fixed));

    for( uint i = 0; i < total_sample_count; i++)
    {
        uint wave_remainder = i % conf.wave_freq; // 
        // double wave_fraction 
        buff[i] = 1;
    }
    return buff;
}

