
#pragma once

#include <cmath>
#include "math/const.hh"


#include "lib/print.hh"
#include "lib/str.hh"
#include "lib/arr.hh"




Arr<int> create_sine_buffer()
{
    struct SineGeneratorConfig {
        uint duration = 1; // Total duration of the generated data in seconds
        uint sample_rate = 48000; // samples per second
        uint sample_depth = 16; // 2^(sample_depth) number of available discrete values during sampling
        uint wave_freq = 200; // Hz = osc. / s
    };
    SineGeneratorConfig conf;

    uint total_sample_count = conf.duration * conf.sample_rate;
    double sample_dt = 1.0 / ((double)conf.sample_rate);
    println(Str::FL(sample_dt, 6, Str::FloatRep::Fixed));

    // Sine constants
    double sine_amp = pow(2.0, conf.sample_depth-1) - 1.0;
    double sine_freq_mult = PI2 * conf.wave_freq;

    // Arrays: time, sine (double), output (int)
    Arr<double> t_arr {(uint)total_sample_count, (double)0.0};
    Arr<double> s_arr {(uint)total_sample_count, (double)0.0};
    Arr<int> out {(uint)total_sample_count, (int)0};

    // populate vectors
    for(uint i = 0; i < total_sample_count; i++)
    {
        double i_d = (double)i;
        t_arr[i] = sample_dt * i_d;

        s_arr[i] = sine_amp * sin( sine_freq_mult * t_arr[i] );

        out[i] = (int) s_arr[i];

        // print(Str::FL(t_arr[i], 5, Str::FloatRep::Fixed));
        // print("  ");
        // print(Str::SI(out[i]));
        // println();
    }

    return out;
}

