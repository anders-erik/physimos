
#pragma once

#include <cmath>

#include "lib/print.hh"
#include "lib/arr.hh"

#include "math/const.hh"

#include "audio_data.hh"





struct Frequency
{
	double frequency;
	double amp; // [0, 1]
};
Frequency default_frequency = {1000.0, 0.25}; // Hz = osc. / s


/** Generates a frequency domain profile based on input functions. Purpose is to experiment with resulting timbres when tranformed into time domain.  */
struct FrequencyProfile
{
	Arr<Frequency> frequencies;

	void generate_3_overtones(double base_frequency)
	{
		Arr<Frequency> freq_tmp;

		freq_tmp.push_back({base_frequency * 1.0, 1.00});
		freq_tmp.push_back({base_frequency * 2.0, 0.30});
		freq_tmp.push_back({base_frequency * 3.0, 0.10});
		// freq_tmp.push_back({base_frequency * 4.0, 0.10});

		frequencies = freq_tmp;
	}
};

struct WaveConfig
{
	double duration = 1.0;
	uint sample_rate = 44100; // samples per second
	uint sample_depth_bit = 16; // 2^(sample_depth) number of available discrete values during sampling
	double gain = 0.5; // [0, 1]: maximum amplitude relative to the maximum values for the chosen bit depth.

	double damping = 0.0; // [0,1]: 0 = no damping. 1 = maximum damping. Damping model is subject to change, thus the input does not mean more than the magnitude of the hidden damping model


	// Derived quantities -- need getter & setters!
	uint sample_count;
	double dt;

	WaveConfig()
	{
		calculate_derived_quantities();
	};

	WaveConfig(double _duration)
	{
		duration = _duration;
		calculate_derived_quantities();
	};

	WaveConfig(double _duration, uint _sample_rate, uint _sample_depth_bit)
	{
		duration = _duration;
		sample_rate = _sample_rate;
		sample_depth_bit = _sample_depth_bit;

		calculate_derived_quantities();
	};

	void set_duration(double _duration)
	{
		duration = _duration;
		calculate_derived_quantities();
	}

	void set_gain(double _gain)
	{
		gain = _gain;
		calculate_derived_quantities();
	}

private:

	void calculate_derived_quantities()
	{
		sample_count = (uint) ((double)sample_rate * duration);
		dt = (double) (1.0 / (double)sample_rate);
	}

};

WaveConfig default_wave_config = {1.0, 44100, 16};


class WaveGen
{
public:

	WaveConfig config = default_wave_config;
	Arr<Frequency> wave_freqs;

	Arr<double> t_arr; // Time step array
    Arr<double> w_arr;	// Wave array
    Arr<int16_t> out_arr; // Output array


	WaveGen()
	{
		this->wave_freqs.set(default_frequency, 1);
		array_allocation();
	}
	WaveGen(double duration)
	{
		config.duration = duration;
		this->wave_freqs.set(default_frequency, 1);
		array_allocation();
	}
	WaveGen(double _duration, uint _frequency)
	{
		config.set_duration(_duration);
		this->wave_freqs.set({(double)_frequency, default_frequency.amp}, 1);
		// array_allocation();
	}
	WaveGen(double duration, Arr<Frequency> frequencies)
	{
		config.duration = duration;
		this->wave_freqs = frequencies;
		array_allocation();
	}
	// WaveGen(double duration, uint sample_rate, uint sample_depth, uint wave_freq)
	// {
	// 	this->wave_freqs.set({(double)wave_freq, default_frequency.amp}, 1);
	// 	array_allocation();
	// }

	void set_config(WaveConfig _config)
	{
		config = _config;
	}

	void set_frequencies(Arr<Frequency> _frequencies)
	{
		wave_freqs = _frequencies;
	}

	void array_allocation()
	{
		t_arr.clear();
		w_arr.clear();
		out_arr.clear();

		t_arr.reserve(config.sample_count);
		t_arr.set(0.0);
		w_arr.reserve(config.sample_count);
		w_arr.set(0.0);
		out_arr.reserve(config.sample_count);
		out_arr.set(0);
	}

	void generate_wave()
	{
		array_allocation();

		for(uint freq_i = 0; freq_i < wave_freqs.count(); freq_i++)
		{
			double freq = wave_freqs[freq_i].frequency;
			double amp = wave_freqs[freq_i].amp;
			double freq_mult = PI2 * freq;


			// Assemble individual frequencies
			for(uint i = 0; i < config.sample_count; i++)
			{
				double i_d = (double)i;

				t_arr[i] = config.dt * i_d;
				w_arr[i] += amp * sin( freq_mult * t_arr[i] );

			}
		}

		// find max frequency magnitude
		// TODO: this does only find the maximum, not the maximum absolute value of the wave
		double max_value = 0.0;
		for(uint i = 0; i < config.sample_count; i++)
		{
			if(w_arr[i] > max_value)
				max_value = w_arr[i];
		}

		// Apply damping
		for(uint i = 0; i < config.sample_count; i++)
			w_arr[i] /= ((double)config.sample_count / (double)(config.sample_count - i) );

		// normalize wave to [-1, 1]
		for(uint i = 0; i < config.sample_count; i++)
			w_arr[i] /= max_value; 

		// generate output wave
		double max_amplitude = pow(2.0, (double)(config.sample_depth_bit-1) ) - 1.0;
		for(uint i = 0; i < config.sample_count; i++)
			out_arr[i] = (int16_t) (w_arr[i] * max_amplitude * config.gain); // 32767.0
	}

	void print_wave()
	{
		for(uint i = 0; i < config.sample_count; i++)
		{
			print(Str::FL(t_arr[i], 5, Str::FloatRep::Fixed));
			print("  ");
			print(Str::SI(out_arr[i]));
			println();
		}
	}

	AudioData get_audio_data()
	{
		return AudioData {out_arr};
	}
};