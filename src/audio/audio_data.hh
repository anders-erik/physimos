
#pragma once

#include <cmath>

#include "lib/arr.hh"
#include "lib/defs.hh"

/* 	
	Core object for raw audio data.
	Implicit audio settings:
		- Sample rate: 44100
		- Channels: 1 (mono)
		- DataType: 16bit signed integer
*/
class AudioData
{
public:

	Arr<int16_t> data;

	AudioData() {};
	AudioData(uint _data_count)
	{
		data.set(0, _data_count);
	};
	AudioData(Arr<int16_t>& data)
	{
		this->data = data;
	};

	// force vector to hold a specific sample count, all set to zero
	void set_sample_count(uint _count)
	{
		data.set(0, _count);
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


struct AudioLength
{
	int sample_rate = 44100;
	int sample_count = 0;
	// int duration_us = 0;
};

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

	// AudioSamplingConfig& operator=(AudioSamplingConfig& rhs)
	// {
	// 	this->sample_rate_s = rhs.sample_rate_s;
	// 	this->sample_count 	= rhs.sample_count;
	// 	this->duration_us 	= rhs.duration_us;
	// }

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
	
};


/* Audio data with double values. On conversion to output format the values will be cliped at [-1, 1] */
class AudioDataRaw
{
public:

	Arr<double> data;
	AudioSamplingConfig sampling_config;

	AudioDataRaw() {};

	// AudioDataRaw(uint _data_count)
	// {
	// 	audio_length.sample_count = _data_count;
	// 	data.set(0.0, audio_length.sample_count);
	// };
	AudioDataRaw(Arr<double>& _data, AudioSamplingConfig _audio_sampling_config)
	{
		data = _data;
		sampling_config = _audio_sampling_config;
	};

	void set_sampling_config(AudioSamplingConfig _sampling_config)
	{
		sampling_config = _sampling_config;
		data.set(0.0, sampling_config.sample_count);
	}


	// void set_audio_length(AudioLength _audio_length)
	// {
	// 	audio_length = _audio_length;
	// 	data.set(0.0, audio_length.sample_count);
	// }


	/* Returns the absolute value of the sample with the largest absolute sample magnitude */
	f64 get_abs_max_sample()
	{
		f64 max_abs = 0.0;

		for(uint i = 0; i < data.count(); i++)
		{
			if(fabs(data[i]) > max_abs )
				max_abs = fabs(data[i]);
		}

		return max_abs;
	}

	/** Normalize all samples using maximum sample magnitude.
		If the data is already contained within the set [-1, 1], no changes to the data are made.
	*/
	void normalize()
	{
		double max_abs = get_abs_max_sample();
		if(max_abs == 0.0)
			return;

		for(uint i = 0; i < data.count(); i++)
		{
			data[i] = data[i] / max_abs;
		}

		// double new_max_abs = get_abs_max_sample();

	}

	AudioData to_audio_data()
	{
		AudioData audio_data;
		audio_data.data = Arr<int16_t>{sample_count(), 0};

		for(uint i = 0; i < data.count(); i++)
		{
			if(data[i] > 1.0 || data[i] < -1.0)
			{
				Print::ln("WARNING: exporting a non-normalized audio data buffer!");
				Print::ln(Str::FL(data[i], 6, Str::FloatRep::Fixed));
				Print::ln(Str::FL(fabs(data[i]), 6, Str::FloatRep::Fixed));
			}

			audio_data.data[i] = (int16_t) (data[i]* 32766);
		}
		
		return audio_data;
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