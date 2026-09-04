
#pragma once

#include <alsa/asoundlib.h>
#include <stdio.h>
#include <cstdint> // fixed width ints

#include "audio_data.hh"


#define PCM_DEVICE "default"

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