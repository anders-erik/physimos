
#pragma once

#include <cstdint> // fixed width ints
#include <stdio.h>

#include <alsa/asoundlib.h>
#define PCM_DEVICE "default"

#include "lib/str.hh"
#include "lib/print.hh"
#include "lib/arr.hh"

#include "audio_data.hh"


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
	
	Alsa();
	Alsa(uint rate, uint channels, Str file_path);
	Alsa(const Alsa&) = delete;

	~Alsa();

	void setup();
	void print_pcm_info();
	int play();
	void play(AudioData audio_data);
	
};