#pragma once

#include "audio/alsa.hh"

struct AudioPlayer
{
	enum class Type
	{
		Alsa
	} type;

	union Value
	{
		Alsa* alsa;
	} value;

	AudioPlayer() = default;

    AudioPlayer(AudioPlayer::Type _type)
    {
        if(_type == AudioPlayer::Type::Alsa)
            set_backend_to_alsa();
    };

	~AudioPlayer()
	{
		if(type == Type::Alsa)
		{
			delete value.alsa;
		}
	}

    void set_backend_to_alsa()
	{
		type = AudioPlayer::Type::Alsa;
		value.alsa = new Alsa();
	}

	void play(AudioData16& _audio_data)
	{
		if(type == Type::Alsa)
		{
			value.alsa->play(_audio_data);
		}
	}

	
};