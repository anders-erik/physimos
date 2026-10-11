#pragma once

#include "audio/alsa.hh"
#include "audio/note.hh"
#include "audio/instrument.hh"
#include "audio/audio_data.hh"
#include "audio/wave_generator.hh"


/* Enables the playing of audio data in parallel which is not possible using the regular Alsa objects. */
struct AlsaParallel
{
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

	AlsaParallel()
	// AlsaParallel(uint _alsa_count)
	{
		for(uint i = 0; i < available_alsa_count; i++)
		{
			// Alsa alsa_tmp;
			// alsas.push_back(alsa_tmp);
			// alsas.emplace_back({});
		}
		// alsas.expand();
		// alsas.set({}); 
	}


	void play(AudioData16& audio_data)
	{
		if(++alsa_index >= available_alsa_count)
			alsa_index = 0;

		switch (alsa_index)
		{
			case 0:
				alsa_0.play(audio_data);
				break;
			case 1:
				alsa_1.play(audio_data);
				break;
			case 2:
				alsa_2.play(audio_data);
				break;
			case 3:
				alsa_3.play(audio_data);
				break;
			case 4:
				alsa_4.play(audio_data);
				break;
			case 5:
				alsa_5.play(audio_data);
				break;
			case 6:
				alsa_6.play(audio_data);
				break;
			case 7:
				alsa_7.play(audio_data);
				break;
		
			default:
				break;
		}
	}
};


/* Generates and plays notes */
class Phyano
{
public:

	AlsaParallel alsa_parallel;
	SynthType synth_type = SynthType::Piano;


	Phyano() {}

	void press(NoteName _note_name)
	{
		Print::ln(Str{"Pressed:  ", NoteSerializer::note_name_to_str(_note_name)});

		AudioData16 note_data;
		
		if(synth_type == SynthType::Pure)
		{
			note_data = Instrument::get_note_audio({_note_name, NoteType::quarter}, 120.0, 0.3);
		}
		else if(synth_type == SynthType::Piano)
		{
			AudioSamplingConfig sampling_config = {AudioSamplingConfig::RATE_DURATION, 44100, 800000};
			note_data = PianoWave::generate(sampling_config, NoteFrequencies::to_frequency(_note_name));
		}

		alsa_parallel.play(note_data);
		return;
	}

	void release(NoteName _note_name)
	{
		Print::ln(Str{"Released: ", NoteSerializer::note_name_to_str(_note_name)});
	}

	AudioData16 get_press_data(NoteName _note_name)
	{
		AudioData16 note_data;
		
		if(synth_type == SynthType::Pure)
		{
			note_data = Instrument::get_note_audio({_note_name, NoteType::quarter}, 120.0, 0.3);
		}
		else if(synth_type == SynthType::Piano)
		{
			AudioSamplingConfig sampling_config = {AudioSamplingConfig::RATE_DURATION, 44100, 800000};
			note_data = PianoWave::generate(sampling_config, NoteFrequencies::to_frequency(_note_name));
		}

		return note_data;
	}

};