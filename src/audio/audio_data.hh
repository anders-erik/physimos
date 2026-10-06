
#pragma once


#include "lib/arr.hh"

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
};

/* Audio data with double values. On conversion to output format the values will be cliped at [-1, 1] */
class AudioDataRaw
{
public:

	Arr<double> data;
	AudioLength audio_length;

	AudioDataRaw() {};

	AudioDataRaw(uint _data_count)
	{
		audio_length.sample_count = _data_count;
		data.set(0.0, audio_length.sample_count);
	};
	AudioDataRaw(Arr<double>& _data, AudioLength _audio_length)
	{
		data = _data;
		audio_length = _audio_length;
	};

	void set_audio_length(AudioLength _audio_length)
	{
		audio_length = _audio_length;
		data.set(0.0, audio_length.sample_count);
	}

	AudioData to_audio_data()
	{
		AudioData audio_data;
		audio_data.data = Arr<int16_t>{0, sample_count()};
		
		for(uint i = 0; i < data.count(); i++)
			audio_data.data[i] = (int16_t) data[i];
		
		return audio_data;
	}


	int sample_rate() { return audio_length.sample_rate; }
	int channel_count() { return 1; }
	int sample_size_bit() { return 8 * sample_size_byte(); }
	int sample_size_byte() { return sizeof(double); }

	uint sample_count() { return audio_length.sample_count; }
	int data_size_byte() { return audio_length.sample_count * sample_size_byte(); }

	double duration_s_double() { return ((double)sample_count()) / ((double)sample_rate()); }
	int duration_ms_int() { return (int) (duration_s_double() * 1000); } // rounds according to double to int cast rounding rules
};