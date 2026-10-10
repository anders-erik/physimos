
#pragma once

#include <cmath>

#include "lib/print.hh"
#include "lib/arr.hh"

#include "math/const.hh"

#include "frequency.hh"
#include "frequency_profile.hh"
#include "audio_data.hh"



struct SineWave
{

	static AudioData32 generate_damped_wave(AudioSamplingConfig _sampling_config, f64 _frequency)
	{
		i64 sample_count = _sampling_config.sample_count;

		Arr<f64> t_arr; // Time step array
		Arr<f64> w_arr;	// Wave array
		Vec<i32> out_arr; // Output array

		t_arr.clear();
		t_arr.reserve(sample_count);
		t_arr.set(0.0);
		w_arr.clear();
		w_arr.reserve(sample_count);
		w_arr.set(0.0);

		out_arr.set_count(sample_count);
		out_arr.set_value(0);

		f64 amp = 1048576.0; // 2^20
		// f64 amp = 2097152.0; // 2^21
		// f64 amp = 4194304.0; // 2^22
		f64 freq_mult = PI2 * _frequency;
		f64 dt = _sampling_config.get_dt_s();


		// Assemble individual frequencies
		for(uint i = 0; i < sample_count; i++)
		{
			double i_d = (double)i;

			t_arr[i] = dt * i_d;
			w_arr[i] += amp * sin( freq_mult * t_arr[i] );
		}


		// find max frequency magnitude
		double max_value = 0.0;
		for(uint i = 0; i < sample_count; i++)
		{
			f64 abs_value = w_arr[i] > 0 ? w_arr[i] : -w_arr[i];

			if(abs_value > max_value)
				max_value = abs_value;
		}

		// Apply damping
		f64 damping_factor = 1.5;
		for(uint i = 0; i < sample_count; i++)
			w_arr[i] /= damping_factor * ((double)sample_count / (double)(sample_count - i) );

		// output array
		for(uint i = 0; i < sample_count; i++)
			out_arr[i] = (i32) w_arr[i];
		
		return AudioData32{out_arr, _sampling_config};
	}



	static AudioData32 generate_damped_wave(AudioSamplingConfig _sampling_config, f64 _frequency, f64 _damping)
	{
		i64 sample_count = _sampling_config.sample_count;

		Arr<f64> t_arr; // Time step array
		Arr<f64> w_arr;	// Wave array
		Vec<i32> out_arr; // Output array

		t_arr.clear();
		t_arr.reserve(sample_count);
		t_arr.set(0.0);
		w_arr.clear();
		w_arr.reserve(sample_count);
		w_arr.set(0.0);

		out_arr.set_count(sample_count);
		out_arr.set_value(0);


		f64 amp = 2097152.0; // 2^21
		// f64 amp = 4194304.0; // 2^22
		f64 freq_mult = PI2 * _frequency;
		f64 dt = _sampling_config.get_dt_s();


		// Assemble individual frequencies
		for(uint i = 0; i < sample_count; i++)
		{
			double i_d = (double)i;

			t_arr[i] = dt * i_d;
			w_arr[i] += amp * sin( freq_mult * t_arr[i] );
		}


		// find max frequency magnitude
		double max_value = 0.0;
		for(uint i = 0; i < sample_count; i++)
		{
			f64 abs_value = w_arr[i] > 0 ? w_arr[i] : -w_arr[i];

			if(abs_value > max_value)
				max_value = abs_value;
		}

		// Apply damping
		for(uint i = 0; i < sample_count; i++)
			w_arr[i] /= _damping * ((double)sample_count / (double)(sample_count - i) );

		// output array
		for(uint i = 0; i < sample_count; i++)
			out_arr[i] = (i32) w_arr[i];
		
		return AudioData32{out_arr, _sampling_config};
	}

};



struct PianoWave
{

	static AudioData16 generate(AudioSamplingConfig _sampling_config, f64 _frequency)
	{
		f64 frequency = _frequency;
		AudioSamplingConfig sampling_config = _sampling_config;
		AudioSamplingConfig sampling_config_050 {AudioSamplingConfig::RATE_DURATION, sampling_config.sample_rate_s, sampling_config.sample_count / 2};
		AudioSamplingConfig sampling_config_025 {AudioSamplingConfig::RATE_DURATION, sampling_config.sample_rate_s, sampling_config.sample_count / 4};



		AudioData32 audio_data_32;
		audio_data_32.set_sampling_config(_sampling_config);


		AudioData32 audio_data_32_1;
		AudioData32 audio_data_32_2;
		AudioData32 audio_data_32_3;
		AudioData32 audio_data_32_4;
		AudioData32 audio_data_32_5;
		AudioData32 audio_data_32_6;
		AudioData32 audio_data_32_7;
		AudioData32 audio_data_32_8;
		AudioData32 audio_data_32_9;
		AudioData32 audio_data_32_10;


		if(false)
		{
			audio_data_32_1 = 	SineWave::generate_damped_wave( sampling_config , frequency * 1.000);
			audio_data_32_2 = 	SineWave::generate_damped_wave( sampling_config , frequency * 2.000);
			audio_data_32_3 = 	SineWave::generate_damped_wave( sampling_config	, frequency * 3.001);
			audio_data_32_4 = 	SineWave::generate_damped_wave( sampling_config	, frequency * 4.020);
			audio_data_32_5 = 	SineWave::generate_damped_wave( sampling_config	, frequency * 5.050);
			audio_data_32_6 = 	SineWave::generate_damped_wave( sampling_config	, frequency * 6.080);
			audio_data_32_7 = 	SineWave::generate_damped_wave( sampling_config	, frequency * 7.120);
			audio_data_32_8 = 	SineWave::generate_damped_wave( sampling_config	, frequency * 8.190);
			audio_data_32_9 = 	SineWave::generate_damped_wave( sampling_config	, frequency * 9.250);
			audio_data_32_10 = 	SineWave::generate_damped_wave( sampling_config	, frequency * 10.35);
		}

		if(false)
		{
			audio_data_32_1 = 	SineWave::generate_damped_wave( sampling_config , frequency * 1.000);
			audio_data_32_2 = 	SineWave::generate_damped_wave( sampling_config , frequency * 2.000);
			audio_data_32_3 = 	SineWave::generate_damped_wave( sampling_config	, frequency * 3.001);
			audio_data_32_4 = 	SineWave::generate_damped_wave( sampling_config	, frequency * 4.020);
			audio_data_32_5 = 	SineWave::generate_damped_wave( sampling_config	, frequency * 5.050);
			audio_data_32_6 = 	SineWave::generate_damped_wave( sampling_config	, frequency * 6.080);
			audio_data_32_7 = 	SineWave::generate_damped_wave( sampling_config	, frequency * 7.120);
			audio_data_32_8 = 	SineWave::generate_damped_wave( sampling_config	, frequency * 8.190);
			audio_data_32_9 = 	SineWave::generate_damped_wave( sampling_config	, frequency * 9.250);
			audio_data_32_10 = 	SineWave::generate_damped_wave( sampling_config	, frequency * 10.35);
		}

		// Best so far!
		if(true)
		{
			audio_data_32_1 = SineWave::generate_damped_wave( sampling_config	, frequency * 1.000	, 1.0 );
			audio_data_32_2 = SineWave::generate_damped_wave( sampling_config	, frequency * 2.000	, 2.0 );	
			audio_data_32_3 = SineWave::generate_damped_wave( sampling_config	, frequency * 3.001	, 4.0 );
			audio_data_32_4 = SineWave::generate_damped_wave( sampling_config	, frequency * 4.020	, 7.0 );
			audio_data_32_5 = SineWave::generate_damped_wave( sampling_config	, frequency * 5.050	, 12.0);	
			audio_data_32_6 = SineWave::generate_damped_wave( sampling_config	, frequency * 6.080	, 20.0);
			audio_data_32_7 = SineWave::generate_damped_wave( sampling_config	, frequency * 7.120	, 40.0);
			audio_data_32_8 = SineWave::generate_damped_wave( sampling_config	, frequency * 8.190	, 50.0);
			audio_data_32_9 = SineWave::generate_damped_wave( sampling_config	, frequency * 9.250	, 55.0);
			audio_data_32_10 = SineWave::generate_damped_wave(sampling_config	, frequency * 10.35	, 55.0);
		}

		if(false)
		{
			audio_data_32_1 = 	SineWave::generate_damped_wave( sampling_config  	, frequency * 1.000);
			audio_data_32_2 = 	SineWave::generate_damped_wave( sampling_config  	, frequency * 2.000);
			audio_data_32_3 = 	SineWave::generate_damped_wave( sampling_config_050	, frequency * 3.001);
			audio_data_32_4 = 	SineWave::generate_damped_wave( sampling_config_050	, frequency * 4.020);
			audio_data_32_5 = 	SineWave::generate_damped_wave( sampling_config_050	, frequency * 5.050);
			audio_data_32_6 = 	SineWave::generate_damped_wave( sampling_config_025	, frequency * 6.080);
			audio_data_32_7 = 	SineWave::generate_damped_wave( sampling_config_025	, frequency * 7.120);
			audio_data_32_8 = 	SineWave::generate_damped_wave( sampling_config_025	, frequency * 8.190);
			audio_data_32_9 = 	SineWave::generate_damped_wave( sampling_config_025	, frequency * 9.250);
			audio_data_32_10 = 	SineWave::generate_damped_wave( sampling_config_025	, frequency * 10.35);
		}
		
		if(false)
		{
			AudioData32 audio_data_32_099 = SineWave::generate_damped_wave(_sampling_config, frequency*0.999);
			// AudioData32 audio_data_32_1 = SineWave::generate_damped_wave(_sampling_config, _frequency);
			AudioData32 audio_data_32_101 = SineWave::generate_damped_wave(_sampling_config, frequency*1.001);
			
			AudioData32 audio_data_32_199 = SineWave::generate_damped_wave(_sampling_config, frequency*1.999);
			// AudioData32 audio_data_32_2 =   SineWave::generate_damped_wave(_sampling_config, _frequency*2.00);
			AudioData32 audio_data_32_201 = SineWave::generate_damped_wave(_sampling_config, frequency*2.001);

			AudioData32 audio_data_32_299 = SineWave::generate_damped_wave(_sampling_config, frequency*2.999);
			// AudioData32 audio_data_32_3 =   SineWave::generate_damped_wave(_sampling_config, _frequency*3.00);
			AudioData32 audio_data_32_301 = SineWave::generate_damped_wave(_sampling_config, frequency*3.001);

			AudioData32 audio_data_32_399 = SineWave::generate_damped_wave(_sampling_config, frequency*3.999);
			// AudioData32 audio_data_32_4 =   SineWave::generate_damped_wave(_sampling_config, _frequency*4.00);
			AudioData32 audio_data_32_401 = SineWave::generate_damped_wave(_sampling_config, frequency*4.001);
		}

		AudioData32 audio_data_32_base_0 = SineWave::generate_damped_wave(_sampling_config, _frequency / 2.0);
		AudioData32 audio_data_32_base_1 = SineWave::generate_damped_wave(_sampling_config, 481.0);
		AudioData32 audio_data_32_base_2 = SineWave::generate_damped_wave(_sampling_config, 522.0);
		AudioData32 audio_data_32_base_3 = SineWave::generate_damped_wave(_sampling_config, 563.0);
		AudioData32 audio_data_32_base_4 = SineWave::generate_damped_wave(_sampling_config, 604.0);
		AudioData32 audio_data_32_base_5 = SineWave::generate_damped_wave(_sampling_config, 645.0);

		for(uint i = 0; i < audio_data_32.sample_count(); i++)
		{
			audio_data_32.data[i] = 0;

			// audio_data_32.data[i] += audio_data_32_base_0.data[i] / 4;
			// audio_data_32.data[i] += audio_data_32_base_1.data[i] / 20;
			// audio_data_32.data[i] += audio_data_32_base_2.data[i] / 20;
			// audio_data_32.data[i] += audio_data_32_base_3.data[i] / 20;
			// audio_data_32.data[i] += audio_data_32_base_4.data[i] / 20;
			// audio_data_32.data[i] += audio_data_32_base_5.data[i] / 20;

			// audio_data_32.data[i] += audio_data_32_099.data[i] / 5;
			// audio_data_32.data[i] += audio_data_32_101.data[i] / 5;

			// audio_data_32.data[i] += audio_data_32_199.data[i] / 6;
			// audio_data_32.data[i] += audio_data_32_201.data[i] / 6;

			// audio_data_32.data[i] += audio_data_32_299.data[i] / 8;
			// audio_data_32.data[i] += audio_data_32_301.data[i] / 8;
			
			// audio_data_32.data[i] += audio_data_32_399.data[i] / 8;
			// audio_data_32.data[i] += audio_data_32_401.data[i] / 8;

			// Based on the analysis of the exported DFT data generated using the sample of a real piano recording
			if(true)
			{
				audio_data_32.data[i] += audio_data_32_1.data[i] 	* 1.00;
				audio_data_32.data[i] += audio_data_32_2.data[i] 	* 0.20;
				audio_data_32.data[i] += audio_data_32_3.data[i] 	* 0.45;
				audio_data_32.data[i] += audio_data_32_4.data[i] 	* 0.40;
				audio_data_32.data[i] += audio_data_32_5.data[i] 	* 0.25;
				audio_data_32.data[i] += audio_data_32_6.data[i] 	* 0.33;
				audio_data_32.data[i] += audio_data_32_7.data[i] 	* 0.08;
				audio_data_32.data[i] += audio_data_32_8.data[i] 	* 0.08;
				audio_data_32.data[i] += audio_data_32_9.data[i] 	* 0.02;
				audio_data_32.data[i] += audio_data_32_10.data[i] 	* 0.04;
			}


			// High attack for the higher frequencies
			if(false)
			{
				if(i < sampling_config_025.sample_count)
				{
					audio_data_32.data[i] += audio_data_32_6.data[i] 	* 0.16;
					audio_data_32.data[i] += audio_data_32_7.data[i] 	* 0.08;
					audio_data_32.data[i] += audio_data_32_8.data[i] 	* 0.08;
					audio_data_32.data[i] += audio_data_32_9.data[i] 	* 0.02;
					audio_data_32.data[i] += audio_data_32_10.data[i] 	* 0.04;
				}

				if(i < sampling_config_050.sample_count)
				{
					audio_data_32.data[i] += audio_data_32_3.data[i]	* 0.45;
					audio_data_32.data[i] += audio_data_32_4.data[i]	* 0.40;
					audio_data_32.data[i] += audio_data_32_5.data[i]	* 0.25;
				}

				audio_data_32.data[i] += audio_data_32_1.data[i] * 1.00;
				audio_data_32.data[i] += audio_data_32_2.data[i] * 0.20;
			}
		}

		return audio_data_32.to_audio_data_i16();
	}

};



struct AudioWaveConfig
{
	double duration = 1.0;
	uint sample_rate = 44100; // samples per second
	uint sample_depth_bit = 16; // 2^(sample_depth) number of available discrete values during sampling
	double gain = 0.5; // [0, 1]: maximum amplitude relative to the maximum values for the chosen bit depth.

	double damping = 0.0; // [0,1]: 0 = no damping. 1 = maximum damping. Damping model is subject to change, thus the input does not mean more than the magnitude of the hidden damping model


	// Derived quantities -- need getter & setters!
	uint sample_count;
	double dt;

	AudioWaveConfig()
	{
		calculate_derived_quantities();
	};

	AudioWaveConfig(double _duration)
	{
		duration = _duration;
		calculate_derived_quantities();
	};

	AudioWaveConfig(double _duration, uint _sample_rate, uint _sample_depth_bit)
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


class AudioWaveGenerator
{
public:
	AudioFrequency default_frequency = {	1000.0, 
											0.25	};
	AudioWaveConfig config = {	1.0, 
								44100, 
								16		};

	Arr<AudioFrequency> wave_freqs;

	Arr<double> t_arr; // Time step array
    Arr<double> w_arr;	// Wave array
    Vec<int16_t> out_arr; // Output array


	AudioWaveGenerator()
	{
		this->wave_freqs.set(default_frequency, 1);
		array_allocation();
	}
	AudioWaveGenerator(double duration)
	{
		config.duration = duration;
		this->wave_freqs.set(default_frequency, 1);
		array_allocation();
	}
	AudioWaveGenerator(double _duration, uint _frequency)
	{
		config.set_duration(_duration);
		this->wave_freqs.set({(double)_frequency, default_frequency.amp}, 1);
		// array_allocation();
	}
	AudioWaveGenerator(double duration, Arr<AudioFrequency> frequencies)
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

	void set_config(AudioWaveConfig _config)
	{
		config = _config;
	}

	void set_frequencies(Arr<AudioFrequency> _frequencies)
	{
		wave_freqs = _frequencies;
	}

	void array_allocation()
	{
		t_arr.clear();
		w_arr.clear();
		// out_arr.clear();

		t_arr.reserve(config.sample_count);
		t_arr.set(0.0);
		w_arr.reserve(config.sample_count);
		w_arr.set(0.0);
		out_arr.set_count(config.sample_count);
		out_arr.set_value(0);
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

	AudioData16 get_audio_data()
	{
		return AudioData16 {out_arr};
	}
};