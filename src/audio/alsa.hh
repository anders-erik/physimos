
#pragma once

#include <cstdint> // fixed width ints
#include <stdio.h>

#include <alsa/asoundlib.h>
#define PCM_DEVICE "default"

#include "lib/str.hh"
#include "lib/print.hh"
#include "lib/arr.hh"

#include "audio_data.hh"


struct Alsa
{
	unsigned int sample_rate = 44100;
	unsigned int channel_count = 1;
	int bytes_per_sample = 2; // 16 bit integers is default

	snd_pcm_t *pcm_handle;
	snd_pcm_hw_params_t *parameters_hw;
	/** 
		The time (in microseconds) between HW interrupts.
		The duration is tied to the frame count value and the sample rate and is retrieved by querying aAlsa after setting the hw parameters.
	 */
	u32 interrupt_delta_time_us;
	/**
		frame := a block containing a single sample from each channel
		This value is set by alsa after writing the configured parameters and is equal to the number of frames the hardware will process at a time
	 */
	snd_pcm_uframes_t interrupt_frame_count;
	
	/** 
		Buffer of size equal to the configured interrupt time/sample size/rate.
		The contents of this buffer is what is being sent to the hardware
	 */
	char *chunk_buffer;
	/** The size of one chunk, which is the number of bytes written at a time to the configured hardware. */
	int chunk_buffer_size;
	
	Alsa();
	Alsa(const Alsa&) = delete;
	~Alsa();

	void setup();
	void print_pcm_info();

	void play(AudioData16& audio_data);
	void stop();
};