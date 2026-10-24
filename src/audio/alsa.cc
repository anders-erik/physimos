
#include "audio/alsa.hh"


Alsa::Alsa()
{
	setup();
}


Alsa::~Alsa()
{
	snd_pcm_drain(pcm_handle);
	snd_pcm_close(pcm_handle);
	free(chunk_buffer);
}


void Alsa::setup()
{
	u32 ret = 0;

	/* Open the PCM device in playback mode */
	// if (ret = snd_pcm_open(&pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK) < 0) 
	if ((ret = snd_pcm_open(&pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0)) < 0) 
		printf("ERROR: Can't open \"%s\" PCM device. %s\n", PCM_DEVICE, snd_strerror(ret));

	/* Allocate parameters object and fill it with default values */
	snd_pcm_hw_params_alloca(&parameters_hw);
	snd_pcm_hw_params_any(pcm_handle, parameters_hw);

	/* Set parameters */
	if ((ret = snd_pcm_hw_params_set_access(	pcm_handle, 
												parameters_hw,
												SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) 
		printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(ret));

	if ((ret = snd_pcm_hw_params_set_format(	pcm_handle, 
												parameters_hw,
												SND_PCM_FORMAT_S16_LE)) < 0) 
		printf("ERROR: Can't set format. %s\n", snd_strerror(ret));

	if ((ret = snd_pcm_hw_params_set_channels(pcm_handle, parameters_hw, channel_count)) < 0) 
		printf("ERROR: Can't set channels number. %s\n", snd_strerror(ret));

	if ((ret = snd_pcm_hw_params_set_rate_near(pcm_handle, parameters_hw, &sample_rate, 0)) < 0) 
		printf("ERROR: Can't set rate. %s\n", snd_strerror(ret));

	/* Write parameters */
	if ((ret = snd_pcm_hw_params(pcm_handle, parameters_hw)) < 0)
		printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(ret));
	
	/* Retrieve interrupt duration for later writing */
	snd_pcm_hw_params_get_period_size(parameters_hw, &interrupt_frame_count, 0);
	snd_pcm_hw_params_get_period_time(parameters_hw, &interrupt_delta_time_us, NULL);


	chunk_buffer_size = interrupt_frame_count * channel_count * bytes_per_sample;
	chunk_buffer = (char *) malloc(chunk_buffer_size);
}


void Alsa::print_pcm_info()
{
	u32 tmp = 0;

	/* Resume information */
	printf("PCM name: '%s'\n", snd_pcm_name(pcm_handle));

	printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(pcm_handle)));

	snd_pcm_hw_params_get_channels(parameters_hw, &tmp);
	printf("channels: %i ", tmp);

	if (tmp == 1)
		printf("(mono)\n");
	else if (tmp == 2)
		printf("(stereo)\n");

	snd_pcm_hw_params_get_rate(parameters_hw, &tmp, 0);
	printf("rate: %d bps\n", tmp);

}


void Alsa::play(AudioData16& audio_data)
{
	int loop_count = audio_data.duration_us() / interrupt_delta_time_us;
	long long_ret = 0;

	for (int loop_i = 0; loop_i < loop_count; loop_i++)
	{
		int frame_offset = loop_i * interrupt_frame_count;
		
		memcpy(	chunk_buffer,
				audio_data.data.data_mut() + frame_offset,
				chunk_buffer_size);

		if ((long_ret = snd_pcm_writei(pcm_handle, chunk_buffer, interrupt_frame_count)) == -EPIPE)
		{
			// snd_pcm_drop(pcm_handle);
			// 	return;
			printf("XRUN. snd_pcm_writei return value: %li  (Buffer underrun). \n", long_ret);
			snd_pcm_prepare(pcm_handle);
		}
		else if (long_ret < 0)
		{
			printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(long_ret));
		}
		// printf("     snd_pcm_writei return value: %li \n", long_ret);

	}

	// snd_pcm_drain(pcm_handle);
}


void Alsa::stop()
{
	snd_pcm_drop(pcm_handle);
	snd_pcm_close(pcm_handle);
	free(chunk_buffer);
	setup(); // recreate the handle that we just dropped!
}