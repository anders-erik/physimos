
#pragma once

#include <cmath>

#include "lib/arr.hh"
#include "lib/defs.hh"
#include "math/vec.hh"




struct AudioSamplingConfig
{
	enum Type {
		RATE_COUNT, 	// samples per second & number of samples
		RATE_DURATION,	// samples per second & total sampling time in microseconds
		COUNT_DURATION,	// number of samples & total sampling time in microseconds
	};

	u64 sample_rate_s = 0;
	u64 sample_count = 0;
	u64 duration_us = 0;

	// u64 bit_depth = 32;


	AudioSamplingConfig() = default;

	AudioSamplingConfig(AudioSamplingConfig::Type type, u64 arg_1, u64 arg_2)
	{
		set_values(type, arg_1, arg_2);
	}

	void set_values(AudioSamplingConfig::Type type, u64 arg_1, u64 arg_2)
	{
		if(type == RATE_COUNT)
		{
			sample_rate_s = arg_1;
			sample_count = arg_2;

			f64 duration_s = (f64)sample_count / (f64)sample_rate_s;
			duration_us = (u64) duration_s * 1000000;
		}
		else if(type == RATE_DURATION)
		{
			sample_rate_s = arg_1;
			duration_us = arg_2;

			sample_count = duration_us * sample_rate_s / 1000000;
		}
		else if(type == COUNT_DURATION)
		{
			sample_count = arg_1;
			duration_us = arg_2;

			f64 duration_s = (f64)duration_us / 1000000.0;

			sample_rate_s = (u64) ( (f64)sample_count / duration_s);
		}
	}
	
	f64 get_duration_s()
	{
		return ((f64)duration_us) / 1000000;
	}

	f64 get_dt_s()
	{
		f64 duration_s = get_duration_s();
		f64 sample_count_f64 = (f64) sample_count;
		return duration_s / sample_count_f64;
	}
};




/* 	
	Core object for raw audio data.
	Implicit audio settings:
		- Sample rate: 44100
		- Channels: 1 (mono)
		- DataType: 16bit signed integer
*/
class AudioData16
{
public:

	Vec<int16_t> data;

	AudioData16() {};
	AudioData16(uint _data_count)
	{
		data.set_count(_data_count);
		data.set_value(0);
	};
	AudioData16(Vec<int16_t>& data)
	{
		this->data = data;
	};

	// force vector to hold a specific sample count, all set to zero
	void set_sample_count(uint _count)
	{
		data.set_count(_count);
		data.set_value(0);
	}

	int sample_rate() { return 44100; }
	int channel_count() { return 1; }
	int sample_size_bit() { return 16; }
	int sample_size_byte() { return 2; }

	uint sample_count() { return data.count(); }
	int data_size_byte() { return data.count() * 2; }

	double duration_s_double() { return ((double)sample_count()) / ((double)sample_rate()); }
	int duration_ms_int() { return (int) (duration_s_double() * 1000); } // rounds according to double to int cast rounding rules
};




/*
	Audio data with 32-bit samples.
	On conversion to output format the values will be cliped using 24-bit
*/
class AudioData32
{
public:

	Vec<i32> data;
	AudioSamplingConfig sampling_config;

	AudioData32() {};

	AudioData32(Vec<i32>& _data, AudioSamplingConfig _audio_sampling_config)
	{
		data = _data;
		sampling_config = _audio_sampling_config;
	};

	void set_sampling_config(AudioSamplingConfig _sampling_config)
	{
		Vec<i32> data_old = data;
		sampling_config = _sampling_config;
		data.set_count(sampling_config.sample_count);
		data.set_value(0.0);
		
		// copy old data into new size
		uint min_count = data.count() < data_old.count() ? data.count() : data_old.count();
		for(uint i = 0; i < min_count; i++)
		{
			data[i] = data_old[i];
		}
	}


	// void set_audio_length(AudioLength _audio_length)
	// {
	// 	audio_length = _audio_length;
	// 	data.set(0.0, audio_length.sample_count);
	// }


	/* Returns the absolute value of the sample with the largest absolute sample magnitude */
	i32 get_abs_max_sample()
	{
		i32 max_abs = 0.0;

		for(uint i = 0; i < data.count(); i++)
		{
			if(abs(data[i]) > max_abs )
				max_abs = abs(data[i]);
		}

		return max_abs;
	}

	/** 
		Reduce very loud sections to avoid clipping and keep relevant audio volume constant.
		Any sample above max the 24-bit signed max is proprtionaly reduced to be contained within 25 bits
	 */
	void smoothen_loud_audio_peaks()
	{
		// i32 abs_average = 0;
		// i64 abs_sum = 0;
		// for(uint i = 0; i < data.count(); i++)
		// {
		// 	abs_sum += abs(data[i]);
		// }
		// abs_average = (i32) (abs_sum / (i64) data.count());

		i32 max_i24 = 8388607;

		f64 max_i24_db = (f64) max_i24;

		for(uint i = 0; i < data.count(); i++)
		{
			// trying to map all values exceeding the max to a 1-bit span above the maximum allowed 'logarithmically' 
			if(data[i] > max_i24)
			{
				// i64 data_i_i64 = data[i] * 1000;
				f64 data_i_f64 = (f64) data[i];
				
				f64 factor = data_i_f64 / max_i24_db;

				f64 bits_above_max = 1 - 1/factor; // OK because we already sorted all samples below max i24

				f64 term_exceeding_max = max_i24_db * bits_above_max;

				f64 final_value = max_i24_db + term_exceeding_max;

				data[i] = (i32) final_value * 0.5;
			}
		}
	}

	/** Normalize all samples using maximum sample magnitude.
		If the data is already contained within the set [-1, 1], no changes to the data are made.
	*/
	void normalize()
	{
		i32 max_abs = get_abs_max_sample();
		if(max_abs == 0)
			return;


		for(uint i = 0; i < data.count(); i++)
		{
			data[i] = data[i] / 3;
		}

		// double new_max_abs = get_abs_max_sample();

	}

	AudioData16 to_audio_data_i16()
	{
		AudioData16 audio_data;
		audio_data.data.set_count(sample_count());
		audio_data.data.set_value(0);

		for(uint i = 0; i < data.count(); i++)
		{
			// if(data[i] > 1.0 || data[i] < -1.0)
			// {
			// 	Print::ln("WARNING: exporting a non-normalized audio data buffer!");
			// 	Print::ln(Str::FL(data[i], 6, Str::FloatRep::Fixed));
			// 	Print::ln(Str::FL(fabs(data[i]), 6, Str::FloatRep::Fixed));
			// }

			audio_data.data[i] = (int16_t) (data[i] / 255); // CLIP AT 24 BIT -> convert to 16 for specific output
		}
		
		return audio_data;
	}

	Vec<i16> to_vec_i16()
	{
		Vec<i16> vec_i16;
		vec_i16.set_count(sample_count());
		vec_i16.set_value(0);

		for(uint i = 0; i < data.count(); i++)
			vec_i16[i] = (i16) (data[i] / 255);
		
		return vec_i16;
	}


	int sample_rate() { return sampling_config.sample_rate_s; }
	int channel_count() { return 1; }
	int sample_size_bit() { return 8 * sample_size_byte(); }
	int sample_size_byte() { return sizeof(double); }

	uint sample_count() { return sampling_config.sample_count; }
	int data_size_byte() { return sampling_config.sample_count * sample_size_byte(); }

	double duration_s_double() { return ((double)sample_count()) / ((double)sample_rate()); }
	int duration_ms_int() { return (int) (duration_s_double() * 1000); } // rounds according to double to int cast rounding rules
};