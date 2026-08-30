#include <alsa/asoundlib.h>
#include <stdio.h>
#include <cstdint> // fixed width ints

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h> // memcpy

#include "lib/print.hh"
#include "lib/arr.hh"

#include "math/vec.hh"

#include "audio.hh"
#include "input.hh"
#include "DFT.hh"

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



class Alsa
{
public:

	unsigned int rate = 44100;
	unsigned int channels = 1;
	int bytes_per_sample = 2; // 16 bit integers is default
	
	Str file_path = "";

	unsigned int ret, tmp, dir;
	long long_ret;

	unsigned int period_time; // variable provided by ALSA that specifies the time (in microseconds) between play/capture hardware interrupts
	snd_pcm_t *pcm_handle;
	snd_pcm_hw_params_t *params;
	snd_pcm_uframes_t frame_count; // Some fixed size of number of frames (frame: a different word for sample that also includes all available channels) used by the current configuration space
	char *buff;
	int buff_size, loops;
	
	Alsa()
	{
		setup();
	}

	Alsa(uint rate, uint channels, Str file_path)
	{
		this->rate = rate;
		this->channels = channels;
		this->file_path = file_path;

		setup();
	}

	Alsa(const Alsa&) = delete;

	~Alsa()
	{
		snd_pcm_drain(pcm_handle);
		snd_pcm_close(pcm_handle);
		free(buff);
	}

	void setup()
	{
		/* Open the PCM device in playback mode */
		// if (ret = snd_pcm_open(&pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK) < 0) 
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

		buff_size = frame_count * channels * bytes_per_sample * 1.5/* 2 -> sample size */;
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

		for (loops = (  (uint)(audio_data.duration_double() * 1000000.0) / period_time); loops > 0; loops--)
		{
			int element_offset = (loop_counter++) * ( frame_count ); // number of elements in one 'period_time'
			
			memcpy(	buff,
					audio_data.data.data_mut() + element_offset,
					buff_size);

			if ((long_ret = snd_pcm_writei(pcm_handle, buff, frame_count)) == -EPIPE) {
				printf("XRUN. snd_pcm_writei return value: %li \n", long_ret);
				snd_pcm_prepare(pcm_handle);
			} else if (long_ret < 0) {
				printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(ret));
			}
			// printf("     snd_pcm_writei return value: %li \n", long_ret);
		}
	}

	// void end()
	// {
	// 	snd_pcm_drain(pcm_handle);
	// 	snd_pcm_close(pcm_handle);
	// 	free(buff);
	// }
};


struct D01
{
	// D01 = [0, 1]
}; 

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


	void populate_from_wave(WaveGen& wave)
	{
		is_little_endian = false;

		wave.generate_wave();

		// Size: 12 bytes
		// header_riff.FileTypeBlocID = ...; // set in struct declaration
		header_riff.FileSize = 44 + wave.config.sample_count * 2 - 8;
		// header_riff.FileFormatID = ...; // set in struct declaration

		// Size: 24 bytes
		// header_format.FormatBlocID = ...; // set in struct declaration
		header_format.BlocSize = 0x10; // size of the format header
		header_format.AudioFormat = 1; // PCM : integer
		header_format.NbrChannels = 1; 
		header_format.Frequency = wave.config.sample_rate; 
		header_format.BitsPerSample = 16;
		header_format.BytePerBloc = header_format.BitsPerSample * header_format.NbrChannels / 8;
		header_format.BytePerSec = wave.config.sample_rate * header_format.BytePerBloc;
		
		// Size: 8 + data_size
		// data_chunk.DataBlocID = ...; // set in struct declaration
		data_chunk.DataSize = wave.config.sample_count * 2;
		data_chunk.SampledData = wave.out_arr; // TODO: make the WAV object own the data. Currently we store a pointer to data on the heap that could be deallocated at any point before writing to file.

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

	template <typename T>
	Arr<T>&& read_bin_file(Str file_path)
	{
		int fd, ret;

		fd = open(file_path.to_c_str(), O_RDONLY);
		if(fd < 0)
		{
			println("ERROR: Failed to open WAV file.");
			return Arr<T> {};
		}

		ret = close(fd);
		if(ret < 0)
		{
			println("ERROR: Failed to close WAV file.");
			return Arr<T> {};
		}

		return Arr<T> {};
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

		ret = read(fd, &header_riff, 12);
		if(ret < 0)
		{
			println("ERROR: Failed to read first 12 bytes of WAV file.");
			return;
		}
		
		ret = read(fd, &header_format, 24);
		if(ret < 0)
		{
			println("ERROR: Failed to read bytes 13-36 of WAV file.");
			return;
		}

		ret = read(fd, &data_chunk, 8);
		if(ret < 0)
		{
			println("ERROR: Failed to read bytes 37-44 of WAV file.");
			return;
		}


		{
			void* buf = malloc(data_chunk.DataSize);

			ret = read(fd, buf, data_chunk.DataSize);
			data_chunk.SampledData.set_from_pointer( (int16_t*)buf, data_chunk.DataSize / sizeof(int16_t));

			free(buf);
		}


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



enum class NoteType
{
	whole,
	half,
	quarter
};

enum class NoteName
{
	C4,
	D4,
	E4,
	F4,
	G4,
	A4,
	B4,
	C5,
};

struct Note 
{
	NoteName name;
	NoteType type;

	bool operator==(Note b)
	{
		if(this->name == b.name && this->type == b.type)
			return true;
		
		return false;
	}
};

// const Note A4 { "A4", 440.0, NoteType::quarter};
// const Note A4 { NoteName::A4, NoteType::quarter};

#include "lib/pair.hh"


class Instrument
{
public:

	static AudioData get_note_audio(Note note, double tempo_bpm, double _gain)
	{
		WaveGen wave_gen;

		// Duration
		double duration = 1.0 / (tempo_bpm / 60.0); // seconds / beat (for quarter note)

		if(note.type == NoteType::whole)
			duration *= 4.0; // four beats
		else if(note.type == NoteType::half)
			duration *= 2.0; // two beats
		else if(note.type == NoteType::quarter)
			duration *= 1.0; // one beats
		
		// Frequency
		wave_gen.wave_freqs.clear();

		if(note.name == NoteName::C4)
			wave_gen.wave_freqs.push_back({261.63, 1.0});
		else if(note.name == NoteName::D4)
			wave_gen.wave_freqs.push_back({293.66, 1.0});
		else if(note.name == NoteName::E4)
			wave_gen.wave_freqs.push_back({329.63, 1.0});
		else if(note.name == NoteName::F4)
			wave_gen.wave_freqs.push_back({349.23, 1.0});
		else if(note.name == NoteName::G4)
			wave_gen.wave_freqs.push_back({392.0, 1.0});
		else if(note.name == NoteName::A4)
			wave_gen.wave_freqs.push_back({440.0, 1.0});
		else if(note.name == NoteName::B4)
			wave_gen.wave_freqs.push_back({493.88, 1.0});
		else if(note.name == NoteName::C5)
			wave_gen.wave_freqs.push_back({523.25, 1.0});
		
		wave_gen.config.set_duration(duration);
		wave_gen.config.set_gain(_gain);

		wave_gen.generate_wave();

		return wave_gen.out_arr;
	}
};

class Song
{
public:

	double bpm = 120.0; // beat / minute
	uint beat_count = 16;

	Arr<Arr<Note>> notes;
	AudioData song_data;

	// Arr<Pair<Note, AudioData>> notes_data;
	// Arr<Note> notes;

	Song() : notes {beat_count, {}}
	{
	}


	void add_wave_to_audiodata_at_beat_count(AudioData& _audio_data, uint beat_index, double beat_note_gain/*gain for specific note during the current beat index*/)
	{

		uint samples_per_beat = (uint)(44100.0 * (60.0 / bpm));

		uint first_sample_offset = samples_per_beat * beat_index;
		// uint last_sample_offset = first_sample_offset + _audio_data.sample_count();

		for(uint i = 0; i < _audio_data.sample_count(); i++)
		{
			int16_t wave_data_with_gain_adjusted = (int16_t) (beat_note_gain * (double)_audio_data.data[i]);
			song_data.data[first_sample_offset + i] += wave_data_with_gain_adjusted;
		}
	}

	void generate()
	{	
		double song_duration_s = (double)beat_count * 60.0 / bpm;
		double sample_count = 44100 * song_duration_s;

		song_data.set_sample_count(sample_count);

		for(uint beat_i = 0; beat_i < notes.count(); beat_i++)
		{

			// double beat_note_gain = 1.0 / sqrt( (double) notes[beat_i].count() ) ; // reduce frequency amplitude when multiple keys are pressed at the same beat
			double beat_note_gain = 0.6 / sqrt( (double) notes[beat_i].count() ) ; // sqrt is good, only need default single key gain be < 1.0
			// double beat_note_gain = 1.0 / (double) notes[beat_i].count();
			// double beat_note_gain = 0.1;

			for(uint note_i = 0; note_i < notes[beat_i].count(); note_i++)
			{
				AudioData note_data = Instrument::get_note_audio(notes[beat_i][note_i], bpm, 0.5);
				add_wave_to_audiodata_at_beat_count(note_data, beat_i, beat_note_gain);
			}
		}
	}

	void play(Alsa& alsa)
	{
		alsa.play(song_data);

		// for(uint i = 0; i < notes.count(); i++)
		// {
			// Note note = notes[i];

			// // Find the already generated audio data for the note
			// for(uint j = 0; j < notes_data.count(); j++)
			// {
			// 	if(note == notes_data[j].XX)
			// 	{
			// 		alsa.play(notes_data[j].YY);
			// 		break;
			// 	}
			// }
		// }
	}
};

void twinkle_twinkle()
{
	Alsa alsa;

	Song twinkle;
	twinkle.beat_count = 16;
	twinkle.bpm = 120;
	
	twinkle.notes[0].push_back({ NoteName::C4, NoteType::quarter});
	twinkle.notes[1].push_back({ NoteName::C4, NoteType::quarter});
	twinkle.notes[2].push_back({ NoteName::G4, NoteType::quarter});
	twinkle.notes[3].push_back({ NoteName::G4, NoteType::quarter});
	twinkle.notes[4].push_back({ NoteName::A4, NoteType::quarter});
	twinkle.notes[5].push_back({ NoteName::A4, NoteType::quarter});
	twinkle.notes[6].push_back({ NoteName::G4, NoteType::half	 });

	twinkle.notes[8].push_back({ NoteName::F4, NoteType::quarter});
	twinkle.notes[9].push_back( { NoteName::F4, NoteType::quarter});
	twinkle.notes[10].push_back( { NoteName::E4, NoteType::quarter});
	twinkle.notes[11].push_back( { NoteName::E4, NoteType::quarter});
	twinkle.notes[12].push_back( { NoteName::D4, NoteType::quarter});
	twinkle.notes[13].push_back( { NoteName::D4, NoteType::quarter});
	twinkle.notes[14].push_back( { NoteName::C4, NoteType::half   });

	twinkle.generate();
	twinkle.play(alsa);

	if(false)
	{
		WaveGen gen_C5(0.5, 523.3);
		FrequencyProfile profile_c5;
		profile_c5.generate_3_overtones(523.3);
		gen_C5.set_frequencies(profile_c5.frequencies);
		gen_C5.generate_wave();

		WaveGen gen_G5(0.5, 784.0);
		gen_G5.generate_wave();
		
		WaveGen gen_A5(0.5, 880.0);
		gen_A5.generate_wave();

		

		alsa.play(gen_C5.get_audio_data());
		alsa.play(gen_C5.get_audio_data());
		alsa.play(gen_G5.get_audio_data());
		alsa.play(gen_G5.get_audio_data());
		alsa.play(gen_A5.get_audio_data());
		alsa.play(gen_A5.get_audio_data());
		alsa.play(gen_G5.get_audio_data());
	}

	// alsa.end();
}


void ambiance_song()
{
	Alsa alsa;

	Song ambiance;
	ambiance.beat_count = 4;
	ambiance.bpm = 60;

	ambiance.notes[0].push_back({ NoteName::C4, NoteType::half});
	ambiance.notes[0].push_back({ NoteName::D4, NoteType::half});
	ambiance.notes[0].push_back({ NoteName::E4, NoteType::half});
	ambiance.notes[0].push_back({ NoteName::F4, NoteType::half});
	ambiance.notes[0].push_back({ NoteName::G4, NoteType::half});

	ambiance.notes[1].push_back({ NoteName::C4, NoteType::half});

	ambiance.notes[2].push_back({ NoteName::C4, NoteType::half});
	ambiance.notes[2].push_back({ NoteName::D4, NoteType::half});
	// ambiance.notes[2].push_back({ NoteName::E4, NoteType::half});
	ambiance.notes[2].push_back({ NoteName::F4, NoteType::half});
	ambiance.notes[2].push_back({ NoteName::G4, NoteType::half});

	ambiance.notes[3].push_back({ NoteName::D4, NoteType::half});
	

	ambiance.generate();
	ambiance.play(alsa);

	// alsa.end();
}





#include <sys/time.h>
class Clock
{
public:

	struct timeval tv_start;

	Clock()
	{
		gettimeofday(&tv_start, NULL);
	};

	uint64_t get_unix_epoch_ms()
	{
		uint64_t time_ms = 0;

		struct timeval tv;
		gettimeofday(&tv, NULL);

		time_ms = (uint64_t) tv.tv_sec * 1000;

		// time_ms += ((double)tv.tv_usec) / 1000000.0;
		time_ms +=  (uint64_t) (tv.tv_usec / 1000);

		return time_ms;
	}

	void print_tv_start()
	{
		printf("time s : %li \n", tv_start.tv_sec);
		printf("time us: %li \n", tv_start.tv_usec);
	}
};


class SleepTimer
{
public:

	Clock clock;

	uint64_t start_time;
	uint64_t duration_ms;

	uint64_t end_time;


	SleepTimer() {}


	void sleep_ms(uint64_t ms)
	{
		usleep(ms * 1000);
	}


	void sleep(uint64_t _duration_ms)
	{
		start_time = clock.get_unix_epoch_ms();
		duration_ms = _duration_ms;
		end_time = start_time + duration_ms;

		uint64_t current_time_ms = clock.get_unix_epoch_ms();

		while(current_time_ms < end_time)
		{
			uint64_t delta_ms = end_time - current_time_ms;
			uint64_t sleep_time_ms = delta_ms * 0.5; // Do not sleep full duration to prevent 
			sleep_ms(sleep_time_ms);

			current_time_ms = clock.get_unix_epoch_ms();
		}
	}
};


class Phyano
{
public:

	// Alsa object were intended to be placed in a container, but the alsa object did not comply.
	// Should be resolved in the near future.
	// Should be trivial with proper move sematins in place for Alsa. Currently copy construction is disabled.
	// Arr<Alsa> alsas;

	Alsa alsa_0;
	Alsa alsa_1;
	Alsa alsa_2;
	Alsa alsa_3;
	Alsa alsa_4;
	Alsa alsa_5;
	Alsa alsa_6;
	Alsa alsa_7;

	uint available_alsa_count = 8;
	uint alsa_index = 0;

	AudioData adata; // 1 second databuffer

	// Phyano() : alsas {10}
	Phyano()
	{
		adata.set_sample_count(44100); // 1 sec

		
		for(uint i = 0; i < available_alsa_count; i++)
		{
			// Alsa alsa_tmp;
			// alsas.push_back(alsa_tmp);
			// alsas.emplace_back({});
		}
		// alsas.expand();
		// alsas.set({}); 
	}

	void press(NoteName _note_name)
	{
		AudioData note_data = Instrument::get_note_audio({_note_name, NoteType::quarter}, 120.0, 0.3);

		if(++alsa_index >= available_alsa_count)
			alsa_index = 0;

		switch (alsa_index)
		{
			case 0:
				alsa_0.play(note_data);
				break;
			case 1:
				alsa_1.play(note_data);
				break;
			case 2:
				alsa_2.play(note_data);
				break;
			case 3:
				alsa_3.play(note_data);
				break;
			case 4:
				alsa_4.play(note_data);
				break;
			case 5:
				alsa_5.play(note_data);
				break;
			case 6:
				alsa_6.play(note_data);
				break;
			case 7:
				alsa_7.play(note_data);
				break;
		
		default:
			break;
		}

		// alsa.play(note_data);
	}

	void add_press(Instrument _instument, NoteName _note_name)
	{
		AudioData note_data = _instument.get_note_audio({_note_name, NoteType::quarter}, 120.0, 0.5);

		if(note_data.sample_count() > 44100)
		{
			println("ERROR: trying to play Phyano note longer than buffer size.");
			return;
		}

		for(uint i = 0; i < note_data.sample_count(); i++)
		{
			adata.data[i] += note_data.data[i];
		}
	}

	AudioData consume_ms(uint ms)
	{
		if(ms > 1000)
		{
			println("ERROR: can't consume more than 1000 ms of Phyano data.");
			return AudioData {};
		}

		uint samples_per_ms = 44;
		uint samples_to_consume = ms * samples_per_ms;

		AudioData return_adata;
		return_adata.set_sample_count(samples_to_consume);

		for(uint i = 0; i < return_adata.sample_count(); i++)
		{
			return_adata.data[i] += adata.data[i];
		}

		shift_data(samples_to_consume);

		return return_adata;
	}


	void shift_data(uint shift_count)
	{
		// Copy
		for(uint i = shift_count; i < adata.sample_count(); i++)
		{
			adata.data[i-shift_count] = adata.data[i];
		}

		// Clear data tail
		for(uint i = (adata.sample_count() - shift_count); i < adata.sample_count(); i++)
		{
			adata.data[i] = 0;
		}
	}

};


class CLI
{
public:

	uint argc;
	Arr<Str> argv;

	CLI(int _argc, char** _argv)
	{
		if(_argc < 1)
		{
			println("ERROR: CLI argc is < 1.");
			return;
		}

		argc = (uint)_argc;
		
		for(uint i = 0; i < argc; i++)
		{
			argv.push_back(_argv[i]);
		}
	}

	Str operator[](uint _argument_index)
	{
		return argv[_argument_index];
	}

	void print()
	{
		Print::ln("CLI::print ");
		for(uint i = 0; i < argc; i++)
		{
			Print::buf("    ");
			Print::buf(Str::UI(i));
			Print::buf(": ");
			Print::ln(argv[i]);
		}
	}
};

#include "io/json/json.hh"
#include "lib/file.hh"

int main(int argc, char** argv)
{
    println("Physimos::audio starting!");

	CLI cli (argc, argv);
	// cli.print();


	// CLOCK STUFF
	Clock clock;
	printf("Unix time ms: %li \n", clock.get_unix_epoch_ms());
	clock.print_tv_start();

	SleepTimer sleep_timer;
	// sleep_timer.sleep(1000);


	if(cli[1] == "phyano")
	{
		Phyano phyano;

		Arr<Key> keys;
		EvdevReader evdev_kbd {"/dev/input/event9"};

		// Start-up jingle
		sleep_timer.sleep(200);
		phyano.press(NoteName::C4); 
		sleep_timer.sleep(200);
		phyano.press(NoteName::G4); 

		for(int i = 0; i < 12000; i++)
		{
			sleep_timer.sleep(10); // poll 1000/X times / second

			keys = evdev_kbd.get_key_presses();
			// evdev_kbd.read_and_print(); // figure out keycodes for adding new keystrokes

			for(uint i = 0; i < keys.count(); i++)
			{
				switch (keys[i])
				{
					case Key::A:
						phyano.press(NoteName::C4); 
						break;
					case Key::S:
						phyano.press(NoteName::D4); 
						break;
					case Key::D:
						phyano.press(NoteName::E4); 
						break;
					case Key::F:
						phyano.press(NoteName::F4); 
						break;
					case Key::G:
						phyano.press(NoteName::G4); 
						break;
					case Key::H:
						phyano.press(NoteName::A4); 
						break;
					case Key::J:
						phyano.press(NoteName::B4); 
						break;
					case Key::K:
						phyano.press(NoteName::C5); 
						break;

					case Key::ESC:
						Print::ln("Escape key pressed. Exiting Phyano.");

						// Shut-down jingle
						sleep_timer.sleep(200);
						phyano.press(NoteName::G4); 
						sleep_timer.sleep(350);
						phyano.press(NoteName::C4); 

						return 0; 

						break;
					
					default:
						break;
				}
			}

			keys.clear();
			
		}

	}
	else if(cli[1] == "sheet")
	{
		Print::ln("Sheet music mode selected");

		if(cli[2] == "play")
		{
			Print::ln("Play music sheet selected");

			Str sheet_file_path = cli[3];

			Print::buf("Song file: ");
			Print::ln(sheet_file_path);
			
			ResMove<Str> file_contents_mv = File::cat_as_str_core_xplat(sheet_file_path);
			if(file_contents_mv.has_error())
			{
				Print::ln("Error: unable to read json sheet file.");
				return -1;
			}
			Str file_contents = file_contents_mv.consume_value();
			Json json;
			json.try_parse(file_contents);
			j_object obj = json.get_object();
			OptPtr<Json> opt_string = json.object_find("format");
			if(opt_string.is_null())
			{
				Print::ln("Error: unable to find json format entry.");
				return -1;
			}

			Print::ln(opt_string.get_ref().get_string());
		}

	}
	else if(cli[1] == "DFT")
	{
		std::complex<double> a = 1;
		std::complex<double> b = 2.0 - 1.0i;
		std::complex<double> c = -1.0i;
		std::complex<double> d = -1.0 + 2.0i;
	
		Arr<std::complex<double>> input_arr;
		input_arr.push_back(a);
		input_arr.push_back(b);
		input_arr.push_back(c);
		input_arr.push_back(d);
		Vec<std::complex<double>> input_vec {4};
		input_vec[0]= a;
		input_vec[1]= b;
		input_vec[2]= c;
		input_vec[3]= d;

		// Soinusoid
		uint sample_count = 21;
		double t0 = 0;
		double tf = 1.0;
		double dt = (tf - t0) / ((double) (sample_count-1));

		Vec<std::complex<double>> sinusoid { sample_count };
		std::complex<double> ampl = 10.0;
		std::complex<double> freq = 2.0;

		
		
		double i_d;
		for(uint i = 0; i < sample_count; i++)
		{
			i_d = (double) i;
			sinusoid[i] = ampl * std::sin( 2 * 3.1415 * freq * (i_d * dt) );
		}

		print_complex_vec(sinusoid);

		// Arr<std::complex<double>> output = DFT::calculate(input_arr);
		// Vec<std::complex<double>> output = DFT::calculate(input_vec);
		Vec<std::complex<double>> output = DFT::calculate(sinusoid);

		print_complex_vec(output);

		print_vec(complex_vec_to_mag_vec(output));

		

		// for(uint i = 0; i < output.count(); i++)
		// for(uint i = 0; i < output.size(); i++)
		// {
		// 	print(Str::FL(output[i].real(), 3, Str::FloatRep::Fixed));
		// 	print(" + ");
		// 	print(Str::FL(output[i].imag(), 3, Str::FloatRep::Fixed));
		// 	print(" i \n");
		// }

		return 0;
	}


	// Print argv
	// print("\n");
	// for(int i = 0; i < argc; i++)
	// {
	// 	print(argv[i]);
	// 	print("\n");
	// }

	// ambiance_song();


	// alsa0.end();    

	// while(1) {}



	// SineWave sine_wave {1.0};
	Arr<Frequency> frequencies;
	// frequencies.push_back({3520.0, 0.05});
	// frequencies.push_back({1760.0, 0.1});
	// frequencies.push_back({880.0, 0.5});
	// frequencies.push_back({440.0, 0.15});
	// frequencies.push_back({220.0, 0.10});
	// frequencies.push_back({110.0, 0.10});

	frequencies.push_back({880.0, 	0.15});
	// frequencies.push_back({660.0, 	0.15});
	// frequencies.push_back({440.0, 	0.20});
	// frequencies.push_back({220.0, 	0.70});

	// frequencies.push_back(900.0);
	// frequencies.push_back(800.0);
	// frequencies.push_back(700.0);
	// frequencies.push_back(600.0);
	// frequencies.push_back({560.0, 0.02});
	// frequencies.push_back({760.0, 0.02});
	// frequencies.push_back({80.0, 0.15});

	WaveConfig w_config;
	WaveGen wave_generator {1.0, frequencies};
	// sine_wave.set_config(default_wave_config);
	// sine_wave.w_config.gain = 0.2;
	wave_generator.generate_wave();
	// sine_wave.print_wave();

	WAV wav; 
	// wav.populate_from_wave(sine_wave);
	// wav.to_little_endian();
	// wav.write_to_file("tmp/test2.wav");
	wav.read_file("resources/audio/sine.wav");
	wav.write_to_file("tmp/sine_out.wav");
	print("Reading Wav file. Size = ");
	print(Str::CH(wav.data_chunk.DataBlocID[1]));
	println(Str::SI(wav.data_chunk.DataSize));
	bin_dump("tmp/dump.bin", &wav, 44);

	// Print sizes of Wav structs
	println(Str::SI(sizeof(WavHeaderRIFF)));
	println(Str::SI(sizeof(WavHeaderDataFormat)));
	println(Str::SI(sizeof(WavDataChunk)));

	println(Str::SI(sizeof(WAV)));


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
		rate 	 = 44100;
		channels = 1;
	}


	WaveGen wave_100hz(0.5, 300.0);
	wave_100hz.generate_wave();

	WaveGen wave_1000hz(0.5, 1000.0);
	wave_1000hz.generate_wave();

	

	// Alsa alsa { 
	// 	rate, 
	// 	channels,
	// 	"resources/audio/sine.wav"
	// };
	Alsa alsa;

	AudioData audio_data;
	audio_data.data = wave_generator.out_arr;

	alsa.print_pcm_info();
	// alsa.play();
	// alsa.play(audio_data);
	// alsa.play(wave_100hz.get_audio_data());
	// alsa.play(wave_1000hz.get_audio_data());
	// alsa.end();

	// twinkle_twinkle();
	ambiance_song();


    printf("End Alsa test\n");

	return 0;
}



int main_2(int argc, char** argv)
{
    println("Physimos::audio starting!");


	// twinkle_twinkle();
	ambiance_song();


    printf("End Alsa test\n");

	return 0;
}