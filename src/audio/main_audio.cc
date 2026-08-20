#include <alsa/asoundlib.h>
#include <stdio.h>
#include <cstdint> // fixed width ints

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h> // memcpy

#include "lib/print.hh"
#include "lib/arr.hh"

#include "audio.hh"

#define PCM_DEVICE "default"



/* 	Central audio data container. 
	Implicit info:
		- Sample rate: 42100
		- Channels: 1 (mono)
		- DataType: 16bit signed integer
*/
class AudioData
{
public:

	Arr<int16_t> data;

	AudioData() {};


	int sample_rate() { return 42100; }
	int channel_count() { return 1; }
	int sample_size_bit() { return 16; }
	int sample_size_byte() { return 2; }

	int sample_count() { return data.count(); }
	int data_size_byte() { return data.count() * 2; }

	double duration_double() { return ((double)sample_count()) / ((double)sample_rate()); }
	int duration_int() { return (int) duration_double(); }
};



class Alsa
{
public:

	unsigned int ret, tmp, dir;
	unsigned int rate, channels;
	unsigned int period_time; // variable provided by ALSA that specifies the time (in microseconds) between play/capture hardware interrupts
	snd_pcm_t *pcm_handle;
	snd_pcm_hw_params_t *params;
	snd_pcm_uframes_t frame_count; // Some fixed size of number of frames (frame: a different word for sample that also includes all available channels) used by the current configuration space
	char *buff;
	int buff_size, loops;
	int bytes_per_sample = 2; // 16 bit integers is default
	Str file_path = "";
	

	Alsa(uint rate, uint channels, Str file_path)
	{
		this->rate = rate;
		this->channels = channels;
		this->file_path = file_path;

		/* Open the PCM device in playback mode */
		if (ret = snd_pcm_open(&pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0) < 0) 
			printf("ERROR: Can't open \"%s\" PCM device. %s\n", PCM_DEVICE, snd_strerror(ret));

		/* Allocate parameters object and fill it with default values */
		snd_pcm_hw_params_alloca(&params);
		snd_pcm_hw_params_any(pcm_handle, params);

		/* Set parameters */
		if (ret = snd_pcm_hw_params_set_access(pcm_handle, params,
						SND_PCM_ACCESS_RW_INTERLEAVED) < 0) 
			printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(ret));

		if (ret = snd_pcm_hw_params_set_format(pcm_handle, params,
							SND_PCM_FORMAT_S16_LE) < 0) 
			printf("ERROR: Can't set format. %s\n", snd_strerror(ret));

		if (ret = snd_pcm_hw_params_set_channels(pcm_handle, params, channels) < 0) 
			printf("ERROR: Can't set channels number. %s\n", snd_strerror(ret));

		if (ret = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0) < 0) 
			printf("ERROR: Can't set rate. %s\n", snd_strerror(ret));

		/* Write parameters */
		if (ret = snd_pcm_hw_params(pcm_handle, params) < 0)
			printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(ret));
		
		/* Allocate buffer to hold single period */
		snd_pcm_hw_params_get_period_size(params, &frame_count, 0);
		snd_pcm_hw_params_get_period_time(params, &period_time, NULL);

		buff_size = frame_count * channels * bytes_per_sample /* 2 -> sample size */;
		buff = (char *) malloc(buff_size);
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

	}

	int play()
	{
		snd_pcm_hw_params_get_period_time(params, &period_time, NULL);

		// pcm = read(0, buff, buff_size);
		// pcm = snd_pcm_writei(pcm_handle, buff, frame_count);


		if(false)
		{
			short buf[128];
			for (int i = 0; i < 100; ++i) {
				if ((ret = snd_pcm_writei (pcm_handle, buf, 128)) != 128) {
					fprintf (stderr, "write to audio interface failed (%s)\n",
						snd_strerror (ret));
					exit (1);
				}
			}
		}

		if(true)
		{
			int fd = open(file_path.to_c_str(), O_RDONLY);
			if(fd < 0)
			{
				println("ERROR: filed to open audio file in Alsa.play(). Trying stdin instead.");
				fd = 0;
			}

			// printf("loopy about to start  ");
			for (loops = (1000000 / period_time); loops > 0; loops--) {
			// // for (loops = (seconds * 100000) / tmp; loops > 0; loops--) {
				// printf("loopy.  ");
			// 	// printf("buff size: %i \n", buff_size);

				if ((ret = read(fd, buff, buff_size)) == 0) {
					printf("Early end of file.\n");
					return 0;
				}

				if ((ret = snd_pcm_writei(pcm_handle, buff, frame_count)) == -EPIPE) {
					printf("XRUN.\n");
					snd_pcm_prepare(pcm_handle);
				} else if (ret < 0) {
					printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(ret));
				}
				
			}

			ret = close(fd);
			if(ret < 0)
			{
				println("ERROR: filed to close audio file in Alsa.play()");
				return 1;
			}
		}

		return 1;
	}


	void play(AudioData audio_data)
	{
		int loop_counter = 0;

		for (loops = (1000000 / period_time); loops > 0; loops--)
		{
			int element_offset = (loop_counter++) * ( frame_count ); // number of elements in one 'period_time'
			memcpy(	buff,
					audio_data.data.data_mut() + element_offset,
					buff_size);

			if ((ret = snd_pcm_writei(pcm_handle, buff, frame_count)) == -EPIPE) {
				printf("XRUN.\n");
				snd_pcm_prepare(pcm_handle);
			} else if (ret < 0) {
				printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(ret));
			}	
		}
	}

	void end()
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
	uint wave_freq = 1000; // Hz = osc. / s

	/* Derived quantities based on provided values above */
	uint sample_count; // total number of samples ( sample_rate * duration )
	double dt; // time between samplings
	double amp; // amplitude of the wave
	double gain = 0.5; // amp multiplier
	double freq_mult; // frequency multiplier for the sine argument

	Arr<double> t_arr;
    Arr<double> s_arr;
    Arr<int16_t> wave_arr;


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
		wave_arr.reserve(sample_count);
		wave_arr.set(0);
	}

	void generate_wave()
	{
		for(uint i = 0; i < sample_count; i++)
		{
			double i_d = (double)i;
			t_arr[i] = dt * i_d;

			s_arr[i] = gain * amp * sin( freq_mult * t_arr[i] );

			wave_arr[i] = (int) s_arr[i];
		}
	}

	void print_wave()
	{
		for(uint i = 0; i < sample_count; i++)
		{
			print(Str::FL(t_arr[i], 5, Str::FloatRep::Fixed));
			print("  ");
			print(Str::SI(wave_arr[i]));
			println();
		}
	}

};



struct WavHeaderRIFF
{
	int8_t FileTypeBlocID[4] = { 0x52, 0x49, 0x46, 0x46};		// (4 bytes) : Identifier « RIFF »  (0x52, 0x49, 0x46, 0x46)
	int32_t FileSize;			// (4 bytes) : Overall file size minus 8 bytes
	int8_t FileFormatID[4] = {0x57, 0x41, 0x56, 0x45};		// (4 bytes) : Format = « WAVE »  (0x57, 0x41, 0x56, 0x45)
};

struct WavHeaderDataFormat
{
	int8_t FormatBlocID[4] = {0x66, 0x6D, 0x74, 0x20};    // (4 bytes) : Identifier « fmt␣ »  (0x66, 0x6D, 0x74, 0x20)
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
	int8_t DataBlocID[4] = {0x64, 0x61, 0x74, 0x61};    	// (4 bytes) : Identifier « data »  (0x64, 0x61, 0x74, 0x61)
    int32_t DataSize;      	// (4 bytes) : Chunk size minus 8 bytes, which is 16 bytes here  (0x10)
	Arr<int16_t> SampledData; 		//  Actual data
};


class WAV
{
public:

	WavHeaderRIFF header_riff;
	WavHeaderDataFormat header_format;
	WavDataChunk data_chunk;

	bool is_little_endian = false;

	WAV() {};


	void populate_from_wave(SineWave& wave)
	{
		is_little_endian = false;

		wave.generate_wave();

		// Size: 12 bytes
		// header_riff.FileTypeBlocID = ...; // set in struct declaration
		header_riff.FileSize = 44 + wave.sample_count * 2 - 8;
		// header_riff.FileFormatID = ...; // set in struct declaration

		// Size: 24 bytes
		// header_format.FormatBlocID = ...; // set in struct declaration
		header_format.BlocSize = 0x10; // size of the format header
		header_format.AudioFormat = 1; // PCM : integer
		header_format.NbrChannels = 1; 
		header_format.Frequency = wave.sample_rate; 
		header_format.BitsPerSample = 16;
		header_format.BytePerBloc = header_format.BitsPerSample * header_format.NbrChannels / 8;
		header_format.BytePerSec = wave.sample_rate * header_format.BytePerBloc;
		
		// Size: 8 + data_size
		// data_chunk.DataBlocID = ...; // set in struct declaration
		data_chunk.DataSize = wave.sample_count * 2;
		data_chunk.SampledData = wave.wave_arr; // TODO: make the WAV object own the data. Currently we store a pointer to data on the heap that could be deallocated at any point before writing to file.

	}

	int32_t change_endinaness_int32(int32_t integer)
	{
		int32_t tmp_int = integer;

		int32_t int_0 = (tmp_int & 0xFF000000) >> 24;
		int32_t int_1 = (tmp_int & 0x00FF0000) >> 8;
		int32_t int_2 = (tmp_int & 0x0000FF00) << 8;
		int32_t int_3 = (tmp_int  & 0x000000FF) << 24;

		return int_0 | int_1 | int_2 | int_3;
	}

	int16_t change_endinaness_int16(int16_t integer)
	{
		int16_t byte_0 = (integer & 0xFF00) >> 8;
		int16_t byte_1 = (integer & 0x00FF) << 8;

		return byte_0 | byte_1;
	}

	void to_little_endian()
	{
		if(!is_little_endian)
		{
			is_little_endian = true;

			// RIFF CHUNK
			header_riff.FileSize = change_endinaness_int32(header_riff.FileSize);

			// FORMAT CHUNK
			header_format.BlocSize = change_endinaness_int32(header_format.BlocSize);
			header_format.AudioFormat = change_endinaness_int16(header_format.AudioFormat);
			header_format.NbrChannels = change_endinaness_int16(header_format.NbrChannels);
			header_format.BytePerSec = change_endinaness_int32(header_format.BytePerSec);
			header_format.BytePerBloc = change_endinaness_int16(header_format.BytePerBloc);
			header_format.BitsPerSample = change_endinaness_int16(header_format.BitsPerSample);

			// DATA CHUNK
			data_chunk.DataSize = change_endinaness_int32(data_chunk.DataSize);
			for(uint i = 0; i < data_chunk.SampledData.count(); i++)
			{
				data_chunk.SampledData[i] = change_endinaness_int16(data_chunk.SampledData[i]);
			}

			// print("Conversion to little endian complete.");
		}
	}

	void to_big_endian()
	{
		if(is_little_endian)
		{
			// CONVERT TO BIG ENDIAN
		}
	}

	void write_to_file(Str file_path)
	{
		int rtrn = -1;
		void* ptr = nullptr;

		if(file_path == "")
		{
			println("Unable to write to WAV file: no file name specified.");
			return;
		}

		int fd = open(file_path.to_c_str(), O_RDWR | O_CREAT );
		if(fd < 0)
		{
			println("ERROR: returned file descript is less than 0.");
			return;
		}

		ptr = (void*) &header_riff;
		rtrn = write(fd, ptr, sizeof(header_riff));
		if(rtrn < 0)
		{
			println("ERROR: Failed to write riff header to file.");
			return;
		}

		ptr = (void*) &header_format;
		rtrn = write(fd, ptr, sizeof(header_format));
		if(rtrn < 0)
		{
			println("ERROR: Failed to write format header to file.");
			return;
		}

		ptr = (void*) &data_chunk;
		rtrn = write(fd, ptr, 8);
		if(rtrn < 0)
		{
			println("ERROR: Failed to write data chunk ID and Size to file to file.");
			return;
		}

		ptr = (void*) data_chunk.SampledData.data_mut();
		rtrn = write(fd, ptr, data_chunk.SampledData.count_byte());
		if(rtrn < 0)
		{
			println("ERROR: Failed to write wave data to file.");
			return;
		}

		rtrn = close(fd);
		if(rtrn < 0)
		{
			println("ERROR: Failed to close fd.");
			return;
		}
	}

	void read_file(Str file_path)
	{
		int fd, ret;

		fd = open(file_path.to_c_str(), O_RDONLY);
		if(fd < 0)
		{
			println("ERROR: Failed to open WAV file.");
			return;
		}

		ret = read(fd, this, 60);
		if(ret < 0)
		{
			println("ERROR: Failed to read first 44 bytes of WAV file.");
			return;
		}

		// ret = read(fd, data_chunk.SampledData);

		print("Reading Wav file. Size = ");
		println(Str::SI(data_chunk.DataSize));




		ret = close(fd);
		if(ret < 0)
		{
			println("ERROR: Failed to close WAV file.");
			return;
		}
	}

};


void bin_dump(Str file_path, void* ptr, uint byte_count)
{
	int fd, ret;

	fd = open(file_path.to_c_str(), O_WRONLY | O_CREAT);
	if(fd < 0)
	{
		println("ERROR: Failed to open bin_dump-file for writing.");
		return;
	}
	ret = fchmod(fd, 00777);


	ret = write(fd, ptr, byte_count);
	if(ret < 0)
	{
		println("ERROR: Failed to write to bin_dump-file.");
		return;
	}

	ret = close(fd);
	if(ret < 0)
	{
		println("ERROR: Failed to close bin_dump-file.");
		return;
	}
}



int main(int argc, char** argv)
{
    println("Physimos::audio starting!");

	SineWave sine_wave;
	sine_wave.generate_wave();
	// sine_wave.print_wave();

	WAV wav; 
	// wav.populate_from_wave(sine_wave);
	// wav.to_little_endian();
	// wav.write_to_file("tmp/test2.wav");
	wav.read_file("resources/audio/sine.wav");
	print("Reading Wav file. Size = ");
	print(Str::CH(wav.data_chunk.DataBlocID[1]));
	println(Str::SI(wav.data_chunk.DataSize));
	bin_dump("tmp/dump.bin", &wav, 44);



    // Arr<int> sine_buff = create_sine_buffer();
	// int* sine_buff_ptr = sine_buff.data_mut();

    // print buff
    // for(uint i = 0; i < sine_buff.count(); i++)
    // {
    //     print(Str::SI(sine_buff[i]));
    //     print(" ");
    // }
	// print("\n");

	
	unsigned int rate, channels;

	if (argc > 2)
    {
		// printf("Usage: %s <sample_rate> <channels> <seconds>\n", argv[0]);
		// return -1;
		rate 	 = atoi(argv[1]);
		channels = atoi(argv[2]);
	}
	else
	{
		rate 	 = 48000;
		channels = 1;
	}


	Alsa alsa { 
		rate, 
		channels,
		"resources/audio/sine.wav"
	};

	AudioData audio_data;
	audio_data.data = sine_wave.wave_arr;

	alsa.print_pcm_info();
	// alsa.play();
	alsa.play(audio_data);
	alsa.end();


    printf("End Alsa test\n");

	return 0;
}