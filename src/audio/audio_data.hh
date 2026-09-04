
#pragma once



/* 	Central audio data container. 
	Implicit info:
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

	double duration_double() { return ((double)sample_count()) / ((double)sample_rate()); }
	int duration_int() { return (int) duration_double(); } // rounds according to double to int cast rounding rules
};