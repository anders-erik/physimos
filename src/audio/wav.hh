#pragma once


#include "lib/str.hh"
#include "lib/arr.hh"

#include "wave_gen.hh"




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
