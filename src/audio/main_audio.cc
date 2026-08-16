#include <alsa/asoundlib.h>
#include <stdio.h>
#include <cstdint> // fixed width ints

#include "lib/print.hh"
#include "lib/arr.hh"

#include "audio.hh"

#define PCM_DEVICE "default"


class Alsa
{
public:

	unsigned int pcm, tmp, dir;
	unsigned int rate, channels, seconds;
	snd_pcm_t *pcm_handle;
	snd_pcm_hw_params_t *params;
	snd_pcm_uframes_t frames;
	char *buff;
	int buff_size, loops;

	Alsa(uint rate, uint channels, uint seconds)
	{
		this->rate = rate;
		this->channels = channels;
		this->seconds = seconds;

		/* Open the PCM device in playback mode */
		if (pcm = snd_pcm_open(&pcm_handle, PCM_DEVICE,
						SND_PCM_STREAM_PLAYBACK, 0) < 0) 
			printf("ERROR: Can't open \"%s\" PCM device. %s\n",
						PCM_DEVICE, snd_strerror(pcm));

		/* Allocate parameters object and fill it with default values */
		snd_pcm_hw_params_alloca(&params);
		snd_pcm_hw_params_any(pcm_handle, params);

		/* Set parameters */
		if (pcm = snd_pcm_hw_params_set_access(pcm_handle, params,
						SND_PCM_ACCESS_RW_INTERLEAVED) < 0) 
			printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));

		if (pcm = snd_pcm_hw_params_set_format(pcm_handle, params,
							SND_PCM_FORMAT_S16_LE) < 0) 
			printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));

		if (pcm = snd_pcm_hw_params_set_channels(pcm_handle, params, channels) < 0) 
			printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));

		if (pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0) < 0) 
			printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));

		/* Write parameters */
		if (pcm = snd_pcm_hw_params(pcm_handle, params) < 0)
			printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));
	
	}

	void print_pcm_info()
	{
		/* Resume information */
		printf("PCM name: '%s'\n", snd_pcm_name(pcm_handle));

		printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(pcm_handle)));

		snd_pcm_hw_params_get_channels(params, &tmp);
		printf("channels: %i ", tmp);

		if (tmp == 1)
			printf("(mono)\n");
		else if (tmp == 2)
			printf("(stereo)\n");

		snd_pcm_hw_params_get_rate(params, &tmp, 0);
		printf("rate: %d bps\n", tmp);

		printf("seconds: %d\n", seconds);
	}

	void allocate()
	{
		/* Allocate buffer to hold single period */
		snd_pcm_hw_params_get_period_size(params, &frames, 0);

		buff_size = frames * channels * 2 /* 2 -> sample size */;
		buff = (char *) malloc(buff_size);

		snd_pcm_hw_params_get_period_time(params, &tmp, NULL);
	}

	int play()
	{
		// printf("loopy about to start  ");
		for (loops = (seconds * 1000000) / tmp; loops > 0; loops--) {
		// for (loops = (seconds * 100000) / tmp; loops > 0; loops--) {
			// printf("loopy.  ");
			// printf("buff size: %i \n", buff_size);

			if (pcm = read(0, buff, buff_size) == 0) {
				printf("Early end of file.\n");
				return 0;
			}

			if (pcm = snd_pcm_writei(pcm_handle, buff, frames) == -EPIPE) {
				printf("XRUN.\n");
				snd_pcm_prepare(pcm_handle);
			} else if (pcm < 0) {
				printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));
			}
			
		}

		return 1;
	}

	void close()
	{
		snd_pcm_drain(pcm_handle);
		snd_pcm_close(pcm_handle);
		free(buff);
	}
};


class SineWave
{
public:

	/* Provided quantities for wave generation */
	uint duration = 1; // Total duration of the generated data in seconds
	uint sample_rate = 48000; // samples per second
	uint sample_depth = 16; // 2^(sample_depth) number of available discrete values during sampling
	uint wave_freq = 200; // Hz = osc. / s

	/* Derived quantities based on provided values above */
	uint sample_count; // total number of samples ( sample_rate * duration )
	double dt; // time between samplings
	double amp; // amplitude of the wave
	double freq_mult; // frequency multiplier for the sine argument

	// Arr<double> t_arr 	{(uint)total_sample_count, (double)0.0};
    // Arr<double> s_arr 	{(uint)total_sample_count, (double)0.0};
    // Arr<int> out 		{(uint)total_sample_count, (int)0};
	Arr<double> t_arr;
    Arr<double> s_arr;
    Arr<int> out;

	// Vec<double> t_vec;

	SineWave()
	{
		calculate_derived_quantities();
	}
	SineWave(uint duration, uint sample_rate, uint sample_depth, uint wave_freq)
	{
		this->duration = duration;
		this->sample_rate = sample_rate;
		this->sample_depth = sample_depth;
		this->wave_freq = wave_freq;

		calculate_derived_quantities();
	}

	void calculate_derived_quantities()
	{
		sample_count = duration * sample_rate;
		dt = 1.0 / ((double)sample_rate);

		// Sine constants
		amp = pow(2.0, sample_depth-1) - 1.0;
		freq_mult = PI2 * wave_freq;

		t_arr.reserve(sample_count);
		t_arr.set(0.0);
		s_arr.reserve(sample_count);
		s_arr.set(0.0);
		out.reserve(sample_count);
		out.set(0);
	}

	void generate_wave()
	{
		for(uint i = 0; i < sample_count; i++)
		{
			double i_d = (double)i;
			t_arr[i] = dt * i_d;

			s_arr[i] = amp * sin( freq_mult * t_arr[i] );

			out[i] = (int) s_arr[i];
		}
	}

	void print_wave()
	{
		for(uint i = 0; i < sample_count; i++)
		{
			print(Str::FL(t_arr[i], 5, Str::FloatRep::Fixed));
			print("  ");
			print(Str::SI(out[i]));
			println();
		}
	}

};



struct WavHeaderRIFF
{
	int32_t FileTypeBlocID;		// (4 bytes) : Identifier « RIFF »  (0x52, 0x49, 0x46, 0x46)
	int32_t FileSize;			// (4 bytes) : Overall file size minus 8 bytes
	int32_t FileFormatID;		// (4 bytes) : Format = « WAVE »  (0x57, 0x41, 0x56, 0x45)
};

struct WavHeaderDataFormat
{
	int32_t FormatBlocID;    // (4 bytes) : Identifier « fmt␣ »  (0x66, 0x6D, 0x74, 0x20)
    int32_t BlocSize;        // (4 bytes) : Chunk size minus 8 bytes, which is 16 bytes here  (0x10)
    int16_t AudioFormat;     // (2 bytes) : Audio format (1: PCM integer, 3: IEEE 754 float)
    int16_t NbrChannels;     // (2 bytes) : Number of channels
    int32_t Frequency;       // (4 bytes) : Sample rate (in hertz)
    int32_t BytePerSec;      // (4 bytes) : Number of bytes to read per second (Frequency * BytePerBloc).
    int16_t BytePerBloc;     // (2 bytes) : Number of bytes per block (NbrChannels * BitsPerSample / 8).
    int16_t BitsPerSample;   // (2 bytes) : Number of bits per sample
};

struct WavDataChunk
{
	int32_t DataBlocID;    	// (4 bytes) : Identifier « fmt␣ »  (0x66, 0x6D, 0x74, 0x20)
    int32_t DataSize;      	// (4 bytes) : Chunk size minus 8 bytes, which is 16 bytes here  (0x10)
	void* SampledData; 		//  Actual data
};

class WavSerializer
{
public:
	bool is_little_endian = false;

	Str file_path_str = "";

	WavHeaderRIFF header_riff;
	WavHeaderDataFormat header_format;
	WavDataChunk data_chunk;

	SineWave wave;

	WavSerializer(SineWave wave) : wave {wave}
	{
	};


	// 1) generates sine wave, 2) populates the headers, and 3) make the chunk data point to the sinewave data
	void populate_from_wave()
	{
		is_little_endian = false;

		// 1)
		wave.generate_wave();

		// 2)
		header_riff.FileTypeBlocID = 0x52494646;
		header_riff.FileSize = 44 + wave.sample_count * 2 - 8;
		header_riff.FileFormatID = 0x57415645;

		header_format.FormatBlocID = 0x666D7420;
		header_format.BlocSize = 0x10; // size of the format header
		header_format.AudioFormat = 1; // PCM : integer
		header_format.NbrChannels = 1;
		header_format.Frequency = 48000; 
		header_format.BytePerSec = 96000;
		header_format.BytePerBloc = 2;
		header_format.BitsPerSample = 16;

		data_chunk.DataBlocID = 0x64617461;
		data_chunk.DataSize = wave.sample_count * 2;
		data_chunk.SampledData = wave.out.data_mut();
	}

	void to_little_endian()
	{
		if(!is_little_endian)
		{
			// CONVERT TO LITTLE ENDIAN
		}
	}

	void to_big_endian()
	{
		if(is_little_endian)
		{
			// CONVERT TO BIG ENDIAN
		}
	}

	void write_to_file()
	{
		if(file_path_str == "")
		{
			println("Unable to write to WAV file: no file name specified.");
			return;
		}

		

	}

};

int main(int argc, char** argv)
{
    println("Physimos::audio starting!");

	SineWave sine_wave;
	sine_wave.generate_wave();
	// sine_wave.print_wave();


    // Arr<int> sine_buff = create_sine_buffer();
	// int* sine_buff_ptr = sine_buff.data_mut();

    // print buff
    // for(uint i = 0; i < sine_buff.count(); i++)
    // {
    //     print(Str::SI(sine_buff[i]));
    //     print(" ");
    // }
	// print("\n");

	
	unsigned int rate, channels, seconds;

	if (argc > 3)
    {
		// printf("Usage: %s <sample_rate> <channels> <seconds>\n", argv[0]);
		// return -1;
		rate 	 = atoi(argv[1]);
		channels = atoi(argv[2]);
		seconds  = atoi(argv[3]);
	}
	else
	{
		rate 	 = 48000;
		channels = 1;
		seconds  = 1;
	}


	Alsa alsa { 
		rate, 
		channels, 
		seconds
	};

	alsa.print_pcm_info();
	alsa.allocate();
	alsa.play();
	alsa.close();


    printf("End Alsa test\n");

	return 0;
}